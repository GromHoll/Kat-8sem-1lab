x=0:1:999;
y=0:1:44;

k=30;

M=moviein(k);
n=1;

load ksi.txt;

xmin=300;
xmax=540;
for r=1:30
    A = ksi(n:n+44,:);
    contourf(x,y,A,30);
    
    set(gca,'XLim',[xmin xmax],'YLim',[0 44])
    xlabel('x');
    ylabel('y');
    
    M(:,r)=getframe;
    n=n+45;
end
 
 repeat=0;
 fsp=10;
 
 movie(M,repeat,fsp); 
