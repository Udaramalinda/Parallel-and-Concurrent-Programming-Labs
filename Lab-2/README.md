# Project Description

This project simulates a bus stop scenario. The simulation involves passengers arriving at a bus stop, boarding buses, and buses departing once they are full or all eligible passengers have boarded.

# Requirements

- Java installed on your system (version 8 or above).
- (If running python version) Python 3 installed for running the Python version of the simulation.

# How to Build and Run the Simulation

## Running the Java Simulation

The simulation can be run directly with the following command.

```bash
make runjava
```

Note that the makefile uses the default parameters of (for convenience with quick trial runs):
- 101 passengers
- bus capacity of 10
- passenger arrival mean time interval of 0.2
- bus arrival mean time interval of 2.0 

To change these parameters, you can modify the Makefile.

## Running the Python Simulation

The Python version of the simulation can be run with the following command.

```bash
make runpython
```