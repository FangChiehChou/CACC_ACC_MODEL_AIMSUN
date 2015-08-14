%% this file reads the cf data and give figures of following status
%% format: Time, ID, Type, Section, Lane, 
%% Position, Speed, acc, v_f, min_headway, 
%% max_dec, max_acc, 
%% leader_ID, leader_type, 
%% leader_speed, leader_accelertion, leader_pos, 
%% distance_gap, time_gap, 
clear all
close all
% FileName = 'C:\CACC_Simu_Data\CF_Tracking.txt';
[FileName,PathName,FilterIndex] = ...
    uigetfile('*.txt','Select the detector data');
fileID = fopen(strcat(PathName,'\',FileName));
CFTracking = textscan(fileID,...
        '%f %f %f %f %f %f %f %f %f %f %f %f%f %f %f %f%f %f %f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf);
 fclose(fileID);
 CFTracking = RemoveExtraLine(CFTracking);
 CFTracking = cell2mat(CFTracking);
%define position of column
DataPrepare();
load('data_index');

num_veh = max(CFTracking(:, id_column))...
    -min(CFTracking(:, id_column));

CFTracking=sortrows(CFTracking, id_column);
save('CF_DATA');
plottrjecotry;
plottpairs;

