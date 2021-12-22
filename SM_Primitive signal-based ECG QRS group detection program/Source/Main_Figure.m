function varargout = Main_Figure(varargin)
% MAIN_FIGURE MATLAB code for Main_Figure.fig
%      MAIN_FIGURE, by itself, creates a new MAIN_FIGURE or raises the existing
%      singleton*.
%
%      H = MAIN_FIGURE returns the handle to a new MAIN_FIGURE or the handle to
%      the existing singleton*.
%
%      MAIN_FIGURE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAIN_FIGURE.M with the given input arguments.
%
%      MAIN_FIGURE('Property','Value',...) creates a new MAIN_FIGURE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Main_Figure_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Main_Figure_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Main_Figure

% Last Modified by GUIDE v2.5 20-Apr-2016 21:03:02

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Main_Figure_OpeningFcn, ...
                   'gui_OutputFcn',  @Main_Figure_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

% --- Executes just before Main_Figure is made visible.
function Main_Figure_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Main_Figure (see VARARGIN)
clc

% Choose default command line output for Main_Figure
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Main_Figure wait for user response (see UIRESUME)
% uiwait(handles.figure1);
linkaxes([handles.axes1,handles.axes2],'x')

% --- Outputs from this function are returned to the command line.
function varargout = Main_Figure_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

% --- Executes on button press in Signal_Load.
function Signal_Load_Callback(hObject, eventdata, handles)
% hObject    handle to Signal_Load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB

% [Sig_pathname_temp, Sig_filename_temp] = File_Load(1);
% if Sig_filename_temp == 0
%     return
% end

if exist('pathname.mat','file')
    pathname = importdata('pathname.mat');
else
    pathname = pwd;
end
[filename_temp, pathname] =  uigetfile('*.mat','Multiselect','on',{},pathname);
if pathname == 0
    return
else
    save('pathname.mat','pathname')
end
if ~iscell(filename_temp)
    filename{1} = filename_temp;
else
    filename = filename_temp;
end
file_num = length(filename);


for FN = 1 : file_num
    
    DB = [];
