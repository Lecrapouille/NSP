/* 
 * tclWinFile.c --
 *
 *      This file contains temporary wrappers around UNIX file handling
 *      functions. These wrappers map the UNIX functions to Win32 HANDLE-style
 *      files, which can be manipulated through the Win32 console redirection
 *      interfaces.
 *
 * Copyright (c) 1995-1996 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * SCCS: @(#) tclWinFile.c 1.44 97/08/05 11:45:34
 */

#include "tclWinInt.h"
#include <shlobj.h>
#include <sys/stat.h>


/*
 * The variable below caches the name of the current working directory
 * in order to avoid repeated calls to getcwd.  The string is malloc-ed.
 * NULL means the cache needs to be refreshed.
 */

static char *currentDir =  NULL;


/*
 *----------------------------------------------------------------------
 *
 * Tcl_FindExecutable --
 *
 *	This procedure computes the absolute path name of the current
 *	application, given its argv[0] value.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The variable tclExecutableName gets filled in with the file
 *	name for the application, if we figured it out.  If we couldn't
 *	figure it out, Tcl_FindExecutable is set to NULL.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_FindExecutable(argv0)
    char *argv0;		/* The value of the application's argv[0]. */
{
    nsp_tcldstring buffer;
    int length;

    nsp_tcldstring_init(&buffer);

    if (tclExecutableName != NULL) {
	ckfree(tclExecutableName);
	tclExecutableName = NULL;
    }

    /*
     * Under Windows we ignore argv0, and return the path for the file used to
     * create this process.
     */

    nsp_tcldstring_set_length(&buffer, MAXPATHLEN+1);
    length = GetModuleFileName(NULL, nsp_tcldstring_value(&buffer), MAXPATHLEN+1);
    if (length > 0) {
	tclExecutableName = (char *) ckalloc((unsigned) (length + 1));
	strcpy(tclExecutableName, nsp_tcldstring_value(&buffer));
    }
    nsp_tcldstring_free(&buffer);
}

/*
 *----------------------------------------------------------------------
 *
 * TclMatchFiles --
 *
 *	This routine is used by the globbing code to search a
 *	directory for all files which match a given pattern.
 *
 * Results: 
 *	If the tail argument is NULL, then the matching files are
 *	added to the interp->result.  Otherwise, nsp_do_glob is called
 *	recursively for each matching subdirectory.  The return value
 *	is a standard Tcl result indicating whether an error occurred
 *	in globbing.
 *
 * Side effects:
 *	None.
 *
 *---------------------------------------------------------------------- */

