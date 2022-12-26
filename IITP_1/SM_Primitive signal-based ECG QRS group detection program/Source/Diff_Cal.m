%% mop_pair Function
function  Sig_Diff = Diff_Cal(Sig, h_len1)

Sig_Dil = mop_Dilation(Sig, h_len1);
Sig_Ero = mop_Erosion(Sig, h_len1);
Sig_Diff = Sig_Dil - Sig_Ero;
%% Dilation Function
function Sig_Morp = mop_Dilation(Sig, h_len)
m=length(Sig);
Sig_Morp(1,1:m)=0;
for k=1:m
   Sig_Morp(k)= max(Sig(max(1,k-h_len):min(k+h_len,m)));
end

%% Erosion Function
function Sig_Morp = mop_Erosion(Sig, h_len)
m=length(Sig);
Sig_Morp(1,1:m)=0;
for k=1:m
   Sig_Morp(k)= min(Sig(max(1,k-h_len):min(k+h_len,m)));
end