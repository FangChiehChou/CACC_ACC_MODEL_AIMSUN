%% load both PEMS and Chengju data to plot
clear all;
load ('SR99N_Sep_18_2014_pems1.mat');

pems_ids = cell2mat(pemsoutput(:,7));
pm = cell2mat(pemsoutput(:, 5));
lane = cell2mat(pemsoutput(:, 9));
type = pemsoutput(:, 10);
for i=1:length(detectorID)
    id = detectorID(i, 1);
    row = pems_ids(:,1)==id;
    if sum(row) >0 && strcmp(type(row, 1), 'Mainline')
        pm_temp = pm(row, 1);
        lane_temp = lane(row, 1);
        detectorID(i, 2) = pm_temp;
        detectorID(i, 3) = lane_temp;
    end
end

% get detector reordered by pm
laneids = [2,3];
rows = (detectorID(:, 3) == laneids(1))...
    | (detectorID(:, 3) == laneids(2));
tempflow1 = flow(:, rows & detectorID(:, 2)>0);
tempspeed2= speed(:, rows & detectorID(:, 2)>0);
detectorID = detectorID(rows & detectorID(:, 2)>0,:);

[u,I,J] = unique(detectorID(:, 2), 'rows', 'first');
hasDuplicates = size(u,1) < size(detectorID(:, 2),1);
ixDupRows = setdiff(1:size(detectorID(:, 2),1), I);

[~, index] = sort(detectorID(:, 2));
detectorID = detectorID(index,:);

%find lane 1 detector and merging into one matrix
speed_surface = zeros(length(tAbs), 1);
index_good=1;
data = speed;
for i=1:length(detectorID)
    if sum(data(:, index(i,1)))>0
        speed_surface(:, index_good) = data(:, index(i, 1));
        mp_surface(index_good) = detectorID(i, 2);
        index_good = index_good+1;
    end
end

surf(tSec, mp_surface, speed_surface');





