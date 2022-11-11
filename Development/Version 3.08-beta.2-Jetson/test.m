fid = fopen([cd '\Batch_Scripts\CopyDirToWinAuto.bat'],'wt');

fprintf(fid, 'cd C:\\Program Files\\PuTTY \n');
FileStorage = [cd '\Pi_Files'];
test = strrep(FileStorage,'\','\\');
fprintf(fid, ['pscp.exe -pw srcl2022 -r blue-spot@192.168.0.107:/home/blue-spot/ExperimentSoftware/* ' test '\n']);
fclose(fid);