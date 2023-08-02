s=poly(0,'s');

Kp=0.0006145;
Go=Kp/s;
Go=syslin('c',Go);
Go=tf2ss(Go);
Td=389.5; // assume aribitrary dead time

dlay=syslin('c',(1-Td*s/2)/(1+Td*s/2)) ; // Pade approximation
dlay=tf2ss(dlay);
G=Go*dlay;

ALV=5; // in percent
duMax=20.0; // max disturbance
lambdALV=ALV/(Kp*duMax*0.5);
lambd=lambdALV;
//lambd=800

Ko=0.9*(1+1/(600*s));
Ko=tf2ss(syslin('c',Ko));
Tr=2*lambd+Td;
Kc=Tr/(Kp*(lambd+Td)^2);

tauc=2.0*Td;
Trs=4*(tauc+Td);
Kcs=1/(Kp*(tauc+Td));

Kn=tf2ss(syslin('c',Kc*(1+1/(Tr*s))));
Kns=tf2ss(syslin('c',Kcs*(1+1/(Trs*s))));

[So,Ro,To]=sensi(G,Ko);
[Sn,Rn,Tn]=sensi(G,Kn);
[Ss,Rs,Ts]=sensi(G,Kns);

//// Analysis Plots
t=0:1:20*lambd;

yo=csim('step',t,To);
yn=csim('step',t,Tn);
ys=csim('step',t,Ts);

yo_uD=csim('step',t,(-Go*So));
yn_uD=csim('step',t,(-Go*Sn));
ys_uD=csim('step',t,(-Go*Ss));

yo_yD=csim('step',t,So);

yn_yD=csim('step',t,Sn);
ys_yD=csim('step',t,Ss);

yo_R=csim('step',t,Ro);
yn_R=csim('step',t,Rn);
ys_R=csim('step',t,Rs);

x=logspace(-4,0,101);
y1=svplot(syslin('c',sysdiag(So,Sn,Ss)),x);

xset('window',1);
xbasc();
subplot(1,2,1)
if exists('%nsp') then 
  plot2d(x',20*log(y1')/log(10),logflag='ln',line_thickness=[2 2 2],leg='Old@Lambda@Skoge');
  title('PV Dist Response')
  xgrid();
else
  plot2d(x',20*log(y1')/log(10),logflag='ln');//,line_thickness=[2 2 2],leg='Old@Lambda@Skoge');
end
subplot(1,2,2)
pause xxx
gainplot([So;Sn;Ss],0.00001,1)

y2=svplot(syslin('c',sysdiag(-Go*So,-Go*Sn,-Go*Ss)),x);

xset('window',2);
subplot(1,2,1)
if exists('%nsp') then 
  plot2d(x',20*log(y2')/log(10),logflag='ln',line_thickness=[2 2 2],leg='Old@Lambda@Skoge');
  title('Load Dist Response')
  xgrid()
else
  plot2d(x',20*log(y2')/log(10),logflag='ln');
end
subplot(1,2,2)
gainplot([-Go*So;-Go*Sn;-Go*Ss],0.00001,1)
//legend('Old','Lambda','Skoge')

pause 
fpeako=freson(ss2tf(So))
fpeakn=freson(ss2tf(Sn))
fpeaks=freson(ss2tf(Ss))

to=1/fpeako
tn=1/fpeakn
ts=1/fpeaks

print(to)
print(tn)
print(ts)

//disp(tn,'Lambda resonant period (s)')
//disp(ts,'Skoge resonant period (s)')

