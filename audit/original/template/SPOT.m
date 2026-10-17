function SPOT(action)
%SPOT Open the SPOT Console for this project.
%   SPOT          starts the console (a page in your browser) and connects this MATLAB session
%                 to it; simulations and the console's Command Window run here
%   SPOT open     opens it in the browser again (and prints its address)
%   SPOT stop     stops the console
%
%   The console's settings are kept in spot_settings.json in this folder; every parameter of
%   SPOT, and any value of your own the diagram should read, is in Constants.m (Constants.NAME).
%   Everything the console does is also available as functions, e.g.
%     s = spot.params.load('spot_settings.json');
%     spot.sim.run(s, pwd);
if nargin == 0
    action = 'start';
end
root = fileparts(mfilename('fullpath'));
switch lower(action)
    case 'start'
        spot.console.start(root);
    case 'open'
        spot.console.open(root);
    case 'stop'
        spot.console.stop(root);
    otherwise
        error('Use SPOT, SPOT open or SPOT stop.');
end
end