int
nsp_match_files( separators, dirPtr, pattern, tail,S)
    char *separators;		/* Directory separators to pass to nsp_do_glob. */
    nsp_tcldstring *dirPtr;	/* Contains path to directory to search. */
    char *pattern;		/* Pattern to match against. */
    char *tail;			/* Pointer to end of pattern.  Tail must
				 * point to a location in pattern. */
    NspSMatrix *S;
{
    char drivePattern[4] = "?:\\";
    char *newPattern, *p, *dir, *root, c;
    int length, matchDotFiles;
    int result = TCL_OK;
    int baseLength = nsp_tcldstring_length(dirPtr);
    nsp_tcldstring buffer;
    DWORD atts, volFlags;
    HANDLE handle;
    WIN32_FIND_DATA data;
    BOOL found=0;

    /*
     * Convert the path to normalized form since some interfaces only
     * accept backslashes.  Also, ensure that the directory ends with a
     * separator character.
     */

    nsp_tcldstring_init(&buffer);
    if (baseLength == 0) {
	nsp_tcldstring_append(&buffer, ".", 1);
    } else {
	nsp_tcldstring_append(&buffer, nsp_tcldstring_value(dirPtr),
		nsp_tcldstring_length(dirPtr));
    }
    for (p = nsp_tcldstring_value(&buffer); *p != '\0'; p++) {
	if (*p == '/') {
	    *p = '\\';
	}
    }
    p--;
    if (*p != '\\' && *p != ':') {
	nsp_tcldstring_append(&buffer, "\\", 1);
    }
    dir = nsp_tcldstring_value(&buffer);
    
    /*
     * First verify that the specified path is actually a directory.
     */

    atts = GetFileAttributes(dir);
    if ((atts == 0xFFFFFFFF) || ((atts & FILE_ATTRIBUTE_DIRECTORY) == 0)) {
	nsp_tcldstring_free(&buffer);
	return TCL_OK;
    }

    /*
     * Next check the volume information for the directory to see whether
     * comparisons should be case sensitive or not.  If the root is null, then
     * we use the root of the current directory.  If the root is just a drive
     * specifier, we use the root directory of the given drive.
     */

    switch (nsp_get_path_type(dir)) {
	case TCL_PATH_RELATIVE:
	    found = GetVolumeInformation(NULL, NULL, 0, NULL,
		    NULL, &volFlags, NULL, 0);
	    break;
	case TCL_PATH_VOLUME_RELATIVE:
	    if (*dir == '\\') {
		root = NULL;
	    } else {
		root = drivePattern;
		*root = *dir;
	    }
	    found = GetVolumeInformation(root, NULL, 0, NULL,
		    NULL, &volFlags, NULL, 0);
	    break;
	case TCL_PATH_ABSOLUTE:
	    if (dir[1] == ':') {
		root = drivePattern;
		*root = *dir;
		found = GetVolumeInformation(root, NULL, 0, NULL,
			NULL, &volFlags, NULL, 0);
	    } else if (dir[1] == '\\') {
		p = strchr(dir+2, '\\');
		p = strchr(p+1, '\\');
		p++;
		c = *p;
		*p = 0;
		found = GetVolumeInformation(dir, NULL, 0, NULL,
			NULL, &volFlags, NULL, 0);
		*p = c;
	    }
	    break;
    }

    if (!found) {
	nsp_tcldstring_free(&buffer);
	TclWinConvertError(GetLastError());
	Scierror("couldn't read volume information for \"%s\": %s\n",
		 dirPtr->string, nsp_posix_error());
	return TCL_ERROR;
    }
    
    /*
     * If the volume is not case sensitive, then we need to convert the pattern
     * to lower case.
     */

    length = tail - pattern;
    newPattern = ckalloc(length+1);
    if (volFlags & FS_CASE_SENSITIVE) {
	strncpy(newPattern, pattern, length);
	newPattern[length] = '\0';
    } else {
	char *src, *dest;
	for (src = pattern, dest = newPattern; src < tail; src++, dest++) {
	    *dest = (char) tolower(*src);
	}
	*dest = '\0';
    }
    
    /*
     * We need to check all files in the directory, so append a *.*
     * to the path. 
     */


    dir = nsp_tcldstring_append(&buffer, "*.*", 3);

    /*
     * Now open the directory for reading and iterate over the contents.
     */

    handle = FindFirstFile(dir, &data);
    nsp_tcldstring_free(&buffer);

    if (handle == INVALID_HANDLE_VALUE) {
	TclWinConvertError(GetLastError());
	Scierror("couldn't read directory \"%s\": %s\n",
		dirPtr->string, nsp_posix_error());
	ckfree(newPattern);
	return TCL_ERROR;
    }

    /*
     * Clean up the tail pointer.  Leave the tail pointing to the 
     * first character after the path separator or NULL. 
     */

    if (*tail == '\\') {
	tail++;
    }
    if (*tail == '\0') {
	tail = NULL;
    } else {
	tail++;
    }

    /*
     * Check to see if the pattern needs to compare with dot files.
     */

    if ((newPattern[0] == '.')
	    || ((pattern[0] == '\\') && (pattern[1] == '.'))) {
        matchDotFiles = 1;
    } else {
        matchDotFiles = 0;
    }

    /*
     * Now iterate over all of the files in the directory.
     */

    nsp_tcldstring_init (&buffer);
    for (found = 1; found; found = FindNextFile(handle, &data)) {
	char *matchResult;

	/*
	 * Ignore hidden files.
	 */

	if (!matchDotFiles && (data.cFileName[0] == '.')) {
	    continue;
	}

	/*
	 * Check to see if the file matches the pattern.  If the volume is not
	 * case sensitive, we need to convert the file name to lower case.  If
	 * the volume also doesn't preserve case, then we return the lower case
	 * form of the name, otherwise we return the system form.
 	 */

	matchResult = NULL;
	if (!(volFlags & FS_CASE_SENSITIVE)) {
	    nsp_tcldstring_set_length(&buffer, 0);
	    nsp_tcldstring_append(&buffer, data.cFileName, -1);
	    for (p = buffer.string; *p != '\0'; p++) {
		*p = (char) tolower(*p);
	    }
	    if (nsp_string_match(buffer.string, newPattern)) {
		if (volFlags & FS_CASE_IS_PRESERVED) {
		    matchResult = data.cFileName;
		} else {
		    matchResult = buffer.string;
		}	
	    }
	} else {
	    if (nsp_string_match(data.cFileName, newPattern)) {
		matchResult = data.cFileName;
	    }
	}

	if (matchResult == NULL) {
	    continue;
	}

	/*
	 * If the file matches, then we need to process the remainder of the
	 * path.  If there are more characters to process, then ensure matching
	 * files are directories and call nsp_do_glob. Otherwise, just add the
	 * file to the result.
	 */

	nsp_tcldstring_set_length(dirPtr, baseLength);
	nsp_tcldstring_append(dirPtr, matchResult, -1);
	if (tail == NULL) {
	    if (nsp_row_smatrix_append_string(S, dirPtr->string)== FAIL) 
	      return FAIL;
	} else {
	    atts = GetFileAttributes(dirPtr->string);
	    if (atts & FILE_ATTRIBUTE_DIRECTORY) {
		nsp_tcldstring_append(dirPtr, "/", 1);
		result = nsp_do_glob(separators, dirPtr, tail,S);
		if (result != TCL_OK) {
		    break;
		}
	    }
	}
    }

    nsp_tcldstring_free(&buffer);
    FindClose(handle);
    ckfree(newPattern);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * TclChdir --
 *
 *	Change the current working directory.
 *
 * Results:
 *	The result is a standard Tcl result.  If an error occurs and 
 *	interp isn't NULL, an error message is left in interp->result.
 *
 * Side effects:
 *	The working directory for this application is changed.  Also
 *	the cache maintained used by TclGetCwd is deallocated and
 *	set to NULL.
 *
 *----------------------------------------------------------------------
 */

int nsp_chdir(char *dirName)     		/* Path to new working directory. */
{
    if (currentDir != NULL) {
	ckfree(currentDir);
	currentDir = NULL;
    }
    if (!SetCurrentDirectory(dirName)) {
	TclWinConvertError(GetLastError());
	nsp_posix_error();
	Scierror("Error: couldn't change working directory to \"%s\"\n",  dirName);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclGetCwd --
 *
 *	Return the path name of the current working directory.
 *
 * Results:
 *	The result is the full path name of the current working
 *	directory, or NULL if an error occurred while figuring it
 *	out.  If an error occurs and interp isn't NULL, an error
 *	message is left in interp->result.
 *
 * Side effects:
 *	The path name is cached to avoid having to recompute it
 *	on future calls;  if it is already cached, the cached
 *	value is returned.
 *
 *----------------------------------------------------------------------
 */

char *nsp_get_cwd()
{
    static char buffer[MAXPATHLEN+1];
    char *bufPtr=NULL, *p;

    if (currentDir == NULL) {
	if (GetCurrentDirectory(MAXPATHLEN+1, buffer) == 0) {
	    TclWinConvertError(GetLastError());
	    if (errno == ERANGE) {
	      Scierror("working directory name is too long\n");
	    } else {
	      nsp_posix_error();
	      Scierror("Error: getting working directory name: %s\n", buffer);
	    }
	    return NULL;
	}
	/*
	 * Watch for the wierd Windows '95 c:\\UNC syntax.
	 */

	if (buffer[0] != '\0' && buffer[1] == ':' && buffer[2] == '\\'
		&& buffer[3] == '\\') {
	    bufPtr = &buffer[2];
	} else {
	    bufPtr = buffer;
	}

	/*
	 * Convert to forward slashes for easier use in scripts.
	 */

	for (p = bufPtr; *p != '\0'; p++) {
	    if (*p == '\\') {
		*p = '/';
	    }
	}
    }
    return bufPtr;
}

#if 0
/*
 *-------------------------------------------------------------------------
 *
 * TclWinResolveShortcut --
 *
 *	Resolve a potential Windows shortcut to get the actual file or 
 *	directory in question.  
 *
 * Results:
 *	Returns 1 if the shortcut could be resolved, or 0 if there was
 *	an error or if the filename was not a shortcut.
 *	If bufferPtr did hold the name of a shortcut, it is modified to
 *	hold the resolved target of the shortcut instead.
 *
 * Side effects:
 *	Loads and unloads OLE package to determine if filename refers to
 *	a shortcut.
 *
 *-------------------------------------------------------------------------
 */

int
TclWinResolveShortcut(bufferPtr)
    nsp_tcldstring *bufferPtr;	/* Holds name of file to resolve.  On 
				 * return, holds resolved file name. */
{
    HRESULT hres; 
    IShellLink *psl; 
    IPersistFile *ppf; 
    WIN32_FIND_DATA wfd; 
    WCHAR wpath[MAXPATHLEN];
    char *path, *ext;
    char realFileName[MAXPATHLEN];

    /*
     * Windows system calls do not automatically resolve
     * shortcuts like UNIX automatically will with symbolic links.
     */

    path = nsp_tcldstring_value(bufferPtr);
    ext = strrchr(path, '.');
    if ((ext == NULL) || (stricmp(ext, ".lnk") != 0)) {
	return 0;
    }

    CoInitialize(NULL);
    path = nsp_tcldstring_value(bufferPtr);
    realFileName[0] = '\0';
    hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
	    &IID_IShellLink, &psl); 
    if (SUCCEEDED(hres)) { 
	hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, &ppf);
	if (SUCCEEDED(hres)) { 
	    MultiByteToWideChar(CP_ACP, 0, path, -1, wpath, sizeof(wpath));
	    hres = ppf->lpVtbl->Load(ppf, wpath, STGM_READ); 
	    if (SUCCEEDED(hres)) {
		hres = psl->lpVtbl->Resolve(psl, NULL, 
			SLR_ANY_MATCH | SLR_NO_UI); 
		if (SUCCEEDED(hres)) { 
		    hres = psl->lpVtbl->GetPath(psl, realFileName, MAXPATHLEN, 
			    &wfd, 0);
		} 
	    } 
	    ppf->lpVtbl->Release(ppf); 
	} 
	psl->lpVtbl->Release(psl); 
    } 
    CoUninitialize();

    if (realFileName[0] != '\0') {
	nsp_tcldstring_set_length(bufferPtr, 0);
	nsp_tcldstring_append(bufferPtr, realFileName, -1);
	return 1;
    }
    return 0;
}
#endif

/*
 *----------------------------------------------------------------------
 *
 * TclWinStat, TclWinLstat --
 *
 *	These functions replace the library versions of stat and lstat.
 *
 *	The stat and lstat functions provided by some Windows compilers 
 *	are incomplete.  Ideally, a complete rewrite of stat would go
 *	here; now, the only fix is that stat("c:") used to return an
 *	error instead infor for current dir on specified drive.
 *
 * Results:
 *	See stat documentation.
 *
 * Side effects:
 *	See stat documentation.
 *
 * const char *path;		Path of file to stat (in current CP). 
 * struct stat *buf; Filled with results of stat call. 
 *----------------------------------------------------------------------
 */

int lstat(const char *path,struct stat * buf)
{
    char name[4];
    int result;

    if ((strlen(path) == 2) && (path[1] == ':')) {
	strcpy(name, path);
	name[2] = '.';
	name[3] = '\0';
	path = name;
    }

#undef stat

    result = stat(path, buf);

#ifndef _MSC_VER

    /*
     * Borland's stat doesn't take into account localtime.
     */

    if ((result == 0) && (buf->st_mtime != 0)) {
	TIME_ZONE_INFORMATION tz;
	int time, bias;

	time = GetTimeZoneInformation(&tz);
	bias = tz.Bias;
	if (time == TIME_ZONE_ID_DAYLIGHT) {
	    bias += tz.DaylightBias;
	}
	bias *= 60;
	buf->st_atime -= bias;
	buf->st_ctime -= bias;
	buf->st_mtime -= bias;
    }

#endif

    return result;
}

/*
 *---------------------------------------------------------------------------
 *
 * TclWinAccess --
 *
 *	This function replaces the library version of access.
 *
 *	The library version of access returns that all files have execute
 *	permission.
 *
 * Results:
 *	See access documentation.
 *
 * Side effects:
 *	See access documentation.
 *
 *---------------------------------------------------------------------------
 */

int
TclWinAccess(
    const char *path,		/* Path of file to access (in current CP). */
    int mode)			/* Permission setting. */
{
    int result;
    const char *p;

#undef access

    result = access(path, mode);

    if (result == 0) {
	if (mode & 1) {
	    if (GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY) {
		/*
		 * Directories are always executable. 
		 */

		return 0;
	    }
	    p = strrchr(path, '.');
	    if (p != NULL) {
		p++;
		if ((stricmp(p, "exe") == 0)
			|| (stricmp(p, "com") == 0)
			|| (stricmp(p, "bat") == 0)) {
		    /*
		     * File that ends with .exe, .com, or .bat is executable.
		     */

		    return 0;
		}
	    }
	    errno = EACCES;
	    return -1;
	}
    }
    return result;
}





/*
 *----------------------------------------------------------------------
 *
 * TclpGetUserHome --
 *
 *	This function takes the passed in user name and finds the
 *	corresponding home directory specified in the password file.
 *
 * Results:
 *	The result is a pointer to a string specifying the user's home
 *	directory, or NULL if the user's home directory could not be
 *	determined.  Storage for the result string is allocated in
 *	bufferPtr; the caller must call nsp_tcldstring_free() when the result
 *	is no longer needed.
 *
 * Side effects:
 *	None.
 *
 * name: User name for desired home directory. 
 * bufferPtr: Uninitialized or free DString filled with name of user's home directory. 
 * 
 *----------------------------------------------------------------------
 */

char *nsp_get_user_home( char *name, nsp_tcldstring *bufferPtr)
{
  return NULL;
}
