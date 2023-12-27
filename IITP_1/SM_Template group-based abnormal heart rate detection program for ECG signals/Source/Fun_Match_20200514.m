function [TP,TN,FP,FN,Type] = Fun_Match_20200514(t,r,M_T)
N = length(t);
TP = 0;
TN = 0;
FP = 0;
FN = 0;
Type = zeros(N,1);
for i = 1 : N
    if t(i) == M_T
        if r(i) == 1
            TP = TP+1;
            Type(i) = 1;
        else
            FN = FN+1;
            Type(i) = 4;
        end
%     else
%     elseif t(i) == 'V' || t(i) == 'A' || t(i) == 'a' || t(i) == 'F'
%     elseif t(i) == 'V' || t(i) == 'A' || t(i) == 'F' || t(i) == 'J'
%     elseif t(i) == 'V' || t(i) == 'A'
%     elseif t(i) == 'V' || t(i) == 'J' || t(i) == 'a'
    elseif t(i) == 'V' || t(i) == 'J' 
%     elseif t(i) == 'J'
%     elseif t(i) == 'V'
%     else
        if r(i) == 1
            FP = FP+1;
            Type(i) = 3;
%             if t(i) == 'F'
%                 Type(i) = 5;
%             end
        else
            TN = TN+1;
            Type(i) = 2;
        end
    end
end