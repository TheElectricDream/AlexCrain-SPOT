plink.exe -ssh pi@192.168.0.100 -pw raspberry command "nohup sudo killall ./ExperimentSoftware/CleanShutdown.elf >/dev/null 2>&1 &"