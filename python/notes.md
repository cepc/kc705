# Notes

    CMOS <----> FPGA <------> PC
                       PCIe

### Questions:
- Protocol? It seems we read/write a stream from/to the Xillibus IP core.
- Is all communication over this stream?
- What commands are there?
- How do events come out? What format are they in? 
- Can we just read events in a loop, and it will wait if there is no new data? Or do we have to synchronize to a clock, and read every x ms? A quick skimming of Xillibus documentation seems to say it will block.

### Operations:
- Do we flash the FPGA from the GUI? or just configure some bits?
- List of things we have to send over... thresholds etc.

### Functions:
- Configure FPGA
- Start / record run
- Display channels / hitmap
  - Accumulated over time
- List saved runs
- Load run config from file
  - Load next file when reached number of events

I'm taking a bit of inspiration from eudaq Run Control, which is fairly simple and stable, and including additional functions as needed.

### Run format:
- Run files should have metadata header: time, settings
- Do we need to write out some clock, e.g. BCID, to synchronize?

### Plots:
- Sensor display
  What is interesting? One sector, or whole matrix at a time?
  - Matrix I: 10x2 sectors, each 16x48 pixels
  - Matrix II: 8x2 sectors, each 16x96 pixels

### Implementation:
- Make reading from / writing to Xillibus and files not block the UI
  - Asynchronous IO in QT?
  - IO on background thread?
- Use QT Widgets or QT Quick? Widgets probably more suited
- Unit testing
  - Important if we want to maintain this for a long time

