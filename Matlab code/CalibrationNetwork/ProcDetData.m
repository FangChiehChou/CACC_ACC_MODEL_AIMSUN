function [ ] = ProcDetData(sim_data, simu_ids, field_ids,...
    start_row, figure_title, fielddata)

load(fielddata);
[cellrow, cellcol] = size(simu_ids);
index = 1;
for j=1:cellcol
    for k=1:cellrow
        vals = simu_ids{k, j};
        [~,valcol] = size(vals);
        for m=1:valcol
            if vals(1, m) ~=0
                data = sim_data(sim_data(:, 2) == vals(1, m),:);
                sim_data_key(index) = vals(1, m);
                sim_data_speed{index}= data(:, 5)*0.621371;
                sim_data_flow{index}= data(:, 4)*12;
                end_row = length(data(:,4))+start_row-1;

                var_name = strcat('flow_', int2str(field_ids(1, j)));
                eval(strcat('tempdata = ', var_name, '(start_row:end_row, m+1);'));                    
                field_data_flow{index}= tempdata*12;    
                var_name = strcat('speed_', int2str(field_ids(1, j)));      
                eval(strcat('tempdata = ', var_name, '(start_row:end_row, m+1);'));           
                field_data_speed{index}= tempdata;

                index = index+1;
            end
        end
    end
end    

simu_speed_pair = containers.Map(sim_data_key,sim_data_speed);
simu_flow_pair = containers.Map(sim_data_key,sim_data_flow);
field_speed_pair = containers.Map(sim_data_key,field_data_speed);
field_flow_pair = containers.Map(sim_data_key,field_data_flow);

% then plot
[~, key_col] = size(keys(simu_speed_pair));
det_ids_key = keys(simu_speed_pair);

for j=1:key_col
    key = det_ids_key{1, j};
    temp = simu_speed_pair(key);
    temp1 = field_speed_pair(key);
    plotspeed(temp, temp1, key, 3*j, figure_title);

    temp = simu_flow_pair(key);
    temp1 = field_flow_pair(key);
    plotflow(temp, temp1, key, 3*j+1, figure_title);
    
    plotcumulativeflow(temp/12, temp1/12, key, 3*j+2, figure_title);
end
end

