plink.exe -ssh pi@192.168.0.102 -pw raspberry command "nohup sudo killall ./ExperimentSoftware/FloatPlatform.elf >/dev/null 2>&1 &"