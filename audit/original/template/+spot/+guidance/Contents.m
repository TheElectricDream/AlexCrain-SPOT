% GUIDANCE  Path planners: the setpoint each controller tracks, from the goal
%
%   defaultPlanner - Move toward the goal at a capped speed, then settle on it.
%   directPlanner  - No path planning: the goal is the setpoint, moving as the goal moves.
%   customPlanner  - User path planner: turns the goal into the setpoint the controller tracks.
