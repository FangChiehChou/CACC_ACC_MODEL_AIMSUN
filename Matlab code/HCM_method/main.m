
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
v_r_max = 1000;
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
surf(v_12_prop); 
% surf(s_r); hold on;
% surf(s_o); hold on;
% surf(s); hold off;
