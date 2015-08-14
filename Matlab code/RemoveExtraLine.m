function data = RemoveExtraLine(data)

[row,column] = size(data);

minimum_size = intmax('int32');
for i =1:column
    temp =  data{1, i};
    [temprow, tempcolumn] = size(temp);
    if temprow<minimum_size
        minimum_size = temprow;
    end
end

for i =1:column
    temp =  data{1, i};
    [temprow, tempcolumn] = size(temp);
    if temprow>minimum_size
        data{1, i}(temprow, :)=[];
    end
end