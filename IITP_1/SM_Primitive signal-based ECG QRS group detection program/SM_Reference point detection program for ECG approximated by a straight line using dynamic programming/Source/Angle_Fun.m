%% Angle
function [out_x, out_y] = Angle_Fun(in_x, in_y, Win_H, Weight_x)
% 제2코사인법칙
% a^2 = b^2 + c^2 -2*b*cos(A)
out_x=in_x;
Len=length(in_y);
out_y(1,1:Len)=180;
in_x=in_x*Weight_x;
% in_y=360*(in_y-min(in_y))/(max(in_y)-min(in_y));
x1_2 = (in_x(1+Win_H*2:Len)-in_x(1:Len-Win_H*2)).^2;
y1_2 = (in_y(1+Win_H*2:Len)-in_y(1:Len-Win_H*2)).^2;
a_L_2 = x1_2 + y1_2;
x2_2 = (in_x(1+Win_H:Len-Win_H)-in_x(1:Len-Win_H*2)).^2;
y2_2 = (in_y(1+Win_H:Len-Win_H)-in_y(1:Len-Win_H*2)).^2;
b_L_2 = x2_2 + y2_2;
b_L_1 = sqrt(b_L_2);
x3_2 = (in_x(1+Win_H*2:Len)-in_x(1+Win_H:Len-Win_H)).^2;
y3_2 = (in_y(1+Win_H*2:Len)-in_y(1+Win_H:Len-Win_H)).^2;
c_L_2 = x3_2 + y3_2;
c_L_1 = sqrt(c_L_2);
Ang_Val= (b_L_2 + c_L_2 - a_L_2)./(2*(b_L_1.*c_L_1));
Ang_data = real(acos(Ang_Val(1:end))*(180/pi));
out_y(1+Win_H:Len-Win_H)=Ang_data;

out_y=180-out_y;

for i = Win_H+1 : Len-Win_H
    if in_y(i) <= in_y(i-Win_H) && in_y(i) <= in_y(i+Win_H)
        out_y(i) = -out_y(i);
    elseif in_y(i+Win_H) - in_y(i)  > in_y(i) - in_y(i-Win_H) && in_y(i) - in_y(i-Win_H) >= 0
        out_y(i) = -out_y(i);
    elseif in_y(i+Win_H) - in_y(i)  > in_y(i) - in_y(i-Win_H) && in_y(i) - in_y(i-Win_H) <= 0
        out_y(i) = -out_y(i);
    end
end