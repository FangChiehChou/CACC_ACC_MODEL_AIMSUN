%% function to see if this car is a cutter

function [output] = follower_is_cutter(CF_DATA, lane_column)
    data = CF_DATA(CF_DATA(:, lane_column)==1,:);
    [row, col] = size(data);
    if row>0
        output = 1;
    else
        output=0;
    end