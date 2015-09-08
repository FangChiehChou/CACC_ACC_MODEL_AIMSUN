%% This file reads the results from the batch simulation
%    and plot the figures compared to HCM method
%    author: Dali Wei
%    create date: 09/07/2015

%% first define paramters
clear all;
rid = 461;%replication id
acc=0;
cacc=0;
data_dir = strcat('C:\CACC_Simu_Data\acc',int2str(acc),...
    '_cacc',int2str(cacc),'\',int2str(rid),'\detector\');
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
mergesectiondata=zeros(flowindex, 4);
for i=1:flowindex
    % read merging area data
    fileID_merge = fopen(strcat(data_dir,'\',mergefilenames{1,i}));
    merging_area = textscan(fileID_merge,...
        '%f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf); 
    merging_data{i} = cell2mat(merging_area);
    fclose(fileID_merge);
    mergesectiondata(i,:)...
       = ProcessSectionData(merging_data{i});
    
    %read section data
%     fileID_section = fopen(strcat(data_dir,'\',sectionfilenames{1,i}));
%     section = textscan(fileID_section,...
%         '%f %f %f %f %f',...
%         'Delimiter',',','EmptyValue',-Inf); 
%     section_data{i} = cell2mat(section); 
%     fclose(fileID_section);
%     mergesectiondata(i,:)...
%        = ProcessSectionData(merging_data{i});
        
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

%% plot figures
% figs on_ramp flow
[unique_ramp,~,~] = unique(flows(1,:));
num_flows = length(unique_ramp);
for i=1:num_flows
    ramp = unique_ramp(1, i);
    [~, col] = find(flows(1,:)== ramp);
    num_col = len(col);
    data = zeros(num_col, 4);
    through_vol = zeros(num_col, 4);
    for j=1:num_col
        through_vol(j)=flows(2,col(1,j));
        data(j,:) = mergesectiondata(col(1,j),:);
    end
    plotresult(ramp, through_vol, data, 1);
end

% figs through flow
[unique_ramp,~,~] = unique(flows(2,:));
num_flows = length(unique_ramp);
for i=1:num_flows
    ramp = unique_ramp(2, i);
    [~, col] = find(flows(2,:)== ramp);
    num_col = len(col);
    data = zeros(num_col, 4);
    ramp_vol = zeros(num_col, 4);
    for j=1:num_col
        ramp_vol(j)=flows(2,col(1,j));
        data(j,:) = mergesectiondata(col(1,j),:);
    end
    plotresult(ramp, ramp_vol, data, 0);
end


