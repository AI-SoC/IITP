%% Filtering
function [out_x, out_y] = Butterworth_Filter(in_x, in_y, Sampling_f, Type, Order_n, Cutoff_f)

%Order_n : 차수
%Cutoff_f : cutoff frequency
%Type : 0(LPF), 1(HPF), 2(BPF)

Cutoff= Cutoff_f / (Sampling_f /2);

if (Type == 0)
    [b,a] = butter(Order_n,Cutoff,'low');
    out_y = filtfilt(b,a,in_y);

elseif (Type == 1)
    [b,a] = butter(Order_n,Cutoff,'high');
    out_y = filtfilt(b,a,in_y);
    
elseif (Type == 2)
    [b,a] = butter(Order_n,Cutoff(2),'low');
    [d,c] = butter(Order_n,Cutoff(1),'high');
    lowpass_data = filtfilt(b,a,in_y);
    out_y = filtfilt(d,c,lowpass_data);
else
    error('타입이 잘못되었습니다');
    
end

out_x=in_x;