%     DB.Sig_pathname = Sig_pathname_temp;
%     DB.Sig_filename = Sig_filename_temp;
    DB.Sig_pathname = pathname;
    DB.Sig_filename = filename{FN};
    
    
    DB.base = 0;
    DB.gain = 200;
    DB.fs = 250;
    
    import_temp = (importdata([DB.Sig_pathname, DB.Sig_filename])-DB.base)/DB.gain;
    DB.Sig_Ori = import_temp;
    clear import_temp
    DB.Sig = DB.Sig_Ori(1,:);
    DB.Sig_Length = length(DB.Sig);
    % Morp_size = 12;
    % [DB.Sig_Filt,DB.Sig_Filt_Base] = Morphology(DB.Sig,Morp_size,Morp_size*2);
    DB.Sig_Filt = Butterworth(DB.Sig,DB.fs);
    DB.Sig_Filt_Base = DB.Sig - DB.Sig_Filt;
    
    % Kurtosis_Size = 3:11;
    % [DB.Sig_Kur,DB.Sig_Kur_Size] = Kurtosis_Cal(DB.Sig,Kurtosis_Size);
    Kurtosis_Size = 5;
    [DB.Sig_Kur,DB.Sig_Kur_Size] = Kurtosis_Cal(DB.Sig_Filt,Kurtosis_Size);
    DB.Sig_Ind = -DB.Sig_Filt.*DB.Sig_Kur;
    Smooth_Size = round(str2double(get(handles.Period_Thr,'string'))*DB.fs);
    DB.Sig_Ind_Smooth = smooth(DB.Sig_Ind,Smooth_Size,'moving')';
    DB.Sig_Ind_Smooth_temp = DB.Sig_Ind_Smooth;
    % DB.Sig_Ind_Smooth = zeros(1,DB.Sig_Length);
    % for k=1:DB.Sig_Length
    %    DB.Sig_Ind_Smooth(k)= max(DB.Sig_Ind_Smooth_temp(max(1,k-Smooth_Size):min(k+Smooth_Size,DB.Sig_Length)));
    % end
    % DB.Sig_Ind_Smooth = Morphology(DB.Sig_Ind_Smooth,Smooth_Size,Smooth_Size*2);
    % Cur_Size = 5;
    % DB.Sig_Cur = Curvature_Cal(DB.Sig,Cur_Size);
    
    
    
    axes(handles.axes1)
    cla
    plot(DB.Sig)
    hold on
    % plot(DB.Sig_Filt,'r')
    hold off
    axes(handles.axes2)
    cla
    plot(DB.Sig_Ind)
    hold on
    plot(DB.Sig_Ind_Smooth,'r')
    line([0,length(DB.Sig)],[0,0],'color',[0,0,0])
    hold off
    
    axes(handles.axes3)
    cla
    
    set(handles.Signal_text,'string',DB.Sig_filename)
    set(handles.Result,'string','Result')
    set(handles.False_Num,'string','0/0')
    
    
    DB.R_peak_pathname = [DB.Sig_pathname,'Rpeak\'];
    DB.R_peak_filename = [DB.Sig_filename(1:end-4),'_R_peak.mat'];
    R_peak_Load_Callback(hObject, eventdata, handles)
%     Primitive_Reset_Callback(hObject, eventdata, handles)
%     Primitive_Save_Callback(hObject, eventdata, handles)
    filename{FN}
%     pause(1)
end

% --- Executes on button press in R_peak_Load.
function R_peak_Load_Callback(hObject, eventdata, handles)
% hObject    handle to R_peak_Load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
% [DB.R_peak_pathname, DB.R_peak_filename] = File_Load(2);
if DB.R_peak_filename == 0
    return
end
Annotation_Key = ['NLRBAaJSVrFejnE/fQ?'];

import_temp = importdata([DB.R_peak_pathname, DB.R_peak_filename]);
DB.R_peak = import_temp.R_peak;
DB.Q_on = import_temp.Q_on;
DB.S_off = import_temp.S_off;

% keyboard
clear import_temp

axes(handles.axes1)
hold on
plot(DB.R_peak,DB.Sig(DB.R_peak),'ro')
plot(DB.Q_on,DB.Sig(DB.Q_on),'r>')
plot(DB.S_off,DB.Sig(DB.S_off),'r<')
hold off
axes(handles.axes2)
hold on
plot(DB.R_peak,DB.Sig_Ind(DB.R_peak),'ro')
hold off

set(handles.R_peak_text,'string',DB.R_peak_filename)

    First_Normal = DB.R_peak(1);

X = [max(1,First_Normal-100), min(length(DB.Sig),First_Normal+100)];
set(handles.Primitive_Start,'string',num2str(floor(X(1)/100)*100+1));
set(handles.Primitive_Length,'string',num2str(ceil((X(2)-floor(X(1)/100)*100+1)/100)*100));

axes(handles.axes3)
Pr_Show_Start = str2double(get(handles.Primitive_Start,'string'));
Pr_Show_Size = str2double(get(handles.Primitive_Length,'string'));
cla
plot(max(1,Pr_Show_Start):min(length(DB.Sig),Pr_Show_Start+Pr_Show_Size),DB.Sig(max(1,Pr_Show_Start):min(length(DB.Sig),Pr_Show_Start+Pr_Show_Size)))

% --- Executes on button press in Primitive_Load.
function Primitive_Load_Callback(hObject, eventdata, handles)
% hObject    handle to Primitive_Load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
[DB.Primitive_pathname, DB.Primitive_filename] = File_Load(3);
if DB.Primitive_filename == 0
    return
end
import_temp = importdata([DB.Primitive_pathname, DB.Primitive_filename]);
DB.Pr_X = import_temp.Pr_X;
% DB.Pr = import_temp.Pr;
DB.Pr = DB.Sig(DB.Pr_X);

DB = Primitive_Separate(DB);
% DB.Pr_Max = max(DB.Sig_Ind_Smooth(DB.Pr_X));
DB.Pr_Max = max(DB.Sig_Ind(DB.Pr_X));

axes(handles.axes3)
cla
plot(max(1,DB.Pr_X(1)-100):min(length(DB.Sig),DB.Pr_X(end)+100), DB.Sig(max(1,DB.Pr_X(1)-100):min(length(DB.Sig),DB.Pr_X(end)+100)))
hold on
plot(DB.Pr1_X,DB.Pr1,'r','Linewidth',2)
plot(DB.Pr2_X,DB.Pr2,'k','Linewidth',2)
plot(DB.Pr3_X,DB.Pr3,'r','Linewidth',2)
hold off

set(handles.Primitive_text,'string',DB.Primitive_filename)

set(handles.Primitive_Start,'string',num2str(max(1,DB.Pr_X(1)-100)))

set(handles.Primitive_Length,'string',num2str(min(length(DB.Sig),DB.Pr_X(end)+100) - max(1,DB.Pr_X(1)-100)))

% --- Executes on button press in Primitive_Reset.
function Primitive_Reset_Callback(hObject, eventdata, handles)
% hObject    handle to Primitive_Reset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
set(handles.Primitive_text,'string','Primitive')
axes(handles.axes3)
Pr_Show_Start = str2double(get(handles.Primitive_Start,'string'));
Pr_Show_Size = str2double(get(handles.Primitive_Length,'string'));
cla
plot(max(1,Pr_Show_Start):min(length(DB.Sig),Pr_Show_Start+Pr_Show_Size),DB.Sig(max(1,Pr_Show_Start):min(length(DB.Sig),Pr_Show_Start+Pr_Show_Size)))
pause(0.01)
Rect = getrect();
DB.Pr_X = round(Rect(1):Rect(1)+Rect(3));
% DB.Pr_X = DB.Q_on(2) : DB.S_off(2);
DB.Pr = DB.Sig(DB.Pr_X);

DB = Primitive_Separate(DB);

axes(handles.axes3)
cla
plot(max(1,DB.Pr_X(1)-100):min(length(DB.Sig),DB.Pr_X(end)+100), DB.Sig(max(1,DB.Pr_X(1)-100):min(length(DB.Sig),DB.Pr_X(end)+100)))
hold on
plot(DB.Pr1_X,DB.Pr1,'r','Linewidth',2)
plot(DB.Pr2_X,DB.Pr2,'k','Linewidth',2)
plot(DB.Pr3_X,DB.Pr3,'r','Linewidth',2)
hold off

% --- Executes on button press in Primitive_Save.
function Primitive_Save_Callback(hObject, eventdata, handles)
% hObject    handle to Primitive_Save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
% if exist('DB','var') == 1
%     if isfield(DB,'Primitive_pathname')
%         [DB.Primitive_filename, DB.Primitive_pathname] = uiputfile('*.mat',{},DB.Primitive_pathname,{});
%     elseif isfield(DB,'Sig_pathname')
%         [DB.Primitive_filename, DB.Primitive_pathname] = uiputfile('*.mat',{},DB.Sig_pathname);
%     else
%         [DB.Primitive_filename, DB.Primitive_pathname] = uiputfile('*.mat',{},pwd);
%     end
% else
%     [DB.Primitive_filename, DB.Primitive_pathname] = uiputfile('*.mat',{},pwd);
% end
% if DB.Primitive_pathname == 0
%     return
% end
DB.Primitive_filename = [DB.Sig_filename(1:end-4),'_Pr.mat'];
DB.Primitive_pathname = [DB.Sig_pathname,'Pr\'];
Data.Pr_X = DB.Pr_X;
Data.Pr = DB.Pr;

save([DB.Primitive_pathname,DB.Primitive_filename],'Data')
set(handles.Primitive_text,'string',DB.Primitive_filename)

% --- Executes on button press in Full_Signal.
function Full_Signal_Callback(hObject, eventdata, handles)
% hObject    handle to Full_Signal (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
axes(handles.axes1);
xlim([0,length(DB.Sig)])
DB.False_View = 0;
set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))

% --- Executes on button press in Previous_False.
function Previous_False_Callback(hObject, eventdata, handles)
% hObject    handle to Previous_False (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
if ~isempty(DB.F_Index)
axes(handles.axes1);
if DB.False_View < 2
    DB.False_View = DB.F_L;
else
    DB.False_View = DB.False_View - 1;
end
View_Size = 400;
xlim([DB.F_Index(DB.False_View) - View_Size , DB.F_Index(DB.False_View) + View_Size])
set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))

