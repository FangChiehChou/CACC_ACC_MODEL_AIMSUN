%% script to load and test the performances
%    of different combination of parameters

close all;

start_row = 7*12; % simulation start time
% field_ids =  [312757, 312771, 312857];
% simu_ids = {[3037 3059 3061 3158 3065],...
%                     [2939 2937 2936 0 0],...
%                     [3150 3152 3154 0 0]};
field_ids =  [312771];
simu_ids = {[2939 2937 2936 0 0]};
                
% read xml file to get the range of paramters
params = ReadXML('c:\\CACC_Simu_Data\\batchexample.xml');
paracombs = GetParamComb(params);

%now get the path for each combination of paramter
[row, col]  = size(paracombs);
for i=1:row
    
    detector_data = ReadDetector(paracombs(i, :), ...
        3164, 'c:\\CACC_Simu_Data\\acc0_cacc0\\');
    title = GenTitle(paracombs(i, :), params);
    ProcDetData(detector_data, ...
        simu_ids, field_ids, start_row, title);
    
end
