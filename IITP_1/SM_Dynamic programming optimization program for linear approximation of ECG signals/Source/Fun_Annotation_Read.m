function DB = Fun_Annotation_Read(pathname,filename)

%% 초기화
DB.P_on = [];
DB.P_peak = [];
DB.P_off = [];
DB.Q_on = [];
DB.R_peak = [];
DB.S_off = [];
DB.T_on = [];
DB.T_peak = [];
DB.T_off = [];
peak_temp = [];
type = [];
i = 1;
Annotation_Key = ['NLRBAaJSVrFejnE/fQ?'];
%% txt 파일 오픈
if exist([pathname, filename(1:end-4),'_manual1.txt'],'file')
    fid = fopen([pathname, filename(1:end-4),'_manual1.txt'],'r');
    fgetl(fid);
    temp = 'start';
    while 1
        temp = fscanf(fid,'%s',1);
        if isempty(temp)
            break
        end
        peak_temp(i) = fscanf(fid,'%d',1);
        type(i) = fscanf(fid,'%s',1);
        fgetl(fid);
%         temp = fscanf(fid,'%d',3);
        i = i+1;
    end
    fclose(fid);
end
%% DB 추출

% QRS
R_pos = [];
for i = 1 : length(type)
    if sum(type(i) == Annotation_Key)
        DB.R_peak = [DB.R_peak, peak_temp(i)];
        R_pos = [R_pos, i];
        if type(i-1) == '('
            DB.Q_on = [DB.Q_on, peak_temp(i-1)];
        else
            DB.Q_on = [DB.Q_on, NaN];
        end
        if type(i+1) == ')'
            DB.S_off = [DB.S_off, peak_temp(i+1)];
        else
            DB.S_off = [DB.S_off, NaN];
        end
    end
end

% P파
for i = 1 : length(R_pos)
    P_exist = 0;
    if i == 1
        i_temp = 0;
    else
        i_temp = R_pos(i-1);
    end
    for j = i_temp+1 : R_pos(i)-1
        if type(j) == 'p'
            DB.P_peak = [DB.P_peak, peak_temp(j)];
            P_exist = 1;
            break
        end
    end
    if P_exist == 1
        if type(j-1) == '('
            DB.P_on = [DB.P_on, peak_temp(j-1)];
        else
            DB.P_on = [DB.P_on, NaN];
        end
        if type(j+1) == ')'
            DB.P_off = [DB.P_off, peak_temp(j+1)];
        else
            DB.P_off = [DB.P_off, NaN];
        end
    else
        DB.P_peak = [DB.P_peak, NaN];
        DB.P_on = [DB.P_on, NaN];
        DB.P_off = [DB.P_off, NaN];
    end
end

% T파
for i = 1 : length(R_pos)
    T_exist = 0;
    if i == length(R_pos)
        i_temp = length(type)+1;
    else
        i_temp = R_pos(i+1);
    end
    for j = R_pos(i)+1 : i_temp-1
        if type(j) == 't'
            DB.T_peak = [DB.T_peak, peak_temp(j)];
            T_exist = 1;
            break
        end
    end
    if T_exist == 1
        if type(j-1) == '('
            DB.T_on = [DB.T_on, peak_temp(j-1)];
        else
            DB.T_on = [DB.T_on, NaN];
        end
        if type(j+1) == ')'
            DB.T_off = [DB.T_off, peak_temp(j+1)];
        else
            DB.T_off = [DB.T_off, NaN];
        end
    else
        DB.T_peak = [DB.T_peak, NaN];
        DB.T_on = [DB.T_on, NaN];
        DB.T_off = [DB.T_off, NaN];
    end
end
% for i = 5 : length(type)-4
%     if sum(type(i) == Annotation_Key)
%         % QRS
%         DB.R_peak = [DB.R_peak, peak_temp(i)];
%         if type(i-1) == '('
%             DB.Q_on = [DB.Q_on, peak_temp(i-1)];
%         else
%             DB.Q_on = [DB.Q_on, NaN];
%         end
%         if type(i+1) == ')'
%             DB.S_off = [DB.S_off, peak_temp(i+1)];
%         else
%             DB.S_off = [DB.S_off, NaN];
%         end
%         % P파
%         if type(i-2) == 'p'
%             DB.P_peak = [DB.P_peak, peak_temp(i-2)];
%             if type(i-3) == '('
%                 DB.P_on = [DB.P_on, peak_temp(i-3)];
%             else
%                 DB.P_on = [DB.P_on,NaN];
%             end
%             if type(i-1) == ')'
%                 DB.P_off = [DB.P_off, peak_temp(i-1)];
%                 DB.Q_on(end) = NaN;
%             else
%                 DB.P_off = [DB.P_off, NaN];
%             end
%         elseif type(i-3) == 'p'
%             DB.P_peak = [DB.P_peak, peak_temp(i-3)];
%             if type(i-4) == '('
%                 DB.P_on = [DB.P_on, peak_temp(i-4)];
%             else
%                 DB.P_on = [DB.P_on,NaN];
%             end
%             if type(i-2) == ')'
%                 DB.P_off = [DB.P_off, peak_temp(i-2)];
%             else
%                 DB.P_off = [DB.P_off, NaN];
%             end
%         else
%             DB.P_peak = [DB.P_peak, NaN];
%             DB.P_on = [DB.P_on, NaN];
%             DB.P_off = [DB.P_off, NaN];
%         end
%         
%         % T파
%         if type(i+2) == 't'
%             DB.T_peak = [DB.T_peak, peak_temp(i+2)];
%             if type(i+1) == '('
%                 DB.T_on = [DB.T_on, peak_temp(i+1)];
%                 DB.S_off(end) = NaN;
%             else
%                 DB.T_on = [DB.T_on, NaN];
%             end
%             if type(i+3) == ')'
%                 DB.T_off = [DB.T_off, peak_temp(i+3)];
%             else
%                 DB.T_off = [DB.T_off, NaN];
%             end
%         elseif type(i+3) == 't'
%             DB.T_peak = [DB.T_peak, peak_temp(i+3)];
%             if type(i+2) == '('
%                 DB.T_on = [DB.T_on, peak_temp(i+2)];
%             else
%                 DB.T_on = [DB.T_on, NaN];
%             end
%             if type(i+4) == ')'
%                 DB.T_off = [DB.T_off, peak_temp(i+4)];
%             else
%                 DB.T_off = [DB.T_off, NaN];
%             end
%         else
%             DB.T_peak = [DB.T_peak, NaN];
%             DB.T_on = [DB.T_on, NaN];
%             DB.T_off = [DB.T_off, NaN];
%         end
%     end
% end
