function [ output_args ] = GEH( E, P )
%GEH Summary of this function goes here
%   Detailed explanation goes here
    
    sub = E - P;
    add = E+P;
    output_args = mean(sqrt(2*sub.^2./add));

end

