function res = ...
    ProcessMergeSectionData(data)

[n_row,~] = size(data);
s_r_simu = sum(data(:,2))/n_row*0.621371; % from [km/h] to [mph]
s_o_simu = sum(data(:,3))/n_row*0.621371; % from [km/h] to [mph]
d_r_simu = sum(data(:,4))/n_row*1.6; % from [veh/km] to [veh/mile]
d_outer_simu = sum(data(:,5))/n_row*1.6; % from [veh/km] to [veh/mile]
res = [s_r_simu, s_o_simu, d_r_simu, d_outer_simu];


