function txt=xml2tex(xmldata)
// xmldata can be a file name as a string or 
// a gmarkup node.

  function [txt,data]=generate_MAN(G,data)
    [contents,data] = explore_children(G,data,node_only=%t);
    txt = ["% -*- mode: latex -*-";
	   contents];
  endfunction

  function [txt,data]=generate_TEXT(G,data)
    [txt,data] = explore_children(G,data,node_only=%f);
  endfunction

  function [txt,data]=generate_IGNORE(G,data)
    data=data;
    txt = m2s(zeros(0,1));

  endfunction

  function [txt,data]=generate_USED_FUNCTIONS(G,data)
    [contents,data] = explore_children(G,data,node_only=%t);
    txt = ["\\paragraph{Used functions}";
	   catenate(contents,sep=", ")];
  endfunction
  
  function [txt,data]=generate_USED_FUNCTION(G,data)
    [contents,data] = explore_children(G,data);
    txt = stripblanks(contents);
  endfunction
  
  function [txt,data]=generate_SEE_ALSO_ITEMS(G,data)
    [contents,data] = explore_children(G,data,node_only=%t);
    I=length(contents)==0;
    contents(I)=[];
    txt = ["\\begin{manseealso}";
	   catenate(contents,sep=",\n");
	   "\\end{manseealso}"];
  endfunction
  
  function [txt,data]=generate_SEE_ALSO(G,data)
    txt =  stripblanks(G.children(1));
    txt_p = strsubst(txt,"_","\\_");
    txt = sprintf(" \\manlink{%s}{%s}",txt_p,txt);
  endfunction

  function [txt,data]=generate_LINK(G,data)
    txt =  stripblanks(G.children(1));
    txt_p = strsubst(txt,"_","\\_");
    txt = sprintf(" \\manlink{%s}{%s}",txt_p,txt);
  endfunction
  
  function [txt,data]=generate_PARAMS(G,data,node_only=%t)
    [contents,data] = explore_children(G,data);
    I=length(contents)==0;
    contents(I)=[];
    if isempty(contents) then txt=m2s(zeros(0,1)); return;end
    txt = ["\\begin{parameters}";
	   "  \\begin{varlist}";
	   "    "+contents;
	   "  \\end{varlist}";
	   "\\end{parameters}"];
  endfunction
  
  function [txt,data]=generate_PARAM(G,data)
    if G.attributes.iskey['type'] then 
      txt= sprintf("\\vname{%s}: Type ''%s'', size \\verb@%s@.",...
		   latexsubst(G.attributes.name),...
		   G.attributes.type,...
		   G.attributes.size);
    else
      contents = explore_children(G,data);
      contents = stripblanks(contents);	  
      txt=sprintf("\\vname{%s}: %s",latexsubst(G.attributes.name),...
		  contents);
    end
  endfunction
  
  function [txt,data]=generate_AUTHORS(G,data)
    [contents,data] = explore_children(G,data,node_only=%t);
    txt = ["\\begin{authors}";
	   "  "+catenate(contents,sep=",");
	   "\\end{authors}"];
  endfunction
  
  function [txt,data]=generate_AUTHOR(G,data)
    if G.attributes.iskey['name'] then 
      txt =[G.attributes.name];
    elseif G.attributes.iskey['url']
      txt =[G.attributes.url];
    end
    if G.attributes.iskey['affiliation'] then 
      txt =sprintf("%s (%s)",txt,G.attributes.affiliation);
    end
  endfunction
  
  function [txt,data]=explore_children(G,data,node_only=%f)
    // only explore the children which are nodes i.e get rid 
    // of intermediate strings.
    txt=m2s(zeros(0,1));
    data.depth = data.depth+1;
    for i= 1:length(G.children)
      node = G.children(i);
      if node_only && type(node,'short') <> 'gmn' then continue;end
      [txt1,data]=markup_explore(node,data);
      txt.concatd[txt1];
    end
  endfunction
  
  function [txt,data]=generate_LANGUAGE(G,data)
    [txt,data]=explore_children(G,data);
    txt = "%% language: " + txt 
  endfunction

  function [txt,data]=generate_DATE(G,data)
    [txt,data]=explore_children(G,data);
    txt = "%% date: " + txt 
  endfunction

  function [txt,data]=generate_SHORT_DESCRIPTION(G,data)
    [txt,data]=explore_children(G,data);
    if strstr(data.title,"_")==0 then 
      txt = ["\\begin{mandesc}";
	     sprintf(" \\short{%s}{%s (%s)} \\\\",data.title, latexsubst(txt), data.type);
	     "\\end{mandesc}"];
    else
      str = strsubst(data.title,"_","\\_");
      txt = ["\\begin{mandesc}";
	     sprintf(" \\shortunder{%s}{%s}{%s (%s)} \\\\",str, data.title, txt, data.type);
	     "\\end{mandesc}"];
    end
  endfunction
  
  function [txt,data]=generate_TITLE(G,data)
    [contents,data]=explore_children(G,data);
    contents = stripblanks(contents);
    txt = [sprintf("\\mansection{%s}",strsubst(contents,"_","\\_"))];
    data.title = contents;
  endfunction

  function [txt,data]=generate_TYPE(G,data)
    [contents,data]=explore_children(G,data);
    txt = "%% Type: " + contents;
    data.type = contents;
  endfunction

  function [txt,data]=generate_DESCRIPTION(G,data)
    [contents,data] =explore_children(G,data);
    txt = ["\\begin{mandescription}";
	   contents;
	   "\\end{mandescription}"];
    
  endfunction
  
  function [txt,data]=generate_PROPERTIES(G,data)
    [contents,data] =explore_children(G,data,node_only=%t);
    txt = ["\\paragraph{Default Properties}";
	   "\\begin{itemize}";
	   contents;
	   "\\end{itemize}"];
  endfunction

  function [txt,data]=generate_PROPERTY(G,data)
    [contents,data] =explore_children(G,data)
    contents = strsubst(contents,"_","\\_");
    contents = stripblanks(contents);
    txt = [sprintf("  \\item %s: %s", G.attributes.name, contents)];
  endfunction
  
  function [txt,data]=generate_default(G,data)
    [contents,data] =explore_children(G,data);
    txt = [sprintf("%% default-generator %s",G.name);
	   sprintf("%% begin %s",G.name);
	   contents;
	   sprintf("%% end %s",G.name)];
  endfunction
  
  function [txt,data]=markup_explore(G,data)
    if nargin < 2 then data=hash(depth=0);end 
    if type(G,'short') == 's' then 
      txt=stripblanks(G);return;
    end;
    if type(G,'short') <> 'gmn' then txt=m2s(zeros(0,1));return;end 
    func = sprintf('generate_%s',G.name);
    if ~exists(func,'callers') then 
      printf("You must implement generate_%s \n",G.name);
      func= 'generate_default';
    end
    ok=execstr(sprintf('[txt,data]=%s(G,data)',func));
    if ~ok then 
      printf("Error in %s\n%s",func,catenate(lasterror(),sep='\n'));
      txt=m2s(zeros(0,1));
      return;
    end
  endfunction
  
  if type(xmldata,'short') == 's' then 
    if ~file('exists',xmldata) then
      msg=sprintf("Error: xml file ""%s.xml"" not found.",xmldata);
      error(msg);
      return;
    end
    G=gmarkup(xmldata);
  else
    G=xmldata
  end
  
  txt=markup_explore(G);
endfunction

