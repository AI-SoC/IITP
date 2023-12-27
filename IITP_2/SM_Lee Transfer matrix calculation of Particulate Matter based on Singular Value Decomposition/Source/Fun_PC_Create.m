
function Result = Fun_PC_Create(Sig, Fs, BLOCK_SIZE, Thr, Width)
L = length(Sig);
%% pc 초기화
categoryData = 0;
categoryData_pre = 0;
smplCnt = 0;
pcCurState = 0; % 0: FLAT, 1: Fall, 2: Rising
wCnt = 0;
smplData_pre = 0;
pcCount = zeros(1,size(Thr,2));

%% 카테고리 획득
smplCat = sum(Sig > Thr,2)+1;
%% 1초 단위 데이터 추출
T = min(inf, floor(L/Fs));
Result = zeros(T, size(Thr,2));

for ii = 1 : T
    pcCount = zeros(1,size(Thr,2));
    smplCnt = 1;  %% 읽은 샘플 수 초기화
    while smplCnt < Fs  %% Fs만큼 데이터 읽어서 카운트 측정
        readcnt = min(BLOCK_SIZE, Fs-smplCnt);        
        for iu = (ii-1)*Fs+smplCnt : (ii-1)*Fs + smplCnt+readcnt -1
            categoryData = smplCat(iu);
            if categoryData_pre == 1  %% categoryData_pre-1 이 0이 되는 경우 예외처리
                wCnt = 1;
            elseif categoryData == categoryData_pre
                if pcCurState == 1 && Sig(iu) < smplData_pre
                    wCnt = wCnt+1;
                    if wCnt >= Width(categoryData_pre-1)
                        pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                        wCnt = wCnt-Width(categoryData_pre-1);
                    end
                    pcCurState = 1;
                elseif pcCurState == 2 && Sig(iu) > smplData_pre
                    wCnt = wCnt+1;
                    if wCnt >= Width(categoryData_pre-1)
                        pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                        wCnt = wCnt-Width(categoryData_pre-1);
                    end
                    pcCurState = 2;
                else
                    if categoryData > 1
                        wCnt = wCnt + 1;
                        if wCnt >= Width(categoryData_pre-1)
                            pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                            wCnt = wCnt-Width(categoryData_pre-1);
                            pcCurState = 1;
                        else
                            pcCurState = 0;
                        end
                    else
                        wCnt = 0;
                        pcCurState = 0;
                    end
                end
            elseif categoryData < categoryData_pre
                if pcCurState ~= 1
                    if categoryData > 1
                        if wCnt >= Width(categoryData_pre-1)
                            pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                            wCnt = wCnt-Width(categoryData_pre-1);
                        elseif wCnt > 0
                            pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                            wCnt = 0;
                        end
                    else
                        if wCnt >= Width(categoryData_pre-1)
                            pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                        end
                        wCnt = 0;
                    end
                else
                    if categoryData > 1
                        if wCnt >= Width(categoryData_pre-1)
                            pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                        end
                        wCnt = 1;
                    else
                        wCnt = 0;
                    end
                end
                pcCurState = 1;
            else
                if categoryData_pre > 1
                    if wCnt >= Width(categoryData_pre-1)
                        pcCount(categoryData_pre-1) = pcCount(categoryData_pre-1)+1;
                    end
                end
                wCnt = 1;
                pcCurState = 2;
            end
            categoryData_pre = categoryData;
            smplData_pre = Sig(iu);            
        end
        smplCnt = smplCnt + readcnt;  % 읽은 길이 만큼 카운트 증가
    end
    Result(ii,:) = pcCount;
end