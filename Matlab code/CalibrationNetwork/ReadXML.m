function params = ReadXML(filename)

doc = parseXML(filename);
[~, cols] = size(doc.Children);

% get the number of parameters
params = param.empty;
index=1;
for i = 1:cols
    value = doc.Children(1, i);
    if strcmp(value.Name, 'Range') == 1
        temp_param = param();
        temp_param.name = value.Children(1, 2).Children.Data;
        temp_param.high =str2double(...
            value.Children(1, 4).Children.Data);
        temp_param.low =str2double(...
            value.Children(1, 6).Children.Data);
        temp_param.increment =str2double(...
            value.Children(1, 8).Children.Data);
        
        h_index =int32(double(temp_param.high-temp_param.low)...
            /temp_param.increment);
        temp_param.values(1)  = temp_param.low;
        for j = 1:h_index
            inc =double(temp_param.increment*double(j));
            temp_param.values(j+1) = temp_param.low + inc;
        end
    
        params(1, index) = temp_param;
        index = index + 1;
    end
end





