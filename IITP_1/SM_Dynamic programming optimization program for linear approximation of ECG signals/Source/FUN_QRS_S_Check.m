function [Err,Err_Type] = FUN_QRS_S_Check(Y,S_off,S_peak,N)
% S_peak가 있는지 체크
Err = 0;
Err_Type = 0;
if isnan(S_peak)
    S_exist = 0;
else
    S_exist = 1;
end

% QRS 방향 체크

% 좌측 R-peak 파형 체크
if Y(1) == max(Y(1:min(length(Y), 2)))
    Type_L = 1;
else
    Type_L = -1;
end
% 우측 R-peak 파형 체크
if Y(end) == max(Y(max(2,length(Y)-2):end))
    Type_R = 1;
else
    Type_R = -1;
end

% S-peak 정상 체크
if S_exist == 1
    for i= S_peak - 1 : -1 : 2
        if Type_L*(Y(i) - Y(i-1)) <= 0 && Type_L*(Y(i) - Y(i+1)) <= 0
            if abs( max(Type_L*Y(i : S_peak)) - Y(i) ) >= abs( max(Type_L*Y(i : S_peak)) - Y(S_peak) )/2
                Err = 1;
                Err_Type = 1;
            end
        end
    end
end

% S-off 정상 체크
% s-peak가 있을 경우
if S_exist == 1
    for i= S_peak + 1 : S_off - 1
        if Type_L*(Y(i) - Y(i-1)) <= 0 && Type_L*(Y(i) - Y(i+1)) <= 0
            if abs( Y(S_off) - Y(i) ) >= abs(Y(S_off) - Y(S_peak))/2
                Err = 1;
                Err_Type = 2;
            end
        end
    end
% S-peak가 없을 경우
elseif S_exist == 0
    for i= S_off - 1 : -1 : 2
        if Type_L*(Y(i) - Y(i-1)) <= 0 && Type_L*(Y(i) - Y(i+1)) <= 0
            if abs( max(Type_L*Y(i : S_off)) - Y(i) ) >= abs( max(Type_L*Y(i : S_iff)) - Y(S_off) )/2
                Err = 1;
                Err_Type = 3;
            end
        end
    end
end
