function Result = RR_Ratio_Create(X,FS)
Ratio = abs((1:X)-X/2);
Result = (-Ratio + max(Ratio))/(max(Ratio)-min(Ratio));
Result(1:round(FS*0.2)) = 0;
Result(end-round(FS*0.2):end) = 0;

% function Result = RR_Ratio_Create(X,N,FS)
% Center = (1:N)*X/(N+1);
% Ratio = zeros(N,X);
% for i = 1 : N
%     Ratio(i,:) = abs((1:X)-Center(i));
% end
% Result_Temp = min(Ratio,[],1);
% Result = (-Result_Temp + max(Result_Temp))/(max(Result_Temp)-min(Result_Temp));
% Result = ones(1,X);
% Result(1:round(FS*0.2)) = 0;
% Result(end-round(FS*0.2):end) = 0;