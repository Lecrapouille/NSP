
function generate(keep_xml=%f)
  FILES=glob('NSP/toolboxes/scicos-4.4/macros/blocks/*/*.sci');
  excluded=['scicos_blk_draw_axes';'P_PROTO';'anim_pen';'bplatform2';'pde_start';'pde_set';'m_sin'];
  for i=1:size(FILES,'*') do
    name = file('tail',FILES(i));
    name = file('rootname',name);
    if size(find(name==excluded),'*')<>0 then continue;end
    printf("%d %s start",i,name);
    node=generate_xml(name);
    dir = "man/src/blocks/";
    if keep_xml then 
      fname = dir + name + '.xml'; 
      putfile(fname,node.sprintf[]);
      ok= execstr('txt=xml2tex(fname)',errcatch=%t);
    else
      ok= execstr('txt=xml2tex(node)',errcatch=%t);
    end
    if ok then 
      fname = dir + name + '.tex'; putfile(fname,txt);
    else
      printf(catenate(['failed for '+name;lasterror()],sep='\n'));
    end
    printf("... done\n",name);
  end
endfunction
