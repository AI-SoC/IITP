function A = Quadratic_Fitting(Sig, Peak_Pos)
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
A = -Poly_Fit(:,1)';



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
A = -Poly_Fit(:,1)';
