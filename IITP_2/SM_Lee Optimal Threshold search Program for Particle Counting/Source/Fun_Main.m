clc
clear
close all

%% 변수 초기화
% GWD 샘플링 주파수
Fs = 50000;

% PC 값 계산 시에 사용되는 배열 크기
BLOCK_SIZE = 2048; 

% 초반 데이터 삭제 샘플 개수, 초기 데이터 오염 구간을 제거하기 위해 사용, 없을 경우 default로 1
cut_Num = 1; 

% 탐색 PC 클래스 개수,  PC0.3을 시작으로 PC0.5, ..., PC10까지를 탐색
% GWD 측정장비에서 측정가능한 수준에 맞게 N을 결정
N = 4; 

% 임계값 탐색 시 증분값, 낮을수록 정밀하나 수행시간이 크게 증가
Inc = 1; 

% 기저선 변동 제거 시 사용할 평균필터 사이즈
w_size = 49; 

Fig_smooth = 1; % filter 사용 시, 결과 출력, 
Flag_detection = 0; % 1은 신규탐색 0은 기존 탐색결과 불러오기
Fig_Result = 1; % 최종 결과 출력
Fig_Thr = 0; % PM Thr 설정 결과 출력

% Width는 현재 1,1,1,1,1,1,1로 픽스
% Width 탐색을 원할 경우, Thr 탐색 구간에 Width 탐색 다중루프문을 동일한 방식으로 추가
% 기하급수적인 수행시간 증가가 예상되므로, Width 탐색 범위는 최소화
Width = [1, 1, 1, 1, 1, 1, 1];

% grimm 장비의 PM농도에 따른 구간 분할을 위한 임계값
% PM10의 농도값을 입력
% 높은 값에서부터 낮은값으로 입력 140 120 90 ...
% 구간 분할을 적용하지 않을 경우 빈행렬값으로 입력
cut_Thr = [];
% 구간 분할 임계값의 개수 저장
cut_L = length(cut_Thr)+1;
% 분할된 구간의 샘플 인덱스 저장
% 초기 시작샘플 1을 초기화하여 저장
cut_Index = 1;

%% 파일 불러오기
% 파일명 선택(폴더명)
% 다른 파일의 임계값 정보를 적용할 수 있도록
% filename_PC에 별도로 저장된 파일명의 임계값 결과물을 불러오도록 설정
% filename_PC가 filename과 같을 경우, 자기 자신의 기존 결과물이 불러와짐

filename = '1016c';  % 불러올 데이터 명
filename_PC = filename;  % 불러올 임계값 결과물 데이터 명
filename_F = '1016c';  % 불러올 임계값 결과물 데이터 명

