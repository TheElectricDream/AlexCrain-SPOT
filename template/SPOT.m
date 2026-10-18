function SPOT(action)
%SPOT Open the SPOT Console for this project.
%   SPOT          starts the console (a page in your browser) and connects this MATLAB session
%                 to it; simulations and the console's Command Window run here
%   SPOT open     opens it in the browser again (and prints its address)
%   SPOT stop     stops the console
%
%   Everything this project chooses lives in Constants.m, next to this file (Constants.NAME):
%   the console's settings, the Designer's choices and the lab calibration it took, with every
%   parameter of SPOT. The console writes it; edit it by hand too. SPOT shares this MATLAB
%   session so the console's Python can read it (the MATLAB Engine for Python).
%   Everything the console does is also available as functions, e.g.
%     s = spot.params.fromConstants();
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
