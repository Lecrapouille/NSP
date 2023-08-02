// Generate xml data for a given block name 
// the block function must be already loaded 
// lang : 'english' | 'french' 

function node=generate_xml(block_name,lang)

  function node=collect_properties(name,lang='english')
  // returns the default properties of a block 
  // as a gmarkup node.
    
    function txt=boolean2s(b,lang)
      if lang == 'english' then
	if b then txt = 'yes' else txt = 'no';end
      else
	if b then txt = 'oui' else txt = 'non';end
      end
    endfunction
    
    if nargin <2 then lang='english';end 
    
    ok =execstr('blk='+name+'(''define'')',errcatch=%t)
    if ~ok then
      error(sprintf('Error: failed to execute %s",name));
      return
    end
    
    deput=blk.model.dep_ut
    if typeof(blk.model.sim)=='list' then
      ftype= string(blk.model.sim(2))
    else
      ftype=""
    end
    
    function txt=collect_regular_inputs()
      if size(blk.model.in,'*') <> 0 then 
	txt=[ "regular inputs",  string(size(blk.model.in,'*'))+' / '+...
	      strcat('['+string(blk.model.in(:))+','+string(blk.model.in2(:))+']',', ')+' / '+...
	      strcat(string(blk.model.intyp),', ')];
      else
	txt = m2s(zeros(0,2));
      end
    endfunction

    function txt=collect_regular_outputs()
      if size(blk.model.out,'*') <> 0 then 
	txt =["regular outputs",  string(size(blk.model.out,'*'))+' / '+...
	      strcat('['+string(blk.model.out(:))+','+string(blk.model.out2(:))+']',', ')+' / '+...
	      strcat(string(blk.model.outtyp),', ') ];
      else
	txt = m2s(zeros(0,2));
      end
    endfunction
    
    txt=["always active", boolean2s(deput(2));
	 "direct-feedthrough", boolean2s(deput(1));
	 "zero-crossing", boolean2s(blk.model.nzcross<>0);
	 "mode", boolean2s(blk.model.nmode<>0);
	 collect_regular_inputs();
	 collect_regular_outputs();
	 "number/sizes of activation inputs",string(size(blk.model.evtin,'*'))
	 "number/sizes of activation outputs",string(size(blk.model.evtout,'*'))
	 "continuous-time state", boolean2s(~isempty(blk.model.state));
	 "discrete-time state", boolean2s(~isempty(blk.model.dstate));
	 "object discrete-time state",boolean2s(~blk.model.odstate.equal[list()]);
	 "name of computational function",string(blk.model.sim(1));
	 "type of computational function", ftype];
    
    properties=list();
    for i=1:size(txt,1)
      node = gmarkup_node_create(name="PROPERTY",attributes=hash(name=txt(i,1)),children=list(txt(i,2)));
      properties.add_last[node];
    end
    node = gmarkup_node_create(name="PROPERTIES",children=properties);
    
  endfunction
    
  function node = collect_block_parameters(name)
  // collect block parameters by executing the block 
    tt_typ=return_typ_block(name,block_name);
    tt_lables=return_lables_block(name,block_name);
    if size(tt_lables,1) == 0 then 
      // we could insert a no parameters tag 
      node = gmarkup_node_create(name="IGNORE");
      return;
    end
    params=list();
    for i=1:size(tt_lables,1)
      attributes = hash(name=tt_lables(i,1),type=tt_typ(i,1),size=tt_typ(i,2));
      node = gmarkup_node_create(name="PARAM",attributes=attributes);
      params.add_last[node];
    end
    node = gmarkup_node_create(name="PARAMS",children=params);
  endfunction
  
  function node = collect_parameters(name)
  // parameters 
    node = collect_tag(name,'PARAMETERS',ignore=%t,gmn_only=%t);
    if node.name == "IGNORE" then
      node =collect_block_parameters(name);
    end
  endfunction
  
  function onode = node_clean_nonmarkup(node)
  // remove the childs which are not gmn 
    nodes=list();
    for i=1:length(node.children) do
      if type(node.children(i),'short')=='gmn' then 
	nodes.add_last[node.children(i)];
      end
    end
    onode = gmarkup_node_create(name=node.name,attributes=node.attributes, ...
				children=nodes);
  endfunction
  
  
  function res = collect_tag(name,tag,ignore=%t,gmn_only=%f)
    // collect data for a given tag in a general 
    // file 
    fname = sprintf('data/%s.xml',tolower(tag));
    G=gmarkup(fname);
    for i=1:length(G.children) do
      node = G.children(i);
      if type(node,'short')=='gmn' && node.attributes.iskey['name'] then 
	if node.attributes.name == name then 
	  res = node;
	  if gmn_only then res=node_clean_nonmarkup(node);end
	  return;
	end
      end
    end
    if ignore then 
      res =gmarkup_node_create(name="IGNORE");
    else
      printf("\nWarning: missing %s for %s\n",tag,name);
      res=gmarkup_node_create(name=tag,attributes=hash(name=name),...
			      children=list(sprintf("Missing %s for %s",tag,name)));
    end
  endfunction

  function node = collect_short_description(name,ignore=%t)
    node = collect_tag(name,'SHORT_DESCRIPTION',ignore=ignore);
  endfunction
  
  function node = collect_calling_sequence(name,ignore=%t)
    node = collect_tag(name,'CALLING_SEQUENCE',ignore=ignore);
  endfunction
  
  function node = collect_description(name,ignore=%t)
    node = collect_tag(name,'DESCRIPTION',ignore=ignore);
  endfunction

  function node = collect_used_function(name,ignore=%t)
    node = collect_tag(name,'USED_FUNCTIONS',ignore=ignore);
  endfunction
  
  function node = collect_see_also(name,ignore=%t)
    node = collect_tag(name,'SEE_ALSO',ignore=ignore);
  endfunction
  
  function node = collect_authors(name,ignore=%t)
    node = collect_tag(name,'AUTHORS',ignore=ignore);
  endfunction
  
  name=block_name;
    
  D = gdate_create();
  date = D.strftime["%d %B %Y"]; // man strftime to see all the format features 
  
  //tt_head=['<?xml version='"1.0'" encoding='"ISO-8859-1'" standalone='"no'"?>'
  //	   '<!DOCTYPE MAN SYSTEM '"'+SCI+'/man/manrev.dtd'">'];
  
  nodes = list();
  nodes.add_last[gmarkup_node_create(name="TITLE",children=list(name))];
  nodes.add_last[gmarkup_node_create(name="TYPE",children=list('Scicos Block'))];
  nodes.add_last[collect_short_description(name)];
  nodes.add_last[collect_calling_sequence(name)];
  nodes.add_last[collect_parameters(name)];
  nodes.add_last[collect_description(name)];
  nodes.add_last[collect_properties(name)];
  nodes.add_last[collect_used_function(name)];
  nodes.add_last[collect_see_also(name)];
  nodes.add_last[collect_authors(name)];
    
  attributes = hash(langage="english",date=date);
  node =  gmarkup_node_create(name="MAN",children=nodes,attributes=attributes);
  
endfunction
