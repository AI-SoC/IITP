function [Kurtosis,Kurtosis_Index] = Kurtosis_Cal(Sig,Kurtosis_Size)
Kur_Num = length(Kurtosis_Size);
Sig_Kur = zeros(Kur_Num,length(Sig));
for i = 1 : Kur_Num
    Sig_Kur(i,:) = Kurtosis_Fitting(Sig,Kurtosis_Size(i));
end
[Sig_Kur_Max,Pos_Max] = max(Sig_Kur,[],1);
[Sig_Kur_Min,Pos_Min] = min(Sig_Kur,[],1);
Sig_Positive_Index = abs(Sig_Kur_Max) > abs(Sig_Kur_Min);
Kurtosis = Sig_Kur_Max.*Sig_Positive_Index + Sig_Kur_Min.*~Sig_Positive_Index;
Kurtosis_Index = Kurtosis_Size(Pos_Max).*Sig_Positive_Index +  Kurtosis_Size(Pos_Min).*~Sig_Positive_Index;

function A = Kurtosis_Fitting(Sig, Cut_Size)
Pr_X= 1:2*Cut_Size+1;

Diff_Temp = Pr_X-(Cut_Size+1);
Quad_Temp = Diff_Temp.^2;
Sum_Temp = sum(Quad_Temp);
Sum_Temp2 = sum(Diff_Temp.^4);

Poly_Fit = zeros(length(Sig),3);
Sig_Cut = zeros(length(Sig),2*Cut_Size+1);
for i = 1 : 2*Cut_Size+1
    Sig_Cut(Cut_Size+1:end-Cut_Size,i) = Sig(i:end-2*Cut_Size-1+i);
end
Poly_Fit(:,1) = Sig_Cut*(Quad_Temp');
Poly_Fit(:,1) = (Poly_Fit(:,1)-Sig_Cut(:,Cut_Size+1)*Sum_Temp)/Sum_Temp2;
A = Poly_Fit(:,1)';
% Ind = -Poly_Fit(:,1).*Sig';