function DB = Primitive_Separate(DB)
%% Primitive 극대점 위치 검출
[DB.Pr_Max_val,DB.Pr_Max_Pos] = max(abs(DB.Sig_Filt(DB.Pr_X)));
if DB.Pr(DB.Pr_Max_Pos) >= 0
    DB.Pr_Sign = 1;
else
    DB.Pr_Max_val = -DB.Pr_Max_val;
    DB.Pr_Sign = -1;
end

%% Primitive 분리
if DB.Pr_Sign == 1
    for i = DB.Pr_Max_Pos : -1 : 2
        if DB.Pr(i) <=DB.Pr(1)
            break
        end
    end
    DB.Pr1_X = DB.Pr_X(1:i+1);
    DB.Pr1 = DB.Pr(1:i+1);
    Pr2_Start = i+1;
    for i = DB.Pr_Max_Pos : 1 : length(DB.Pr)-1
        if DB.Pr(i) <= DB.Pr(end)
            break
        end
    end
    DB.Pr3_X = DB.Pr_X(i-1:end);
    DB.Pr3 = DB.Pr(i-1:end);
    Pr2_End = i-1;
    DB.Pr2_X = DB.Pr_X(Pr2_Start : Pr2_End);
    DB.Pr2 = DB.Pr(Pr2_Start : Pr2_End);
    DB.Pr2_Max_Pos = DB.Pr_Max_Pos - Pr2_Start+1;
else
    for i = DB.Pr_Max_Pos : -1 : 2
        if DB.Pr(i) >=DB.Pr(1)
            break
        end
    end
    DB.Pr1_X = DB.Pr_X(1:i+1);
    DB.Pr1 = DB.Pr(1:i+1);
    Pr2_Start = i+1;
    for i = DB.Pr_Max_Pos : 1 : length(DB.Pr)-1
        if DB.Pr(i) >= DB.Pr(end)
            break
        end
    end
    DB.Pr3_X = DB.Pr_X(i-1:end);
    DB.Pr3 = DB.Pr(i-1:end);
    Pr2_End = i-1;
    DB.Pr2_X = DB.Pr_X(Pr2_Start : Pr2_End);
    DB.Pr2 = DB.Pr(Pr2_Start : Pr2_End);    
    DB.Pr2_Max_Pos = DB.Pr_Max_Pos - Pr2_Start+1;
end


Primitive_Min_Size = 5;
if length(DB.Pr1) < Primitive_Min_Size
    DB.Pr1_exist = 0;
    DB.Pr2 = [DB.Pr1(1:end-1), DB.Pr2];
    DB.Pr2_X = [DB.Pr1_X(1:end-1), DB.Pr2_X];
    DB.Pr1 = [];
    DB.Pr1_X = [];
else
    DB.Pr1_exist = 1;
end


if length(DB.Pr3) < Primitive_Min_Size
    DB.Pr3_exist = 0;
    DB.Pr2 = [DB.Pr2, DB.Pr3(2:end)];
    DB.Pr2_X = [DB.Pr2_X, DB.Pr3_X(2:end)];
    DB.Pr3 = [];
    DB.Pr3_X = [];
else
    DB.Pr3_exist = 1;
end

    DB.Pr1_exist = 1;

    DB.Pr3_exist = 1;

