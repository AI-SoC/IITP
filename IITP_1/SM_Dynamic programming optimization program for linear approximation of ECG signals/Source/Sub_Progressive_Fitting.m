% function [X1,Y1,X2,Y2] = Sub_Progressive_Fitting(Sig,Fs,d_max)
% L = length(Sig);
% X1(1) = 1;
% Y1(1) = Sig(X1(1));
% 
% k = 1;
% while k < L-1
%     for k = X1(end)+1 : L
%         if max(abs(FUN_Point_to_Line_Dist(X1(end),k,X1(end)+1:k-1,Y1(end),Sig(k),Sig(X1(end)+1:k-1),Fs))) > d_max
%             X1 = [X1, k-1];
%             Y1 = [Y1, Sig(k-1)];
%             break
%         end
%     end
% end
% 
% X2 = [X1(2:end), L];
% Y2 = Sig(X2);



function Pos = Sub_Progressive_Fitting(X,Y,Fs,d_max)
L = length(X);

k = 1;
EP = 1;
Temp = zeros(1,L,'logical');

t_max = 32;

while k < L-1
    for k = EP+1 : L
        % 점과 직선사이의 거리가 임계값을 넘으면 정점 추가
        if (max(abs(FUN_Point_to_Line_Dist(X(EP),X(k),X(EP+1:k-1),Y(EP),Y(k),Y(EP+1:k-1),Fs))) > d_max) | (k-EP > t_max)
%         if max(abs(FUN_Point_to_Line_Dist(X(EP),X(k),X(EP+1:k-1),Y(EP),Y(k),Y(EP+1:k-1),Fs))) > d_max
            Temp(k-1) = 1;
            EP = k-1;
            break
        end
    end
end
Pos = [1,find(Temp),L];