end
% --- Executes on button press in Next_False.
function Next_False_Callback(hObject, eventdata, handles)
% hObject    handle to Next_False (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
if ~isempty(DB.F_Index)
axes(handles.axes1);
    if DB.False_View >= DB.F_L
        DB.False_View = 1;
    else
        DB.False_View = DB.False_View + 1;
    end
    View_Size = 400;
    xlim([DB.F_Index(DB.False_View) - View_Size , DB.F_Index(DB.False_View) + View_Size])
    set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))
end

% --- Executes during object creation, after setting all properties.
function Primitive_Start_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Primitive_Start (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function Primitive_Length_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Primitive_Length (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --------------------------------------------------------------------
function Primitive_Interval_Callback(hObject, eventdata, handles)
% hObject    handle to Primitive_Interval (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
axes(handles.axes1)
X = xlim;
set(handles.Primitive_Start,'string',num2str(floor(X(1)/100)*100+1));
set(handles.Primitive_Length,'string',num2str(ceil((X(2)-floor(X(1)/100)*100+1)/100)*100));

% --------------------------------------------------------------------
function Debug_Callback(hObject, eventdata, handles)
% hObject    handle to Debug (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
keyboard

% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure
delete(hObject);

% --- Executes on button press in fp_view.
function fp_view_Callback(hObject, eventdata, handles)
% hObject    handle to fp_view (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of fp_view
global DB
DB.fp_view = get(handles.fp_view,'value');
if DB.fp_view == 1
    DB.F_Index = DB.F_Index2;
else
    DB.F_Index = DB.F_Index1;
end
DB.F_L = length(DB.F_Index);
DB.False_View = 0;

axes(handles.axes1);
xlim([0,length(DB.Sig)])
DB.False_View = 0;
set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))


% --------------------------------------------------------------------
function Primitive_R_peak_Callback(hObject, eventdata, handles)

global DB
clc
DB.fp_view = get(handles.fp_view,'value');

DB.Max_Size_Weight = (str2double(get(handles.Period_Thr,'string')));
DB.Pr_Basic_Weight = str2double(get(handles.Basic_Thr,'string'));
DB.Ind_Delete_Weight = str2double(get(handles.Ind_Delete_Weight,'string'));
DB.Ind_Thr_Weight = str2double(get(handles.Ind_Thr_Weight,'string')); 
DB.PVC_Weight = str2double(get(handles.PVC_Weight,'string'));

% DB = Primitive_QRS_20160130(DB);
% DB = Primitive_QRS_20160202(DB);
% DB = Primitive_QRS_20160211(DB);
DB = Primitive_QRS_20160212(DB);
% return
%% ¸ÅÄª·ü Ãâ·Â
set(handles.Result,'string',sprintf('%s Total:%5d\nTP:%5d FP:%5d FN:%5d \n',DB.Sig_filename,length(DB.R_peak), DB.tp, DB.fp, DB.fn))
set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))

%% Figure



axes(handles.axes1)
cla
plot(DB.Sig)
hold on
plot(DB.Sig_Filt,'r')
line([DB.R_peak-25;DB.R_peak+25],[DB.Sig(DB.R_peak);DB.Sig(DB.R_peak)],'color','r')
line([DB.R_peak_Detect;DB.R_peak_Detect],[DB.Sig(DB.R_peak_Detect)-1.5;DB.Sig(DB.R_peak_Detect)+1.5],'color','k')
plot(DB.QRS_on_Detect,DB.Sig(DB.QRS_on_Detect),'r>')
plot(DB.QRS_off_Detect,DB.Sig(DB.QRS_off_Detect),'r<')
hold off

axes(handles.axes2)
cla
plot(DB.Sig_Ind)
hold on
% plot(DB.Sig_Ind_Smooth,'r')
plot(DB.R_peak_Detect1,DB.Sig_Ind(DB.R_peak_Detect1),'bs')
% plot(DB.R_peak_Detect2,DB.Sig_Ind(DB.R_peak_Detect2),'k*')
% plot(DB.RR_Ratio,'r')
line([0,length(DB.Sig)],[0,0],'color',[0,0,0])
hold off


% --- Executes on button press in Reset_Config.
function Reset_Config_Callback(hObject, eventdata, handles)
% hObject    handle to Reset_Config (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB

DB.Max_Size_Weight = 0.1;
DB.Pr_Basic_Weight = 0.1;
DB.Ind_Delete_Weight = 0.5;
DB.Ind_Thr_Weight = 0.5;
DB.PVC_Weight = 0.4;


set(handles.Period_Thr,'string',num2str(DB.Max_Size_Weight));
set(handles.Basic_Thr,'string',num2str(DB.Pr_Basic_Weight));
set(handles.Ind_Delete_Weight,'string',num2str(DB.Ind_Delete_Weight));
set(handles.Ind_Thr_Weight,'string',num2str(DB.Ind_Thr_Weight));
set(handles.PVC_Weight,'string',num2str(DB.PVC_Weight));


% --------------------------------------------------------------------
function Auto_Load_Callback(hObject, eventdata, handles)
% hObject    handle to Auto_Load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global DB
clc

if exist('pathname_Signal.mat','file')
    pathname = importdata('pathname_Signal.mat');
else
    pathname = pwd;
end
[filename_temp, pathname] =  uigetfile('*.mat',{},pathname,'multiselect','on');
if pathname ~= 0
    save('pathname_Signal.mat','pathname')
else
    return
end
if iscell(filename_temp)
    filename = filename_temp;
else
    filename{1} = filename_temp;
end
clear filename_temp
DB.Result_Table = cell(length(filename)+1,5);
Total_Diff = [];
for FN = 1 : length(filename)
    filename{FN}
    DB = [];
    DB.fs = 250;
    DB.base = 0;
    DB.gain = 200;
    %% ½ÅÈ£ È¹µæ
    
    DB.Sig_pathname = pathname;
    DB.Sig_filename = filename{FN};
    DB.Sig = (importdata([DB.Sig_pathname, DB.Sig_filename])-DB.base)/DB.gain;
    set(handles.Signal_text,'string',DB.Sig_filename)
    DB.Sig = DB.Sig(1,:);
    DB.Sig_Length = length(DB.Sig);
    
    DB.Sig_Filt = Butterworth(DB.Sig,DB.fs);
    DB.Sig_Filt_Base = DB.Sig - DB.Sig_Filt;
    
    Kurtosis_Size = 5;
    [DB.Sig_Kur,DB.Sig_Kur_Size] = Kurtosis_Cal(DB.Sig_Filt,Kurtosis_Size);
    DB.Sig_Ind = -DB.Sig_Filt.*DB.Sig_Kur;
    
    DB.Sig_Diff = Diff_Cal(DB.Sig_Kur,12).*Diff_Cal(DB.Sig_Filt,12).^(1/4);
    
    
%     Morp_Size = 12;
%     DB.Sig_Diff = (imdilate(DB.Sig_Kur,Morp_Size)-imerode(DB.Sig_Kur,Morp_Size)).*...
%         (imdilate(DB.Sig_Filt,Morp_Size)-imerode(DB.Sig_Filt,Morp_Size));
    
    %% R-peak È¹µæ
    DB.R_peak_pathname = [DB.Sig_pathname,'Rpeak\'];
    DB.R_peak_filename = [DB.Sig_filename(1:end-4), '_R_peak.mat'];
    Annotation_Key = ['NLRBAaJSVrFejnE/fQ?'];
    import_temp = importdata([DB.R_peak_pathname, DB.R_peak_filename]);
    DB.R_peak = import_temp.R_peak;
    DB.Q_on = import_temp.Q_on;
    DB.S_off = import_temp.S_off;
    set(handles.R_peak_text,'string',DB.R_peak_filename)
    
    %% Pr È¹µæ
    DB.Primitive_pathname = [DB.Sig_pathname,'Pr\'];
    DB.Primitive_filename = [DB.Sig_filename(1:end-4), '_Pr.mat'];
    import_temp = importdata([DB.Primitive_pathname, DB.Primitive_filename]);
    DB.Pr_X = import_temp.Pr_X;
    DB.Pr = DB.Sig(DB.Pr_X);
    
    DB = Primitive_Separate(DB);
    DB.Pr_Max = max(DB.Sig_Ind(DB.Pr_X));    
    
    set(handles.Primitive_text,'string',DB.Primitive_filename)    
    set(handles.Primitive_Start,'string',num2str(max(1,DB.Pr_X(1)-100)))    
    set(handles.Primitive_Length,'string',num2str(min(length(DB.Sig),DB.Pr_X(end)+100) - max(1,DB.Pr_X(1)-100)))
    
    
    %% ÃÊ±âÈ­
    DB.fp_view = get(handles.fp_view,'value');
    DB.Max_Size_Weight = (str2double(get(handles.Period_Thr,'string')));
    DB.Pr_Basic_Weight = str2double(get(handles.Basic_Thr,'string'));
    DB.Ind_Delete_Weight = str2double(get(handles.Ind_Delete_Weight,'string'));
    DB.Ind_Thr_Weight = str2double(get(handles.Ind_Thr_Weight,'string'));
    DB.PVC_Weight = str2double(get(handles.PVC_Weight,'string'));
    %% ½ÇÇà
    tic
    DB = Primitive_QRS_20160212(DB);
    toc
%     DB.Result_Table{FN+1,1} = DB.Sig_filename;
%     DB.Result_Table{FN+1,2} = length(DB.R_peak);
%     DB.Result_Table{FN+1,3} = DB.tp;
%     DB.Result_Table{FN+1,4} = DB.fp;
%     DB.Result_Table{FN+1,5} = DB.fn;
    Total_Diff{FN+1,1} = filename{FN};
    for i = 1 : 6
        Total_Diff{FN+1,i+1} = DB.Diff_Table(i);
    end
    %% ¸ÅÄª·ü Ãâ·Â
    
%     set(handles.Result,'string',sprintf('%s Total:%5d\nTP:%5d FP:%5d FN:%5d \n',DB.Sig_filename,length(DB.R_peak), DB.tp, DB.fp, DB.fn))
%     set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))
    
    %% Figure
    axes(handles.axes1)
    cla
    plot(DB.Sig)
    hold on
    plot(DB.Sig_Filt,'r')
    line([DB.R_peak-25;DB.R_peak+25],[DB.Sig(DB.R_peak);DB.Sig(DB.R_peak)],'color','r')
    line([DB.R_peak_Detect;DB.R_peak_Detect],[DB.Sig(DB.R_peak_Detect)-1.5;DB.Sig(DB.R_peak_Detect)+1.5],'color','k')
    hold off
    
    axes(handles.axes2)
    cla
    plot(DB.Sig_Ind)
    hold on
    plot(-DB.Sig_Kur,'r')
    plot(DB.Sig_Diff,'k')
    plot(DB.R_peak_Detect,DB.Sig_Ind(DB.R_peak_Detect),'bs')
%     plot(DB.R_peak_Detect2,DB.Sig_Ind(DB.R_peak_Detect2),'k*')
    line([0,length(DB.Sig)],[0,0],'color',[0,0,0])
    hold off
    
    axes(handles.axes3)
    cla
    plot(max(1,DB.Pr_X(1)-100):min(length(DB.Sig),DB.Pr_X(end)+100), DB.Sig(max(1,DB.Pr_X(1)-100):min(length(DB.Sig),DB.Pr_X(end)+100)))
    hold on
    plot(DB.Pr1_X,DB.Pr1,'r','Linewidth',2)
    plot(DB.Pr2_X,DB.Pr2,'k','Linewidth',2)
    plot(DB.Pr3_X,DB.Pr3,'r','Linewidth',2)
    hold off
    
%     pause
end
% DB.Result_Table{1,1} = 'Record';
% DB.Result_Table{1,2} = 'Total Beat';
% DB.Result_Table{1,3} = 'TP';
% DB.Result_Table{1,4} = 'FP';
% DB.Result_Table{1,5} = 'FN';
% DB.Result_Table

Total_Diff{1,1} = 'Record';
Total_Diff{1,2} = 'Q_on mean';
Total_Diff{1,3} = 'Q_on std';
Total_Diff{1,4} = 'S_off mean';
Total_Diff{1,5} = 'S_off std';
Total_Diff{1,6} = 'QRS_Dur mean';
Total_Diff{1,7} = 'QRS_Dur std';
DB.Total_Diff = Total_Diff;
% keyboard

% --------------------------------------------------------------------
function Primitive_QRS_Callback(hObject, eventdata, handles)
% hObject    handle to Primitive_QRS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global DB
clc
DB.fp_view = get(handles.fp_view,'value');

DB.Max_Size_Weight = (str2double(get(handles.Period_Thr,'string')));
DB.Pr_Basic_Weight = str2double(get(handles.Basic_Thr,'string'));
DB.Ind_Delete_Weight = str2double(get(handles.Ind_Delete_Weight,'string'));
DB.Ind_Thr_Weight = str2double(get(handles.Ind_Thr_Weight,'string')); 
DB.PVC_Weight = str2double(get(handles.PVC_Weight,'string'));

% DB = Primitive_QRS_20160130(DB);
% DB = Primitive_QRS_20160202(DB);
% DB = Primitive_QRS_20160211(DB);
DB = Primitive_QRS_20160212(DB);
% return
%% ¸ÅÄª·ü Ãâ·Â
% set(handles.Result,'string',sprintf('%s Total:%5d\nTP:%5d FP:%5d FN:%5d \n',DB.Sig_filename,length(DB.R_peak), DB.tp, DB.fp, DB.fn))
% set(handles.False_Num,'string',sprintf('%d/%d',DB.False_View,DB.F_L))

%% Figure



axes(handles.axes1)
cla
plot(DB.Sig)
hold on
% plot(DB.Sig_Filt,'r')
% plot(DB.QRS_on_Detect,DB.Sig(DB.QRS_on_Detect),'r>')
% plot(DB.QRS_off_Detect,DB.Sig(DB.QRS_off_Detect),'r<')
line([DB.R_peak_Detect;DB.R_peak_Detect],[DB.Sig(DB.R_peak_Detect)-1.5;DB.Sig(DB.R_peak_Detect)+1.5],'color','k')
line([DB.Q_on;DB.Q_on],[DB.Sig(DB.Q_on)-1.5;DB.Sig(DB.Q_on)+1.5],'color','r','Linestyle','-.')
line([DB.S_off;DB.S_off],[DB.Sig(DB.S_off)-1.5;DB.Sig(DB.S_off)+1.5],'color','r','Linestyle','-.')
% 
% for i = 1 : length(DB.R_peak_Detect);
%     plot(DB.QRS_on_Detect_X{i},DB.QRS_on_Detect_Data{i},'k','linewidth',2)
%     plot(DB.QRS_off_Detect_X{i},DB.QRS_off_Detect_Data{i},'k','linewidth',2)
% end
hold off

axes(handles.axes2)
cla
plot(DB.Sig_Ind)
hold on
plot(DB.R_peak_Detect,DB.Sig_Ind(DB.R_peak_Detect),'bs')
line([0,length(DB.Sig)],[0,0],'color',[0,0,0])
hold off


% --------------------------------------------------------------------
function Primitive_Cluster_Callback(hObject, eventdata, handles)
% hObject    handle to Primitive_Cluster (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global DB
N = length(DB.R_peak_Detect);
L = length(DB.Pr)+2*DB.Pr_Expand_size;
LL = length(DB.Pr_L);
RL = length(DB.Pr_R);
DB.QRS_Cluster = zeros(N,L);





DB.Pr_Cluster = [interp1(linspace(0,1,LL),DB.Sig(DB.Pr_L_X(1:end)),linspace(0,1,LL)) ...
                 interp1(linspace(0,1,RL-1),DB.Sig(DB.Pr_R_X(2:end)),linspace(0,1,RL-1))];
% DB.Pr_Cluster = [interp1(linspace(0,1,LL),DB.Sig_Filt(DB.Pr_X(1:LL)),linspace(0,1,LL)) ...
%                  interp1(linspace(0,1,RL-1),DB.Sig_Filt(DB.Pr_X(LL+1:end)),linspace(0,1,RL-1))];
                       
DB.Pr_Cluster = DB.Pr_Cluster - DB.Pr_Cluster(LL);
                       
% DB.Pr_Cluster = DB.Pr_Cluster;
DB.Pr_Cluster = DB.Pr_Cluster/max(abs(DB.Pr_Cluster));
% DB.Pr_Cluster = DB.Pr_Cluster/abs(DB.Pr_Cluster(1));
% DB.Pr_Cluster = DB.Pr_Cluster/abs(DB.Pr_Cluster(end));

DB.Pr_Cluster_mean = mean(DB.Pr_Cluster);
DB.Pr_Cluster_std = std(DB.Pr_Cluster);

DB.Pr_Cluster_norm = (DB.Pr_Cluster - DB.Pr_Cluster_mean)/DB.Pr_Cluster_std;

DB. QRS_Ori_Xcorr = zeros(1,N);

for i = 1 : N
    DB.QRS_Cluster_Ori(i,:) = [interp1(linspace(0,1,DB.R_peak_Detect(i) - DB.Q_on(i)+1),DB.Sig(DB.Q_on(i) : DB.R_peak_Detect(i)),linspace(0,1,LL)) ...
                           interp1(linspace(0,1,DB.S_off(i) - DB.R_peak_Detect(i)),DB.Sig(DB.R_peak_Detect(i)+1:DB.S_off(i)),linspace(0,1,RL-1))];
%     DB.QRS_Cluster(i,:) = [interp1(linspace(0,1,length(DB.QRS_on_Detect_X{i})),DB.Sig_Filt(DB.QRS_on_Detect_X{i}),linspace(0,1,LL)) ...
%                            interp1(linspace(0,1,length(DB.QRS_off_Detect_X{i}(2:end))),DB.Sig_Filt(DB.QRS_off_Detect_X{i}(2:end)),linspace(0,1,RL-1))];
    Peak = DB.QRS_Cluster_Ori(i,LL);
    DB.QRS_Cluster_Ori(i,:) = (DB.QRS_Cluster_Ori(i,:) - Peak);
%     DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:);
    DB.QRS_Cluster_Ori(i,:) = DB.QRS_Cluster_Ori(i,:)/max(abs(DB.QRS_Cluster_Ori(i,:)));
%     DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:)/abs(DB.QRS_Cluster(i,1));
%     DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:)/abs(DB.QRS_Cluster(i,end));
    
