tic;

x = 0:1:999;
y = 0:1:44;

k = 30;

M = moviein(k);
n = 1;

load psi.txt;

xmin = 350;
xmax = 540;

for r = 1:15
    A = psi(n:n+44,:);
    contourf(x,y,A,30);
    caxis([0 1]);
    colorbar();
        
    set(gca, 'XLim', [xmin xmax], 'YLim', [0 44])    
    xlabel('x');
    ylabel('y');

    M(:,r) = getframe;

    n = n + 45;
end

repeat = 0;
fps = 10;

movie(M,repeat,fps);

toc;
        
        
        