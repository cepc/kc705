# EUDAQ with JadePix Run

## ENV setting

```
export EUDAQ_MODULE_DIR="/home/cepcdaq/JadePix/kc705/lib"
export PATH="/home/cepcdaq/JadePix/eudaq/bin:${PATH}"
export PATH="/home/cepcdaq/JadePix/kc705/bin:${PATH}"
``

## Running process

```
euCliProducer -n JadeProducer -t jade
euCliProducer -n TluProducer -t tlu
euCliCollector -n TriggerIDSyncDataCollector -t tgid
StdEventMonitor -r tcp://localhost:44000 -t my_mon
```