%     DB.QRS_Cluster(i,:) = (DB.QRS_Cluster(i,:) - Peak);
    DB.QRS_Ori_Xcorr(i) = max(normxcorr2(DB.QRS_Cluster_Ori(i,:), DB.Pr_Cluster));
%     DB.QRS_Cluster_Ori_mean(i) = mean(DB.QRS_Cluster_Ori(i,:));
%     DB.QRS_Cluster_Ori_std(i) = std(DB.QRS_Cluster_Ori(i,:));
%     
%     DB.QRS_Cluster_Ori_norm(i,:) = (DB.QRS_Cluster_Ori(i,:) - DB.QRS_Cluster_Ori_mean(i))/DB.QRS_Cluster_Ori_std(i);
%     
%     DB.QRS_Ori_Xcorr(i) = sum(DB.QRS_Cluster_Ori_norm(i,:).*DB.Pr_Cluster_norm)/L;
    
end

for i = 1 : N
    DB.QRS_Cluster(i,:) = [interp1(linspace(0,1,DB.R_peak_Detect(i) - DB.QRS_on_Detect(i)+1),DB.Sig(DB.QRS_on_Detect(i) : DB.R_peak_Detect(i)),linspace(0,1,LL)) ...
                           interp1(linspace(0,1,DB.QRS_off_Detect(i) - DB.R_peak_Detect(i)),DB.Sig(DB.R_peak_Detect(i)+1:DB.QRS_off_Detect(i)),linspace(0,1,RL-1))];
