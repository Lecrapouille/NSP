// les points du losange

function [Pts,Tr]=triangles()
  n=60
  x1=linspace(0,1,n)';
  y1=1-x1;
  z1=ones(size(x1));
  
  // les points du cercle
  theta=linspace(0,2*%pi,n)';
  x2=sin(theta);
  y2=cos(theta);
  z2=2*ones(size(x2));
  
  P1=[x1,y1,z1];
  P1=[P1;x1,-y1,z1];
  P1=[P1;-x1,-y1,z1];
  P1=[P1;-x1,y1,z1];
  n1=size(P1,'r');
  P2=[x2,y2,z2];
  Pts=[P1;P2];
  Tr=[];
  for i=1:n1;
    for j=1:(size(x2,'*')-1)
      T=[i;size(P1,'r')+j;size(P1,'r')+j+1];
      Tr=[Tr,T];
    end
  end
endfunction

if %f then 
  [Pts,Tr]=triangles()
  P=polyhedron_create(Mcoord=Pts, Mface = Tr);
  nval = size(P.Mcoord(:,3)','*');
  val = 1:nval;

  ncol=nval;
  cmap= [bluecolormap(ncol);[0.92 0.92 0.92]];

  fmode = %t; 
  mode = "Cairo";
  mode = "Gtk";
  mode = "OpenGl";
  F=figure_create(wresize=%t,fname=mode,driver=mode,id=20);
  // a top level axes 
  A=objs3d_create(top=%t,title="Main title", colormap=cmap,box_style=0,box_color=33,alpha=56,theta=52);
  // ,wrect=[0,0,1,1],frect=[0,-2,6,2],arect=[1,1,1,1]/12);
  // to have iso mode 
  A.scale_flag=4;

  F.children(1)= A;

  color_low=1,color_high= ncol;
  Ps=spolyhedron_create(Mcoord=P.Mcoord,Mface=P.Mface,Mval = val,...
			vmin = min(val), vmax=max(val),...
			coloutmin= color_low, coloutmax= color_high,...
			colmin = color_low, colmax = color_high,...
			back_color=1, mesh=%f, shade=%t);
  
  A.children($+1) = Ps;
  F.connect[];
end


// levels

function [xl,yl]=draw_level(zval)
  n=100
  x1=linspace(0,1,n)';
  y1=1-x1;
  z1=ones(size(x1));
  // les points du cercle
  theta=linspace(0,2*%pi,n)';
  x2=sin(theta);
  y2=cos(theta);
  z2=2*ones(size(x2));

  P1=[x1,y1,z1];
  P1=[P1;x1,-y1,z1];
  P1=[P1;-x1,-y1,z1];
  P1=[P1;-x1,y1,z1];
  n1=size(P1,'r');
  P2=[x2,y2,z2];
  
  pts=[];

  for i=1:n1
    for j=1:(size(x2,'*')-1)
      p1=P1(i,:);
      p2=P2(j,:);
      lambda = (zval - p2(3))/(p1(3)-p2(3));
      pts = [pts; lambda*p1(1:2) + (1-lambda)*p2(1:2)];
    end
  end
  x=pts(:,1);y = pts(:,2);
  k = convhull (x, y);
  xl= x(k); yl= y(k);
  // plot2d(x(k),y(k),iso=%t,rect=[-1,-1,1,1]);
endfunction

if %f then 
  for z=linspace(1.01,1.98,10);
    F=get_current_figure[];
    if length(F.children) >= 1 then
      Axes=F(1);
      Axes.children=list();
    end
    [xl,yl]=draw_level(z);
    plot2d(xl,yl,iso=%t,rect=[-1,-1,1,1]);
    Axes=F(1);
    Axes(1).invalidate[];
    xpause(1000,%t);
  end
end

ncol=128;
cmap= [jetcolormap(ncol);[0.92 0.92 0.92]];

xinit(opengl=%t);

if %f then 
  for z=linspace(1.01,1.98,20);
    [xl,yl]=draw_level(z);
    [xx,yy,zz]=spaghetti(xl,yl,z*ones(size(xl)),0.03,6,head=%f);
    plot3d1(xx,yy,zz,iso=%t,box_style="none",colormap=cmap);
    // param3d(xl,yl,z*ones(size(xl)),alpha = 35, theta = 45,iso=%t,box_style="none");
    // xpause(1000,%t);
  end
end

if %t then 
  axx=[];ayy=[];azz=[];
  for z=linspace(1.01,1.98,20);
    [xl,yl]=draw_level(z);
    [xx,yy,zz]=spaghetti(xl,yl,z*ones(size(xl)),0.03,6,head=%f);
    axx=[axx,xx];
    ayy=[ayy,yy];
    azz=[azz,zz];
  end
  plot3d1(axx,ayy,azz,iso=%t,box_style="none",colormap=cmap);
end
