x=0:10:990;
y=0:3:42;

k=30;

M=moviein(k);

n=1;

load u.txt;
load v.txt;

xmin=300;
xmax=540;

for r=1:30
    A = u(n:n+14,:);
    B = v(n:n+14,:);
    quiver(x,y,A,B,3);
    set(gca,'XLim',[xmin xmax],'YLim',[0 44])
    xlabel('x');
    ylabel('y');
    M(:,r)=getframe;
    n=n+15;
end

repeat=0;
fsp=10;

movie(M,repeat,fsp); 