%     DB.QRS_Cluster(i,:) = [interp1(linspace(0,1,length(DB.QRS_on_Detect_X{i})),DB.Sig(DB.QRS_on_Detect_X{i}),linspace(0,1,LL)) ...
%                            interp1(linspace(0,1,length(DB.QRS_off_Detect_X{i}(2:end))),DB.Sig(DB.QRS_off_Detect_X{i}(2:end)),linspace(0,1,RL-1))];
%     DB.QRS_Cluster(i,:) = [interp1(linspace(0,1,length(DB.QRS_on_Detect_X{i})),DB.Sig_Filt(DB.QRS_on_Detect_X{i}),linspace(0,1,LL)) ...
%                            interp1(linspace(0,1,length(DB.QRS_off_Detect_X{i}(2:end))),DB.Sig_Filt(DB.QRS_off_Detect_X{i}(2:end)),linspace(0,1,RL-1))];
    Peak = DB.QRS_Cluster(i,LL);
    DB.QRS_Cluster(i,:) = (DB.QRS_Cluster(i,:) - Peak);
%     DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:);
    DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:)/max(abs(DB.QRS_Cluster(i,:)));
%     DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:)/abs(DB.QRS_Cluster(i,1));
%     DB.QRS_Cluster(i,:) = DB.QRS_Cluster(i,:)/abs(DB.QRS_Cluster(i,end));
    
