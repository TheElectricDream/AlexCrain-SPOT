function [name, color] = platformStyle(key)
    %PLATFORMSTYLE What a platform is called and its colour in figures.
    %   [name, color] = spot.plotting.platformStyle('RED') returns 'Chaser' and its RGB colour.
    %   The keys stay RED, BLACK and BLUE (and ARM) in the code, the diagram and the data.
    %
    %   Inputs
    %     key    'RED', 'BLACK', 'BLUE' or 'ARM' (any case); anything else is returned as is
    %
    %   Outputs
    %     name   'Chaser' (silver, drawn mid grey so it shows on white), 'Target' (gold),
    %            'Obstacle' (blue), 'Arm' (the Chaser's colour), or the key (black)
    %     color  1x3 RGB
    %
    %   See also spot.data.toFigure.

    switch upper(key)
        case 'RED'
            name = 'Chaser';
            color = [0.45 0.48 0.52];
        case 'BLACK'
            name = 'Target';
            color = [0.80 0.58 0.05];
        case 'BLUE'
            name = 'Obstacle';
            color = [0 0 1];
        case 'ARM'
            name = 'Arm';
            color = [0.45 0.48 0.52];
        otherwise
            name = key;
            color = [0 0 0];
    end
end
