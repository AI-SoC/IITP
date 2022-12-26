function Cur = Curvature_Cal(Sig, Cur_size)
L = length(Sig);
dy = zeros(1,L);
dy2 = zeros(1,L);
for i = Cur_size+1 : L-Cur_size
    dy(i) = (Sig(i+Cur_size) - Sig(i-Cur_size))/2;
end
for i = Cur_size*2+1 : L-Cur_size*2
    dy2(i) = (dy(i+Cur_size) - dy(i-Cur_size))/2;
end
% dy = [Sig(Cur_size:end)-Sig(1:end-(Cur_size-1)),zeros(1,Cur_size-1)];
% dy2 = [zeros(1,Cur_size-1),dy(Cur_size:end)-dy(1:end-(Cur_size-1))];
Cur = dy2./((1+dy.^2).^(3/2));