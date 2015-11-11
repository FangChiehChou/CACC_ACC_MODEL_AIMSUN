function data = ReadDetector(values,rep_id, parent)
    
    [~, col] = size(values);
    folder = '';
    for i=1:col
        tempstr = num2str(values(1, i));
        tempstr = strrep(tempstr, '.', '_');
        folder = strcat(folder, tempstr);
    end
    
    folder = strcat(parent, num2str(rep_id), '\\detector\\', folder );
    filename = strcat(folder, '\\detector_run.txt');
%     filename = strcat(parent, '\\detector_run.txt');
    fileID_merge = fopen(filename);
    sim_data = textscan(fileID_merge,...
        '%f %f %f %f %f %f %f',...
        'Delimiter',',','EmptyValue',-Inf); 
    data = cell2mat(sim_data); fclose(fileID_merge);
        
end