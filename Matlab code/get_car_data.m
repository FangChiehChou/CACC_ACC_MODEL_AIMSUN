function leader_data = get_car_data(CFTracking,...
    id_column, lane_column, time_column, ...
    leaderid_column, id, id_is_leader)

if nargin == 6
    leader_data ...
        = CFTracking(CFTracking(:,id_column)==id,:);
else
    leader_data ...
        = CFTracking(CFTracking(:,leaderid_column)==id,:);
end

leader_data ...
    = leader_data(leader_data(:,lane_column)==2,:);
leader_data = sortrows(leader_data, time_column);