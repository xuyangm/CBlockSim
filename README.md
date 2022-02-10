A High-Performance Blockchain Simulator
Introduction
CBlockSim is an open source blockchain simulator extended from BlockSim: Alharby. We migrate BlockSim from Python to C++. Using bitwise operation supported by C++, we speed up the simulation process a lot. Moreover, we add a module to simulate the real network so that the simulator can be used to do experiments related to the overlay network of blockchain, for example testing gossip protocols of Bitcoin or Ethereum.

cd build/<br>
bash start.sh
