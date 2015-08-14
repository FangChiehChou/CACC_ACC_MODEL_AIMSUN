%% plot pairs of trajectory of speed profiles

load('CF_DATA');
%% first fine the car who has the cut behavior
cut_index=1;
for i =1: length(ids)
    id = ids(i);
    data = CFTracking(CFTracking(:,id_column)==id,:);
    % data that contains the cut car 
    data = data(data(:,lane_column)==1,:); 
    [row, ~] = size(data);
    if (row > 0)
        cut_ids(cut_index) = id;
        cut_index = cut_index+1;
    end
end
% leader_id = cut_ids(13);
leader_id = 707;
plot_sample_num =5;
save('temp');
plotpairspeed();
plotpairdistance();
plotpairposition();