% Grimm 장비 데이터 불러오기
DB1 = importdata([pwd,'\',filename,'\','DB1_',filename,'.mat']); % grimm PC 데이터 불러오기, DB_read.m 파일로 데이터 생성
DB_PM = importdata([pwd,'\',filename,'\','DB1_',filename,'_PM_sat.mat']); %grimm PM 데이터 불러오기, DB_PM_Read.m 파일로 데이터 생성
DB_PM10 = DB_PM(:,4); % grimm 장비의 PM10 값 별도 저장

% GWD ADC 데이터 불러오기
% 필터링 된 ADC값 종류 선택
file_type = 3;
% 1: FIR필터 + IIR bypass      filter_gain4p5 데이터
% 2: FIR bypass + IIR 필터
% 3: 기존 FIR 필터 + IIR 필터   avg3 데이터
% 1028 dataset은 file_type 3, avg3 데이터만 사용

fid = fopen([pwd,'\',filename,'\',filename,'_filter_noise_avg3.pcm'],'r');
DB2 = fread(fid, inf,'int16');
fclose(fid);


filter_type = 0; % 0: filter 미사용,  1: FIR filter, 2: IIR filter

% 2048 이상의 값들은 0으로 변경
for i = 1 : length(DB2)
    if DB2(i) > 2047
        DB2(i) = 0;
    end
end

%% 데이터 길이 조정
% grimm 장비 측정 데이터 길이와 GWD 측정 데이터의 길이를 일치시키기 위한 전처리
% grimm 장비는 PC값이 1/6 Hz로 6초에 한 샘플씩 측정
% GWD의 ADC는 50 KHz
% 따라서, 30만배의 샘플링 차이가 발생함
% grimm PC 길이의 30만배까지 GWD 데이터를 잘라내거나,
% GWD의 1/300000 길이만큼 grimm PC값을 잘라냄

% grimm 장비와 GWD의 길이 비교 후, 짧은 길이 선택
Min_L = min(size(DB1,1), floor(size(DB2,1)/50000/6));
% Min_L만큼 데이터 잘라내기
DB1 = DB1(1:Min_L,:);
DB2 = DB2(1:Min_L*300000,:);
DB_PM = DB_PM(1:Min_L,:);
DB_PM10 = DB_PM10(1:Min_L,:);


% 추출된 데이터의 길이 재측정
L = length(DB1);

%% 저농도 구간 분리
% PM10 값의 농도가 100 이하로 떨어지는 지점을 저장
% 해당 지점을 기준으로 데이터를 정규화하여 차후 비교
cut1 = 1;
for i = 1 : Min_L
    if DB_PM(i,4) < 100
        cut1 = i;
        break
    end
end

%% PM 기반 구간 설정
% cut_Thr에 저장된 PM10의 농도값이 있을 경우 동작
if ~isempty(cut_Thr)
    % PM10값에 대한 평균 필터를 취함
    ms = 20; % 평균필터 사이즈
    L = length(DB_PM10);
    DB_PM10_M = zeros(L,1); % 평균필터된 PM10값 저장
    for i = 1 : L
        ms_temp = min([ms, i-1, L-i]);
        DB_PM10_M(i,1) = sum(DB_PM10(i-ms_temp:i+ms_temp))/(2*ms_temp+1);
    end
    
    % cut_Thr에 저장된 농도보다 작은 값이 나타날 경우, 구간을 분할
    for j = 1 : length(cut_Thr)
        cut_flag = 0;
        for i = 2 : L % cut_Index는 1로 초기화되어 있으므로 2부터 체크 시작
            if DB_PM10_M(i) < cut_Thr(j)
                cut_Index = [cut_Index, i]; % 분할되는 지점 샘플 인덱스 저장
                cut_flag = 1;
                break
            end
        end
        if cut_flag == 0
            cut_Thr(j) = [];
            cut_L = cut_L-1;
        end
    end
    
    if Fig_Thr
        figure
        plot(DB_PM10,'b-.')
        hold on
        plot(DB_PM10_M,'r')
        for j = 1 : length(cut_Thr)
            plot([1,L],[cut_Thr(j), cut_Thr(j)],'k')
        end
        legend('PM10','mean PM','cut Thr')
        return
    end
end


%% 구간 분할
% 마지막 인덱스는 신호길이+1로 저장
% 각 구간의 종료샘플 위치를 다음 인덱스-1 지점을 저장하므로 +1을 해야 L번째 샘플이 불러와짐
cut_Index(end+1) = L+1; 
for i = 1 : cut_L
    DB1_cut{i} = DB1(cut_Index(i) : cut_Index(i+1)-1,:);
    DB_PM_cut{i} = DB_PM(cut_Index(i) : cut_Index(i+1)-1,:);
    DB2_cut{i} = DB2((cut_Index(i)-1)*300000+1 : (cut_Index(i+1)-1)*300000);
end


%% 최적 임계값 탐색
Result_Total = [];
% Flag_detection이 1일 경우 최적임계값 탐색 시작
% 0일 경우, 기존 저장된 최적임계값을 불러와서 바로 출력
if Flag_detection == 1 
    % 임계값을 최대치로 초기화
    Thr = 2048*ones(1,7);
    % 오차가 최소화가 되는 임계값 정보 저장
    min_Thr = Thr;
    
    % 각 구간별 최적임계값 탐색
    for j = 1 : cut_L
        % 이전 구간의 최적 임계값을 활용하여 탐색범위 최소화를 위한 임계값 정보 저장
        Thr_before = min_Thr;
        % 첫번째 임계값의 탐색 시작값
        Base(j) = 0;
        
        % ADC값으로부터 주어진 임계값에 따른 PC값 계산 결과 저장
        % 50 KHz -> 1 Hz
        Result_Temp = [];
        
        % 1초 단위로 저장된 Result_Temp값을 6초 단위로 다운샘플링
        % 1 Hz -> 1/6 Hz
        % grimm PC값과 동일한 길이로 일치됨
        Result = [];
        
        % 탐색할 Thr 개수만큼 다중루프문을 진행
        for Dep = 1 : N
            Dep;
            % Dep가 1일 때는 이전 Dep의 임계값 정보가 없으므로, Base로부터 탐색 시작
            if Dep == 1
                % 현재 측정된 Result PC값과 grimm PC값과의 최소 오차값 저장
                % 무한대로 초기화
                min_Diff = inf;
                
                % Thr값을 증가시키며 탐색
                for Th_temp = Base(j) : Inc : min([Thr_before(Dep),Base(j)+30])
                    Thr(1) = Th_temp;
                    % 결정된 첫번째 임계값에 기반하여 PC값 획득
                    Result_Temp = Fun_PC_Create(DB2_cut{j}, Fs, BLOCK_SIZE, Thr, Width); % PC값 획득
                    for i = 1 : size(DB1_cut{j},1)
                        Result(i,:) = mean(Result_Temp(6*(i-1)+1:6*i,:)); % 6초 단위 합계로 다운 샘플링
                    end
                    
                    % 현재 측정된 값은 dPC값에 가까우므로, 이를 누적하여 보정된 PC값을 획득
                    for i = 6:-1:1
                        Result(:,i) = Result(:,i)+Result(:,i+1);
                    end
                    
                    % 획득된 PC값과 reference인 grimm 장비의 PC값과의 유사도 측정
                    % N번째 PC값까지만 이용하며, PM10이 100이 되는 cut1 지점을 기준으로 정규화하여 
                    % 오차 비교
                    [Diff, Diff_vec] = Fun_PC_Comp(Result, DB1_cut{j}, N,cut1); % Reference와 오차 비교
                    
                    % 기존 오차보다 작을 경우 최소 오차정보, 최소 오차 임계값, 등을 갱신
                    if Diff < min_Diff  
                        min_Thr = Thr;
                        min_Diff = Diff;
                        min_Diff_vec = Diff_vec;
                    end
                end
            else
                % Dep가 1이 아닐 경우, 이전 Thr값에서부터 탐색을 진행
                Thr = min_Thr;
                min_Diff = inf;
                % 이전 Thr값과 이전 탐색구간의 임계값 등을 활용하여 탐색구간을 줄여 봄
                % 수동으로 결정된 부분이므로, 차후 지속적인 분석을 통해 개선이 필요함
                for Th_temp = Thr(Dep-1) : Inc : min([2040,max(Thr(Dep-1)*3, Thr(Dep-1)+100), Thr_before(Dep)])
                    Thr(Dep) = Th_temp;
                    Result_Temp = Fun_PC_Create(DB2_cut{j}, Fs, BLOCK_SIZE, Thr, Width); % PC값 획득
                    for i = 1 : size(DB1_cut{j},1)
                        Result(i,:) = mean(Result_Temp(6*(i-1)+1:6*i,:)); % 6초 단위 합계로 다운 샘플링
                    end
                    for i = 6:-1:1
                        Result(:,i) = Result(:,i)+Result(:,i+1);
                    end
                    [Diff, Diff_vec] = Fun_PC_Comp(Result, DB1_cut{j}, N,cut1); % Reference와 오차 비교
                    if Diff < min_Diff  % 기존 오차보다 작을 경우 갱신
                        min_Thr = Thr;
                        min_Diff = Diff;
                        min_Diff_vec = Diff_vec;
                    end
                end
            end
        end
        % j번째 구간의 최소오차일 때 임계값 및 PC값 정보를 저장
        Thr_Total(j,:) = min_Thr;
        Result_cut{j} = Result;
        Result_Total = [Result_Total; Result_cut{j}];
    end
    % 최적 임계값 정보 저장
    if file_type == 1
        save([filename,'_cut_Total_Thr_filter',sprintf('_%d',w_size),'.mat'],'Thr_Total')
    elseif file_type == 2
        if filter_type == 1
            save([filename,'_cut_Total_Thr_pga2_FIR',sprintf('_%d',w_size),'.mat'],'Thr_Total')
        elseif filter_type == 2
            save([filename,'_cut_Total_Thr_pga2_IIR',sprintf('_%d',w_size),'.mat'],'Thr_Total')
        end
    elseif file_type == 3
        save([filename,'_cut_Total_Thr_avg3.mat'],'Thr_Total')
    end
    
else
    % 최적임계값 탐색을 하지 않고, 기존 최적임계값 검출결과를 불어오기
%     if file_type == 1
%         Thr_Total = importdata([filename,'_cut_Total_Thr_filter',sprintf('_%d',w_size),'.mat'])
%     elseif file_type == 2
%         if filter_type == 1
%             Thr_Total = importdata([filename,'_cut_Total_Thr_pga2_FIR',sprintf('_%d',w_size),'.mat'])
%         elseif filter_type == 2
%             Thr_Total = importdata([filename,'_cut_Total_Thr_pga2_IIR',sprintf('_%d',w_size),'.mat'])
%         end
%     elseif file_type == 3
%         Thr_Total = importdata([filename,'_cut_Total_Thr_avg3.mat'])
%     end

    % 다른 파일의 임계값 정보를 적용할 수 있도록
    % filename_PC에 별도로 저장된 파일명의 임계값 결과물을 불러오도록 설정
    % filename_PC가 filename과 같을 경우, 자기 자신의 기존 결과물이 불러와짐
    Thr_Total = importdata([filename_PC,'_cut_Total_Thr_avg3.mat']) 
    
        
    % 불러온 임계값 정보를 바탕으로 PC값 측정
    for j = 1 : cut_L
        Thr = Thr_Total(j,:);
        Result = [];
        Result_Temp = [];

        Result_Temp = Fun_PC_Create(DB2_cut{j}, Fs, BLOCK_SIZE, Thr, Width); % PC값 획득
        for i = 1 : size(DB1_cut{j},1)
            Result(i,:) = mean(Result_Temp(6*(i-1)+1:6*i,:)); % 6초 단위 합계로 다운 샘플링
        end
        for i = 6:-1:1
            Result(:,i) = Result(:,i)+Result(:,i+1);
        end
        Result_cut{j} = Result;
        Result_Total = [Result_Total; Result_cut{j}];
    end
end



if Fig_Result == 1
    figure
    for i = 1 : cut_L
        subplot(2,cut_L,i)
        plot(DB1_cut{i})
        title(sprintf('grimm PC %d번째 구간',i))
        ylim([0,max(DB1(:))])
        subplot(2,cut_L,i+cut_L)
        plot(Result_cut{i})
        title(sprintf('장비 PC %d번째 구간',i))
        ylim([0,max(Result_Total(:))])
    end
    
    figure
    subplot(2,1,1)
    plot(DB1)
    title('grimm PC 통합')
    subplot(2,1,2)
    plot(Result_Total)
    title('장비 PC 통합')
    
    Result_Total2 = Result_Total;
    for i = 1 : size(Result_Total,1)
        if Result_Total(i,1) > 400
            Result_Total2(i,1) = (Result_Total2(i,1)-400)*((65000-400)/(1000-400))+400;
            Result_Total2(i,2) = Result_Total2(i,2)*((65000-400)/(1000-400));
            Result_Total2(i,3) = Result_Total2(i,3)*((65000-400)/(1000-400));
        end
    end
    figure
    subplot(2,1,1)
    plot(DB1)
    title('grimm PC 통합')
    subplot(2,1,2)
    plot(Result_Total2)
    title('장비 PC 통합')
    
end


L2 = floor(size(DB2,1)/50000/6); % PC 샘플 데이터 길이

Result_Total2 = Result;

return