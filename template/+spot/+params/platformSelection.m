function n = platformSelection(p)
%PLATFORMSELECTION The diagrams' platformSelection code for a set of platforms.
%   n = spot.params.platformSelection(s.platforms), where the struct has logical fields
%   RED, BLACK, BLUE and ARM. Errors for combinations the diagrams don't implement.
%
%   0 none            4 RED+BLACK+ARM   8 RED+BLACK+BLUE
%   1 RED             5 RED+ARM         9 BLACK+BLUE
%   2 BLACK           6 BLUE           10 RED+BLUE+ARM
%   3 RED+BLACK       7 RED+BLUE       11 RED+BLACK+BLUE+ARM
key = [logical(p.RED), logical(p.BLACK), logical(p.BLUE), logical(p.ARM)];
codes = [0 0 0 0  0
         1 0 0 0  1
         0 1 0 0  2
         1 1 0 0  3
         1 1 0 1  4
         1 0 0 1  5
         0 0 1 0  6
         1 0 1 0  7
         1 1 1 0  8
         0 1 1 0  9
         1 0 1 1 10
         1 1 1 1 11];
row = find(all(codes(:, 1:4) == key, 2), 1);
if isempty(row)
    names = {'Chaser (RED)', 'Target (BLACK)', 'Obstacle (BLUE)', 'Arm'};
    error('spot:params:platformSelection', ...
        'The combination %s has not been implemented in the diagrams.', strjoin(names(key), ' + '));
end
n = codes(row, 5);
end
