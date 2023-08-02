**Note: This repo is an unofficial copy from CVS repo with fixes to make the compilation successful.**

# New Stack Project (NSP)

Website: [cermics.enpc.fr/~jpc/nsp-tiddly](cermics.enpc.fr/~jpc/nsp-tiddly)

Nsp is a GPL Scientific Software Package.
* It is based on a complete new rewrite of ScicosLab.
* The interpreter is written in C and objects with an internal class system.
* Gtk toolkit can be used from Nsp through a set of generated wrappers. The language bindings and class for Nsp are generated, the generator being based on the pygtk generator for python.
* It is modular (modular interpreter design, possible dynamic link of internal and external libraries).
* It should compile on Linux, MacOSX-macports, MacOSX-homebrew, Windows-Cygwin, Windows-Mingwin native Win32.

Compilation:
```
./autogen.sh
./configure
make -j1
sudo make install
```

Note: compiling with more cores (i.e. `-j8`) may fail the compilation. You can alternate `make -j1` and `make -j8` until it works.
