#!../../bin/darwin-aarch64/sysmon
epicsEnvSet("IOC","iocsysmon")
epicsEnvSet("TOP","../..")
cd "$(TOP)"

## Register all support components
dbLoadDatabase "dbd/sysmon.dbd"
sysmon_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords "db/sysmon.db"
dbLoadRecords "sysmonApp/Db/sysmon.db"

iocInit
