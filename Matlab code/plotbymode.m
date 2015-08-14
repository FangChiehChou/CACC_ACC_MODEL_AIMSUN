function [] = plotbymode(x,y,marker, color, mode,type, ...
    plotmode)

if(type(1,1) == 0)   
    if plotmode == 0
         plot(x,y,'color','k','LineWidth',2);
         return
    else if plotmode == 1
             plot(x,y,'color',color,'LineWidth',2);
             return            
        end
    end
% leader's leader mode
elseif type == -1
     plot(x,y,'color',color,'LineWidth',2);
     return
end
for i=1:length(x)
    if marker == 'o'        
         plot(x(i),y(i),marker,'color',color);
    else
        %maunal mode
        if mode(i) == 0        
            plot(x(i),y(i),'d-','color',color);
        else        
            plot(x(i),y(i),marker,'color',color);
        end
    end
    hold on;
end
