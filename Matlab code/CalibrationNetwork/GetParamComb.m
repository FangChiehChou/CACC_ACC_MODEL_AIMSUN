function res = GetParamComb(params)

tempres = [];
res = [];
[tempres, res] = dfs(params, res, tempres, 1);

end

function [tempres, res, index] = ...
    dfs(params, res, tempres, index)

[~, cols] = size(params);

if length(tempres) == cols
    [row,~] = size(res);
    res(row+1, :) = tempres;
else
    values = params(1, index).values;
    for i=1:length(values)
        tempres(index) = values(1, i);    
        [tempres, res] = dfs(params, res, tempres, index+1);
        tempres(index) = [];
    end
end    
end