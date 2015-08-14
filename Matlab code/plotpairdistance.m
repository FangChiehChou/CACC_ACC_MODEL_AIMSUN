load('temp');

%% plot speed
% select the cut in vehicle
leader_data = ...
        get_car_data(CFTracking, id_column,lane_column,...
                           time_column,leaderid_column, leader_id);
traj_figure = figure('Name','gap')
axes1 = axes('Parent',traj_figure,'FontSize',16);
plotgap(leader_data(:,time_column),...
    leader_data(:,distancegap_column), 'k');
hold on;
index=2;
for i=1:plot_sample_num
    % find vehicle who has this cut-in car as its leader
    % on the left lane    
    followerer_data = ...
        get_car_data(CFTracking, id_column,lane_column,...
                           time_column, leaderid_column, leader_id, 1);
    f_ids = unique(followerer_data(:,id_column));
    [row, ~] = size(f_ids);
    if row==0
        break;
    else
        % get the follower id
        follower_id = f_ids(1,1);
        % if the follower is also a cutter, plot with dash
        temp_follow = CFTracking(...
                                 CFTracking(:,id_column)==follower_id, :);
        
        plotgap(temp_follow(:,time_column),...
            temp_follow(:,distancegap_column),...
            ColorSet(index,:));
        index = index + 1;
        hold on;
        % this follower is the new leader
        leader_id = follower_id;
    end
end
% Create xlabel
xlabel('Time (second)');
% Create ylabel
ylabel('Spacing (meter)');
hold off;