%     DB.QRS_Cluster(i,:) = (DB.QRS_Cluster(i,:) - Peak);
    DB.QRS_Xcorr(i) = max(normxcorr2(DB.QRS_Cluster(i,:), DB.Pr_Cluster));
    
%     DB.QRS_Cluster_mean(i) = mean(DB.QRS_Cluster(i,:));
%     DB.QRS_Cluster_std(i) = std(DB.QRS_Cluster(i,:));
%     
%     DB.QRS_Cluster_norm(i,:) = (DB.QRS_Cluster(i,:) - DB.QRS_Cluster_mean(i))/DB.QRS_Cluster_std(i);
%     
%     DB.QRS_Xcorr(i) = sum(DB.QRS_Cluster_norm(i,:).*DB.Pr_Cluster_norm)/L;
end

Diff_Table = zeros(1,N);
for i = 1 : N
    Diff_Table(i) = std(DB.Pr_Cluster - DB.QRS_Cluster(i,:));
end

Thr = 0.2;

figure
set(gcf,'color',[1,1,1])
subplot(1,2,1)
% hist(Diff_Table,0:.01:1)
hold on
for i = 1 : N
%     plot(DB.QRS_Cluster(i,:))
%     if Diff_Table(i) > Thr
%     if Diff_Table(i) <= Thr
        plot3(1:L,DB.QRS_Cluster_Ori(i,:),-i*ones(1,L))
