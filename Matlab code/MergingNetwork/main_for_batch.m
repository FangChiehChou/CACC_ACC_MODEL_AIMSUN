%% This file reads the results from the batch simulation
%    and plot the figures
%    author: Dali Wei
%    create date: 09/07/2015

%% first define paramters

clear all;
close all;
rids = [461,904,907,905];%replication id
acc = 0;
cacc=0;
merge_section = 332;
mainlane = 330;
ramp = 331;
data_dir = strcat('C:\CACC_Simu_Data\acc',int2str(acc),...
    '_cacc',int2str(cacc),'\',int2str(rids(1,1)),'\detector\');
listing = dir(data_dir);
[row,~] = size(listing);
flowindex =1;
for i=1:row
    file = listing(i);
    if(file.isdir==0)
        if(strfind(file.name,'txt') >0) %this is a txt file
            if(strfind(file.name, 'section_run_')>0)
                index = strfind(file.name, '_');
                index_dot = strfind(file.name,'.');
                onramp(flowindex) = sscanf(file.name(index(2)+1:index(3)-1),'%d');
                through(flowindex) = sscanf(file.name(index(3)+1:index(4)-1),'%d');
                offramp(flowindex) = sscanf(file.name(index(4)+1:index_dot-1),'%d');
                sectionfilenames{flowindex} = file.name;
                suffix = strcat(int2str(onramp(flowindex)), '_',...
                    int2str(through(flowindex)), '_', int2str(offramp(flowindex)),'.txt');
                mergefilenames{flowindex} = strcat('merge_section_', suffix);
                detectorfilenames{flowindex} = strcat('detector_run_', suffix);
                flowindex = flowindex+1;
            end
        end
    end
end

% combine flows
flows = [onramp; through; offramp];
flowindex=flowindex-1;

%% then import files
for i=1:flowindex
    data={};
    for j = 1: length(rids)
        data_dir = strcat('C:\CACC_Simu_Data\acc',int2str(acc),...
                '_cacc',int2str(cacc),'\',int2str(rids(1, j)),'\detector\');
% read merging area data
%     fileID_merge = fopen(strcat(data_dir,'\',mergefilenames{1,i}));
%     merging_area = textscan(fileID_merge,...
%         '%f %f %f %f %f',...
%         'Delimiter',',','EmptyValue',-Inf); 
%     merging_data{i} = cell2mat(merging_area);
%     fclose(fileID_merge);
%     mergesectiondata(i,:)...
%        = ProcessSectionData(merging_data{i});
    
    %read section data
        fileID_section = fopen(strcat(data_dir,'\',sectionfilenames{1,i}));
        if(fileID_section > 0)
            section = textscan(fileID_section,...
                '%f %f %f %f %f %f %f %f %f %f',...
                'Delimiter',',','EmptyValue',-Inf); 
            data{j,1}...
                = ProcessSectionData(cell2mat(section),...
                    merge_section, mainlane, ramp);
            fclose(fileID_section);
        end
    end
    if(~isempty(data))
        mergesectiondata(i,:)...
            = mean(cell2mat(data),1);
    end
%     mergesectiondata(i,:) = 
        
    %read detector data
%     fileID_detector = fopen(strcat(data_dir,'\',detectorfilenames{1,i}));
%     detector = textscan(fileID_detector,...
%         '%f %f %f %f %f',...
%         'Delimiter',',','EmptyValue',-Inf); 
%     detector_data{i} = cell2mat(detector); 
%     fclose(fileID_detector);
%     mergesectiondata(i,:)...
%        = ProcessSectionData(merging_data{i});
end

%% plot 3d figure
[unique_ramp,~,~] = unique(flows(1,:));
[unique_mainlane,~,~] = unique(flows(2,:));
speed = zeros(length(unique_ramp),length(unique_mainlane));
through = zeros(length(unique_ramp),length(unique_mainlane));
traveltime = zeros(length(unique_ramp),length(unique_mainlane));
index=1;
for i=1:length(unique_ramp)
    ramp = unique_ramp(1, i);
    for j=1:length(unique_mainlane)   
        mainlane = unique_mainlane(1, j);
        [~, col] = find(flows(1,:)== ramp & flows(2,:)== mainlane);
        total_input(index) = mainlane+ramp;
        if ~isempty(col)
            total_through(index) = mergesectiondata(col(1,1), 3);
            speed(i,j) = mergesectiondata(col(1,1), 6);
            through(i,j) = mergesectiondata(col(1,1), 3);
            traveltime(i,j) = mergesectiondata(col(1,1), 4);
        else
            total_through(index) = 0;
            speed(i,j) = 0;
            through(i,j) = 0;
            traveltime(i,j) = 0;            
        end
        index = index +1;
    end
end

cols = find(total_through~=0);
plot(total_input(1,cols), total_through(1,cols), 'ro');

%% plot figures
% figs on_ramp flow
[unique_ramp,~,~] = unique(flows(1,:));
[rows, cols] = size(mergesectiondata);
num_flows = length(unique_ramp);
for i=1:num_flows
    ramp = unique_ramp(1, i);
    [~, col] = find(flows(1,:)== ramp);
    num_col = length(col);    
    data = zeros(num_col, cols);
    through_vol = zeros(num_col, 1);
    for j=1:num_col
        through_vol(j)=flows(2,col(1,j));
        data(j,:) = mergesectiondata(col(1,j),:);
    end
%     plotresult_section(ramp, through_vol, data, 1);
    whole_data{i} = data;
    through_vols{i} = through_vol;
end

plot_by_category(whole_data,3,2,4,1,'Traffic throughput [vph]',[0,9000],unique_ramp,...
    through_vols);
plot_by_category(whole_data,4,2,4,1,'Travel time [s]',[0,300],unique_ramp,...
    through_vols);
plot_by_category(whole_data,6,2,4,1,'Speed [km/h]',[0,120],unique_ramp,...
    through_vols);
% plot_by_category(whole_data,7,2,3,1,'Density [veh/km]',[0,60],unique_ramp,...
%     through_vols);

% figs through flow
whole_data={};
through_vols={};
[unique_ramp,~,~] = unique(flows(2,:));
num_flows = length(unique_ramp);
for i=1:num_flows
    ramp = unique_ramp(1, i);
    [~, col] = find(flows(2,:)== ramp);
    num_col = length(col);
    data = zeros(num_col, cols);
    ramp_vol = zeros(num_col, 1);
    for j=1:num_col
        ramp_vol(j)=flows(1,col(1,j));
        data(j,:) = mergesectiondata(col(1,j),:);
    end
%     plotresult_section(ramp, ramp_vol, data, 0);
    whole_data{i} = data;
    through_vols{i} = ramp_vol;
end

plot_by_category(whole_data,3,6,4,0,'Traffic throughput [vph]',[0,9000],unique_ramp,...
    through_vols);
plot_by_category(whole_data,4,6,4,0,'Travel time [s]',[0,300],unique_ramp,...
    through_vols);
plot_by_category(whole_data,6,6,4,0,'Speed [km/h]',[0,120],unique_ramp,...
    through_vols);
% plot_by_category(whole_data,7,2,3,0,'Density [veh/km]',[0,60],unique_ramp,...
%     through_vols);


