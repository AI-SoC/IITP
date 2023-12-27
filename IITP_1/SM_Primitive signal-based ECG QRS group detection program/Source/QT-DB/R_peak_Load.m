clc, clear all, close all

[filename_temp, pathname] =  uigetfile('*.mat',{},'multiselect','on');
if pathname == 0
    return
end
if iscell(filename_temp)
    filename = filename_temp;
else
    filename{1} = filename_temp;
end

Annotation_Key = ['NLRBAaJSVrFejnE/fQ?'];

for FN = 1 : length(filename)
    fid = fopen([pathname, filename{FN}(1:end-4),'_manual1.txt'],'r');
    fgetl(fid);
    temp = 'start';
    i = 1;
    type = [];
    peak_temp = [];
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
    DB.R_peak = [];
    DB.Q_on = [];
    DB.S_off = [];
    for i = 2 : length(peak_temp)-1
        if sum(type(i) == Annotation_Key)
            DB.R_peak(end+1) = peak_temp(i);
            if type(i-1) == '('
                DB.Q_on(end+1) = peak_temp(i-1);
            else
                DB.Q_on(end+1) = NaN;
            end
            
            if type(i+1) == ')'
                DB.S_off(end+1) = peak_temp(i+1);
            else
                DB.S_off(end+1) = NaN;
            end
        end
    end
    save([pathname,'\Rpeak\',filename{FN}(1:end-4),'_R_peak.mat'],'DB')
end