%     end
end
plot(DB.Pr_Cluster,'r','linewidth',2)
hold off

xlabel('Sample[n]')
ylabel('Amplitude[mv]')
% title(get(handles.Signal_text,'string'))
subplot(1,2,2)
hold on
for i = 1 : N
%     plot(DB.QRS_Cluster(i,:))
%     if Diff_Table(i) > Thr
%     if Diff_Table(i) <= Thr
        plot3(1:L,DB.QRS_Cluster(i,:),-i*ones(1,L))
%     end
end
plot(DB.Pr_Cluster,'r','linewidth',2)
hold off
xlabel('Sample[n]')
ylabel('Amplitude[mv]')
Num = sum(Diff_Table<=Thr)
Num = sum(Diff_Table>Thr)

figure
set(gcf,'color',[1,1,1])
plot(DB.QRS_Ori_Xcorr)
hold on
plot(DB.QRS_Xcorr,'r')
hold off
legend('QT-DB','Proposed Algorithm')

mean(DB.QRS_Ori_Xcorr)
std(DB.QRS_Ori_Xcorr)
mean(DB.QRS_Xcorr)
std(DB.QRS_Xcorr)



qm = [0.9861, 0.9673, 0.9675, 0.9736, 0.9859, 0.9693, 0.9678, 0.9803];
qs = [0.0115, 0.0356, 0.0199, 0.0153, 0.0143, 0.0260, 0.0206, 0.0177];
pm = [0.9958, 0.9825, 0.9878, 0.9865, 0.9911, 0.9924, 0.9879, 0.9925];
ps = [0.0042, 0.0134, 0.0105, 0.0163, 0.0084, 0.0067, 0.0129, 0.0102];

figure
plot(qm,qs,'bo',pm,ps,'rs')

% figure
% hold on
% for i = 2 : N-1
%     Temp = DB.Sig(DB.R_peak_Detect(i)-30:DB.R_peak_Detect(i)+30)-DB.Sig(DB.R_peak_Detect(i));
%     Temp = Temp/max(abs(Temp));
%     plot(Temp);
% end
