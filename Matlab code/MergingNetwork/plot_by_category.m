function  [] = plot_by_category(whole_data, column_index, row, col, isramp...
    ,ylab, ylimit, volumes,through_vol)

xlab = 'Ramp volume [veh/hr]';
suffix =  'Through traffic volume = ';
if isramp == 1
    xlab = 'Through traffic volume (veh per hour)';
    suffix =  'Ramp volume = ';
end

i=1;
j=1;
figure;
for k=1:row*col
    if i>row
        i=1;
        j=j+1;
    end
    subplot(row,col, k);
    plot(through_vol{k}, whole_data{k}(:, column_index), 'ro','LineWidth', 2); hold on;
    if ~isempty(strfind(cellstr(ylab),'Throughput'))
%         plot(cell2mat(through_vol),cell2mat(through_vol)+volumes(1,k),'b','LineWidth', 2);
    end
    hold off; 
    titlestr = strcat(suffix, int2str(volumes(1,k)),' veh/hr');
    xlabel(xlab);
    ylabel(ylab);
    ylim(ylimit);
    title(titlestr);
    i=i+1;
end