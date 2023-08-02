
S=getfile('demo_resources.txt');
txt=m2i([],'uint8');
for i=1:size(S,'*')
  execstr(sprintf('s=[%s];',strsubst(S(i),","," ")));
  txt.concatr[m2i(s,'uint8')];
end

txt=sprint(txt, as_read=%t, base64=%t);
putfile('demo_resources.sci',txt);

