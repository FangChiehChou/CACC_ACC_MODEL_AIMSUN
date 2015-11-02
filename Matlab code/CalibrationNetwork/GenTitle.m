function title = GenTitle(values, params)

    [~, col] = size(values);
    title = '';
    for i=1:col
        temp = strrep(params(1, i).name, 'sce_','');
        temp = strrep(temp, 'exp_','');
        temp = strrep(temp, 'car_','');
        temp = strrep(temp, '_', ' ');
        title = strcat(title, temp, ': ', num2str(values(1, i)), '; ');
    end

end