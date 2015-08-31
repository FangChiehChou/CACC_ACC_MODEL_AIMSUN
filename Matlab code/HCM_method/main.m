
% define geometries
l_a = 900; % acceleration lane length [ft]
n_lane = 4; % number of lanes
n_o = n_lane-2; % number of outer lanes

% define speed limits
s_ff = 65; % mainlane free flow speed [mph]
s_fr = 50; % on-ramp [mph]

% define flow
v_r = 500; 
v_f = 3000;

v_r_increment = 100; % on ramp flow [veh/hr/lane]
v_r_max = 2000;
v_r_min = 100; 

v_f_increment = 200;  % mainlane [veh/hr] across all lanes
v_f_max = 7000;
v_f_min = 2000;

num_vals_r =  (v_r_max-v_r_min)/v_r_increment+1;
num_vals_f =  (v_f_max-v_f_min)/v_f_increment+1;

d_r = zeros( num_vals_r, num_vals_f);
s_r = zeros( num_vals_r, num_vals_f);
s_o = zeros( num_vals_r, num_vals_f);
s = zeros( num_vals_r, num_vals_f);
v_12_prop = zeros( num_vals_r, num_vals_f);

for v_r = v_r_min:v_r_increment:v_r_max
    for v_f = v_f_min:v_f_increment:v_f_max
        i = (v_r-v_r_min)/v_r_increment+1;
        j = (v_f-v_f_min)/v_f_increment+1;
        [d_r(i,j), s_r(i,j), s_o(i,j), s(i,j),v_12_prop(i,j)] = Merging(l_a,n_lane,v_r,v_f,...
                        s_ff, s_fr);
    end
end

% surf(d_r); hold on;
% surf(v_12_prop); 
% surf(s_r); hold on;
% surf(s_o); hold on;
% surf(s); hold off;

%% load output files from AIMSUN

PathName = 'C:\CACC_Simu_Data\acc0_cacc0\461\detector\';
FileNames = {'merge_section.txt', 'detector_run.txt'};

fileID_merge = fopen(strcat(PathName,'\',FileNames{1,1}));
merging_area = textscan(fileID_merge,...
        '%f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf); 
merging_area = cell2mat(merging_area); fclose(fileID_merge);

fileID_before = fopen(strcat(PathName,'\',FileNames{1,2}));
detector_mainlane = textscan(fileID_before,...
        '%f %f %f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf);
detector_mainlane = cell2mat(detector_mainlane);fclose(fileID_before);

period = max(detector_mainlane(:,1))/3600; % time period [hr]
v_12_simu = sum(detector_mainlane(detector_mainlane(:,3) == 1,4))...
    /period;
v_34_simu = sum(detector_mainlane(detector_mainlane(:,3) == 3,4))...
    /period;
v_f_simu = v_12_simu+v_34_simu;

[n_row,~] = size(merging_area);
s_r_simu = sum(merging_area(:,2))/n_row*0.621371; % from [km/h] to [mph]
s_o_simu = sum(merging_area(:,3))/n_row*0.621371; % from [km/h] to [mph]
d_r_simu = sum(merging_area(:,4))/n_row*1.6; % from [veh/km] to [veh/mile]
d_outer_simu = sum(merging_area(:,5))/n_row*1.6; % from [veh/km] to [veh/mile]
v_12_prop_simu = v_12_simu/(v_f_simu); % get lane distribution

v_r = 500;
[d_r_hcm, s_r_hcm, s_o_hcm, s_hcm,v_12_prop_hcm] ...
    = Merging(l_a,n_lane,v_r,v_f_simu,s_ff, s_fr);

%% display results:
display({'Simulated density ', d_r_simu, 'vs HCM density ', d_r_hcm});
display({'Simulated proportion of v_12', v_12_prop_simu, 'vs HCM  proportion of v_12 ', v_12_prop_hcm});
display({'Simulated inner speed', s_r_simu, 'vs HCM inner speed ', s_r_hcm});
display({'Simulated outer speed', s_o_simu, 'vs HCM outer speed ', s_o_hcm});



