%% Plot comparisons of detector results with the field data

clear all;
close all;
load('fielddata.mat');



% define simulation detector and its corresponding field detector
field_ids =  [312757, 312771, 312857];
simu_ids = {[3037 3059 3061 3158 3065],...
                    [2939 2937 2936 0 0],...
                    [3150 3152 3154 0 0]};
              %                     [3136 3137 3139 0 0],...
field_simu_pair = containers.Map(field_ids,simu_ids);

%Reps
reps = [904, 905, 906, 907, 3164, 3165, 3166, 3167, 3168];

for i=1:1%length(reps)
    %clear temp vars
    clear simu_speed_pair simu_flow_pair field_speed_pair field_flow_pair;
    
    %folder
    PathName = strcat(...
        'C:\CACC_Simu_Data\acc0_cacc0\', int2str(reps(i)), '\detector\detector_run.txt');
    
    % first read field flow and speed 
    fileID_merge = fopen(PathName);
    sim_data = textscan(fileID_merge,...
        '%f %f %f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf); 
    sim_data = cell2mat(sim_data); fclose(fileID_merge);
    [cellrow, cellcol] = size(simu_ids);
    index = 1;
    for j=1:cellcol
        for k=1:cellrow
            vals = simu_ids{k, j};
            [valrow,valcol] = size(vals);
            for m=1:valcol
                if vals(1, m) ~=0
                    data = sim_data(sim_data(:, 2) == vals(1, m),:);
                    sim_data_key(index) = vals(1, m);
                    sim_data_speed{index}= data(:, 5)*0.621371;
                    sim_data_flow{index}= data(:, 4)*12;
                    end_row = length(data(:,4))+start_row-1;
                    
                    var_name = strcat('flow_', int2str(field_ids(1, j)));
                    eval(strcat('tempdata = ', var_name, '(start_row:end_row, m+1);'));                    
                    field_data_flow{index}= tempdata*12;    
                    var_name = strcat('speed_', int2str(field_ids(1, j)));      
                    eval(strcat('tempdata = ', var_name, '(start_row:end_row, m+1);'));           
                    field_data_speed{index}= tempdata;
                    
                    index = index+1;
                end
            end
        end
    end    
    simu_speed_pair = containers.Map(sim_data_key,sim_data_speed);
    simu_flow_pair = containers.Map(sim_data_key,sim_data_flow);
    field_speed_pair = containers.Map(sim_data_key,field_data_speed);
    field_flow_pair = containers.Map(sim_data_key,field_data_flow);
    
    % then plot
    [key_row, key_col] = size(keys(simu_speed_pair));
    det_ids_key = keys(simu_speed_pair);
    
    for j=1:key_col
        key = det_ids_key{1, j};
        temp = simu_speed_pair(key);
        temp1 = field_speed_pair(key);
        plotspeed(temp, temp1, key, 2*j);
                        
        temp = simu_flow_pair(key);
        temp1 = field_flow_pair(key);
        plotflow(temp, temp1, key, 2*j+1);
       
    end
end