% CONSOLE  Starting, opening and stopping the SPOT Console (the web page and its server)
%
%   ensureShell     - Make sure MATLAB's system() has a working shell.
%   findPython      - A Python 3.6+ to run the SPOT Console's web server with.
%   open            - Open (or just show the address of) the project's running SPOT Console.
%   openBrowser     - Open a URL in the computer's default web browser.
%   start           - Start the SPOT Console for a project and connect this MATLAB session to it.
%   status          - The project's running SPOT Console (ports and token), or [] if none answers.
%   stop            - Disconnect the MATLAB worker and stop the project's SPOT Console web server.
%   removeOldLibraryPaths - Take old Custom_Library folders off the MATLAB path.
%
%   Not MATLAB, in this package folder too (MATLAB ignores these subfolders):
%     webapp/      the console itself: app.py starts the server (Python 3.6, spot_console/), static/
%                  is the page. SPOT starts it with --local; the lab console (TX2) runs it as a service.
%                  Its Interactive Mode (PhaseSpace tab) flies a platform with the diagram's own
%                  controller, ported to Python in spot_console/control.py (see help spot.control).
%     phasespace/  StreamData.cc, its Makefile and the PhaseSpace library source (libowl-src-410):
%                  the PhaseSpace streamer the lab console compiles and runs (built into output/).
%     deploy/      deploy_tx2.sh (installs the lab console on the TX2 and its service file),
%                  setup_hardware_link.sh and spot_agent.sh (the platforms' side of the Hardware tab
%                  and experiments).
%   Run the scripts with a leading ./ (e.g. ./+spot/+console/deploy/deploy_tx2.sh): bash takes a
%   path starting with + for an option.
