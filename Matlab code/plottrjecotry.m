%% plot trajectory
load('CF_DATA');
ids = unique(CFTracking(:,id_column));
traj_figure = ...
    figure('Name','All trajectories');
axes1 = axes('Parent',traj_figure,'FontSize',16);
for i =1: length(ids)
    id = ids(i);
    data = CFTracking(CFTracking(:,id_column)==id,:);
    color = ColorSet(1,:);
    if length(unique(data(:,lane_column)))>1
        color = ColorSet(3,:);
    end
    data = data(data(:,lane_column)==2,:);
    if(isempty(data))
        continue;
    end
    plot(data(:, time_column), data(:, position_column),...
        'color', color); hold on;
%     plot(data(:, time_column), data(:, position_column),...
%         'color', ColorSet(1+int32(mod(i, length(ColorSet))),:)); hold on;
    text(data(1, time_column),data(1, position_column),...
        int2str(id)); hold on;
end
% Create xlabel
xlabel('Time (second)');
% Create ylabel
ylabel('Postion (meter)');
hold off;