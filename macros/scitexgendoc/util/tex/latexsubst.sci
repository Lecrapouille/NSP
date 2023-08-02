//-*-   Encoding: utf-8  -*- 
function fields=latexsubst(fields)
// The xml part should not be treated here 
  fields=strsubst(fields,'<P>',' ')
  fields=strsubst(fields,'</P>',' \\')
  fields=strsubst(fields,'<VERB>','{\bf ')
  fields=strsubst(fields,'</VERB>','}')
  fields=strsubst(fields,'<LINK>','\textbf{')
  fields=strsubst(fields,'</LINK>','}')
  fields=strsubst(fields,'<VERBATIM>','\begin{verbatim}')
  fields=strsubst(fields,'</VERBATIM>','\end{verbatim}')
  fields=strsubst(fields,'<![CDATA[','')
  fields=strsubst(fields,']]>','')
  
  fields=strsubst(fields,'€','E');// XXX
  // fields=strsubst(fields,'','\""o')
  fields=strsubst(fields,'_','\_')
  fields=strsubst(fields,'%','\%')
  fields=strsubst(fields,'&','\&')
  fields=strsubst(fields,'^','\textasciicircum')
  fields=strsubst(fields,'$','\$')
  fields=strsubst(fields,'<','$<$')
  fields=strsubst(fields,'>','$>$')

  fields=strsubst(fields,'\&gt;','>')
  fields=strsubst(fields,'\&lt;','<')

endfunction
