function Pos = Sub_Curvature_Fitting(Angle_x,Angle_y,LE_Type)

% [~, ~, LE_Binary_x] = Local_Extrema_Fun(Angle_x, Angle_y, LE_Type);
LE_Binary_x = zeros(1,length(Angle_y));
Diff = Angle_y(2:end)-Angle_y(1:end-1);
LE_Binary_x(2:end-1) = Diff(2:end).*Diff(1:end-1) < 0;

%% Thresholding
Th_in_x=Angle_x;
Th_in_y=Angle_y.*LE_Binary_x;
Th_Val=0.3*180;
sort_num = 10;

[~,Pos] = find(abs(Th_in_y) >= Th_Val);
if length(Pos) > sort_num
    [~,p] = sort(abs(Th_in_y(Pos)),'descend');
    Pos = Pos(p(1:sort_num));
end
Pos = sort(Pos,'ascend');
Pos = [Angle_x(1),Th_in_x(Pos),Angle_x(end)];

