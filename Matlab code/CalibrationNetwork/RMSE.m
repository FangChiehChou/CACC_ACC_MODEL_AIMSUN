function [ output_args ] = RMSE( E, P )
%RMSE Summary of this function goes here
%   Detailed explanation goes here

    I = ~isnan(E) & ~isnan(P); 
    E = E(I); P = P(I);

    output_args=sqrt(sum((E(:)-P(:)).^2)/numel(E));

end

