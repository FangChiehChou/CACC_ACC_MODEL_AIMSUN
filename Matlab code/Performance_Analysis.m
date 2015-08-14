%% plot density, flow, and speed information 
close all;
MAINLANE = 23551;

FileName = 'C:\CACC_Simu_Data\detecotr_info.txt';
fileID = fopen(FileName);
Detector_Info = textscan(fileID,'%f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf);
fclose(fileID);
Detector_Info = cell2mat(Detector_Info);
Detector_Info=sortrows(Detector_Info, 4);
Detector_Info(:,4) =max( Detector_Info(:,4))-Detector_Info(:,4);
%open detector data
[FileName,PathName,FilterIndex] = ...
    uigetfile('*.txt','Select the detector data');
fileID = fopen(strcat(PathName,'\',FileName));
Detector_Data= textscan(fileID,'%f %f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf);
fclose(fileID);

%transfer from cells to matrices
Detector_Data = cell2mat(Detector_Data);
%sum up the flow from the first detector
Total_Flow = sum(Detector_Data (Detector_Data(:, 2)==Detector_Info(1,1),3));
%find all detectors on the mainlane
DetectorMainLane = Detector_Info(...
    Detector_Info(:,2) == MAINLANE, :);
DetectorMainLane = DetectorMainLane(...
    DetectorMainLane(:,3) == 2, :);

save('detector_data');
DrawFigures;



