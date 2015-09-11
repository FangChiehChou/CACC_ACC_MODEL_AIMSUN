%% Average the input data given the sectionid

function res = ...
    ProcessSectionData(data, sectionid, leading_sec, ramp_sec)

tempdata = data(data(:,2) == sectionid,:);
% remove the first 2 rows for warm-up
tempdata(1:2, :)=[];
res = mean(tempdata);

%travel time
tempdata = data(data(:,2) == leading_sec,:);
tempdata(1:2, :)=[];
res(1, 4) = res(1, 4) +mean(tempdata(:,4));
res(1,6) = (res(1,6) + mean(tempdata(:,6)))/2;

%travel time
tempdata = data(data(:,2) == ramp_sec,:);
tempdata(1:2, :)=[];
res(1, 4) = res(1, 4) +mean(tempdata(:,4));
res(1,6) = (res(1,6) + mean(tempdata(:,6)))/2;

% s_r_simu = sum(data(:,2))/n_row*0.621371; % from [km/h] to [mph]
% s_o_simu = sum(data(:,3))/n_row*0.621371; % from [km/h] to [mph]
% d_r_simu = sum(data(:,4))/n_row*1.6; % from [veh/km] to [veh/mile]
% d_outer_simu = sum(data(:,5))/n_row*1.6; % from [veh/km] to [veh/mile]
% res = [s_r_simu, s_o_simu, d_r_simu, d_outer_simu];




