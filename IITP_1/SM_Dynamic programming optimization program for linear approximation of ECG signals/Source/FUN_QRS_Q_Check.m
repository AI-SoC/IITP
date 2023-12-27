function [Err,Err_Type] = FUN_QRS_Q_Check(Y,Q_on,Q_peak,N)
% Q_peak�� �ִ��� üũ
Err = 0;
Err_Type = 0;
if isnan(Q_peak)
    Q_exist = 0;
else
    Q_exist = 1;
end

% QRS ���� üũ

% ���� R-peak ���� üũ
if Y(1) == max(Y(1:min(length(Y), 2)))
    Type_L = 1;
else
    Type_L = -1;
end
% ���� R-peak ���� üũ
if Y(end) == max(Y(max(2,length(Y)-2):end))
    Type_R = 1;
else
    Type_R = -1;
end

% Q-peak ���� üũ
if Q_exist == 1
    for i= Q_peak + 1 : length(Y) - 1
        if Type_R*(Y(i) - Y(i-1)) <= 0 && Type_R*(Y(i) - Y(i+1)) <= 0
            if abs( max(Type_R*Y(Q_peak : i)) - Y(i) ) >= abs( max(Type_R*Y(Q_peak : i)) - Y(Q_peak) )/2
                Err = 1;
                Err_Type = 1;
            end
        end
    end
end

% Q-on ���� üũ
% Q-peak�� ���� ���
if Q_exist == 1
    for i= Q_on + 1 : Q_peak - 1
        if Type_R*(Y(i) - Y(i-1)) <= 0 && Type_R*(Y(i) - Y(i+1)) <= 0
            if abs( Y(Q_on) - Y(i) ) >= abs(Y(Q_on) - Y(Q_peak))/2
                Err = 1;
                Err_Type = 2;
            end
        end
    end
% Q-peak�� ���� ���
elseif Q_exist == 0
    for i= Q_on + 1 : length(Y) - 1
        if Type_R*(Y(i) - Y(i-1)) <= 0 && Type_R*(Y(i) - Y(i+1)) <= 0
            if abs( max(Type_R*Y(Q_on : i)) - Y(i) ) >= abs( max(Type_R*Y(Q_on : i)) - Y(Q_on) )/2
                Err = 1;
                Err_Type = 3;
            end
        end
    end
end
