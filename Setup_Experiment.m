% The following script will create a new SPOT experiment.

function [] = Setup_Experiment()

clear;
clc;
close all;

expName = inputdlg('Please enter the name of the experiment (no spaces):');

if isempty(expName)==1
    fprintf("No name entered...\n");
else
    copyfile([cd '\Template_Files\Version 3.08-beta.2\'],...
    [cd '\Projects\' expName{1,1}]);

    cd([cd '\Projects\' expName{1,1}]);

    movefile('Template_v3_08_2020a.slx',[expName{1,1} '.slx']);
end



end