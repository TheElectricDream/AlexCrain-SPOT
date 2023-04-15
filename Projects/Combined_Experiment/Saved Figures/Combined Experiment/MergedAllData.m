clear;
clc;
close('all');

% Load in data files:

RED   = importdata([cd '\ExperimentData_RED_2023_4_8_10_3\ExpLog']);
BLUE  = importdata([cd '\ExperimentData_BLUE_2023_4_8_10_3\ExpLog']);

% Align time vectors using timeseries:

TimeSeriesRED   = timeseries(RED(:,2:end),RED(:,1));
TimeSeriesBLUE  = timeseries(BLUE(:,2:end),BLUE(:,1));
 
[TimeSeriesRED,TimeSeriesBLUE] = synchronize(TimeSeriesRED,TimeSeriesBLUE,'intersection'); 

% Loop through all arrays. If a column average is exactly 0, merged:

for i = 1:size(TimeSeriesRED.data,2)
   
    ColumnMean = mean(TimeSeriesRED.data(:,i));
    
    if ColumnMean == 0
        
        fprintf(['Column ' num2str(i) ' is empty.\n']);
        
        ColumnMeanBLUE  = mean(TimeSeriesBLUE.data(:,i));
            
        if ColumnMeanBLUE ~= 0
                
            TimeSeriesRED.data(:,i) = TimeSeriesBLUE.data(:,i);  
            
        else
            
            % do nothing
            
        end
                     
        
    end
       
end

dataClass = ApplyDataDictionaryCE([TimeSeriesRED.time, TimeSeriesRED.data]);

save([cd '\ExperimentData_CombinedExperiment_Merged_2023_04_08.mat'],'dataClass');