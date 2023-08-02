
function txt=return_desc_block(name)
  txt=m2s(zeros(0,1));
  [ok,data]=return_block_getvalue_info(name,path);
  if ~ok then return;end 
  if length(data) == 0 then return;end 
  txt = data(1);
endfunction

function txt=return_lables_block(name,path)
//sortie : txt : une matrice de chaîne de caractères
//               de taille mx2 donnant pour chaque
//               paramètres de la boîte de dialogue :
//               txt(,1) : le type (ex 'vec', 'mat',...)
//               txt(,2) : la taille (ex -1, 2, [3,2],...)
//
  txt=m2s(zeros(0,1));
  [ok,data]=return_block_getvalue_info(name,path);
  if ~ok then return;end 
  if length(data) == 0 then return;end 
  txt = data(2)(:);
endfunction

function txt=return_typ_block(name,path)
  txt=m2s(zeros(0,1));
  [ok,data]=return_block_getvalue_info(name,path);
  if ~ok then return;end 
  if length(data) == 0 then return;end 
  data = data(3);
  for i=1:(length(data)/2) do 
    txt.concatd[[data(2*i-1),sci2exp(data(2*i))]];
  end
endfunction

function txt=return_ini_block(name)
  txt=m2s(zeros(0,1));
  [ok,data]=return_block_getvalue_info(name,path);
  if ~ok then return;end 
  if length(data) == 0 then return;end 
  txt = data(4);
endfunction

function [ok,data]=return_block_getvalue_info(name,path)  
// utility function which collects data for a specific block 
// using a redefinition of the getvalue function 
  ok = %f; data=list();
  
  if nargin<2 then path = "";end 
  if ~file('exists',path) then path="";end 
  
  if ~exists(name, 'callable') then
    if path == "" then
      error(sprintf("Error: function %s not found",name));
      return;
    end
    // try to load the function 
    fname = path+name+'.sci';
    if ~file('exists',fname) then
      error(sprintf("Error: file %s not found\n",fname));
      return
    end
    ok = exec(fname,errcatch=%t);
    if ~ok then
      error(sprintf("Error: failed to load %s",fname));
      return;
    end
  end

  %scicos_prob=%f;
  alreadyran=%f
  needcompile=4
  //redefine getvalue, edit_curv
  // getvalue=mgetvalue;
  // edit_curv=medit_curv;
  // SUPER_f=MSUPER_f;
  // PAL_f=MPAL_f;
  // dialog=mmdialog;
  
  function [ok,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20]=...
	getvalue_doc(%desc,%labels,%typ,%ini)
    global par_types
    par_types=list(%desc,%labels,%typ,%ini);
    function L=xfun(desc,list,flag) L=x_choices(desc,list,flag);endfunction
    [ok,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20]=...
	getvalue_internal(%desc,%labels,%typ,%ini,use_dialog=%f,ch_fun=xfun);
  endfunction
  
  function result=scicos_editsmat_id(title,txt,comment='Enter code:') 
    result = txt
  endfunction
    
  getvalue=getvalue_doc;
  scicos_editsmat = scicos_editsmat_id;
  
  // retrieve labels of getvalue fonction
  global(par_types=list());
  
  ok=execstr('blk='+name+'(''define'')',errcatch=%t)
  if ~ok then
    error(catenate(lasterror(),sep='\n'));
    error(sprintf("Error: failed to define a %s block",name));
    return
  end
  
  ok =execstr('blk='+name+'(''set'',blk)',errcatch=%t)
  if ~ok then
    error(catenate(lasterror(),sep='\n'));
    error(sprintf("Error: failed to call set for a %s block",name));
    return
  end
  data=par_types 
  clearglobal par_types;
  ok = %t;
endfunction
