function [pathname, filename] = File_Load(type)
filename = 0;
if type == 1
    if exist('pathname_Signal.mat','file')
        pathname = importdata('pathname_Signal.mat');
    else
        pathname = pwd;
    end
    [filename, pathname] =  uigetfile('*.mat',{},pathname);
    if pathname ~= 0
        save('pathname_Signal.mat','pathname')
    end
elseif type == 2
    if exist('pathname_R_peak.mat','file')
        pathname = importdata('pathname_R_peak.mat');
    else
        pathname = pwd;
    end
    [filename, pathname] =  uigetfile('*.mat',{},pathname);
    if pathname ~= 0
        save('pathname_R_peak.mat','pathname')
    end
elseif type == 3
    if exist('pathname_Primitive.mat','file')
        pathname = importdata('pathname_Primitive.mat');
    else
        pathname = pwd;
    end
    [filename, pathname] =  uigetfile('*.mat',{},pathname);
    if pathname ~= 0
        save('pathname_Primitive.mat','pathname')
    end
end