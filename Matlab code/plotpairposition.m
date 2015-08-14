load('temp');

%% plot speed
% select the cut in vehicle
leader_data = ...
        get_car_data(CFTracking, id_column,lane_column,...
                           time_column,leaderid_column, leader_id);
figure('Name','speed')
axes1 = axes('Parent',traj_figure,'FontSize',16);
plotbymode(leader_data(:,time_column),...
    leader_data(:,position_column), 'o', ColorSet(1,:),...
    leader_data(:,mode_column),...
    leader_data(:,type_column),...
    0);
hold on;%also plot its leader
leader_leader_id = leader_data(:, leaderid_column);
[row, ~] = size(leader_leader_id);
if(row>0)
    leader_leader_id = leader_leader_id(1,1);
    leader_leader_data = ...
        get_car_data(CFTracking, id_column, lane_column,...
                           time_column,leaderid_column, leader_leader_id);
    plotbymode(leader_leader_data(:,time_column),...
        leader_leader_data(:,position_column), 's', ColorSet(1,:),...
        leader_leader_data(:,mode_column),...
        -1,...
        0); %-1 to plot leader's leader
    hold on;
end
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
        marker = '+';
        if follower_is_cutter(temp_follow,lane_column)  == 1
            marker = 'o';
        end
        plotbymode(temp_follow(:,time_column),...
            temp_follow(:,position_column), marker,...
            ColorSet(index,:),...
            temp_follow(:,mode_column),...
            temp_follow(:,type_column),...
            1);
        index = index + 1;
        hold on;
        % this follower is the new leader
        leader_id = follower_id;
    end
end
% Create xlabel
xlabel('Time (second)');
% Create ylabel
ylabel('Position (meter)');
hold off;