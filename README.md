# A High-Performance Blockchain Simulator</br></br>

## Introduction</br>
CBlockSim is an open source blockchain simulator extended from BlockSim: Alharby. We migrate BlockSim from Python to C++. Using bitwise operation supported by C++, we speed up the simulation process a lot. Moreover, we add a module to simulate the real network so that the simulator can be used to do experiments related to the overlay network of blockchain, for example testing gossip protocols of Bitcoin or Ethereum.</br></br>

## Configure</br>
Users can configure blockchain via editing Config.h and Config.cpp</br></br>

## Modules</br>
Users can modify some components of blockchain easily. Modules can be found in Policy folder and Factories folder.</br>
The Policy folder contains two types of policies now, one for block and another one for transaction.</br>
The Factories folder contains topology generation and transaction generation.</br></br>

### 1. Block Policy: BlockPolicy.h, BlockPolicy.cpp</br>
#### (1) *LoadPoW* and *LoadPoS*</br>
Describe how to simulate block generation，corresponding to Block Proposal in paper.<br>
PoW and a simple PoS have been included, both using exponential distribution.<br>
#### (2) *LoadBitcoinPropagation* and *LoadEthereumPropagation*</br>
Describe block propagation protocol, corresponding to Information Propagation in paper.<br>
The protocols of Bitcoin, i.e. CBR, and Ethereum, i.e. Eth Wire Protocol have been simulated. Users can add new function to implement different propagation protocols.<br>
#### (3) *LoadLongestRule* and *LoadGHOSTRule* </br>
Describe how to finalize blockchain, corresponding to Block Finalization in paper. </br>
The longest rule and GHOST rule have been included. Users can add new function to create new finalization rule.<br><br>

### 2. Transaction Policy: TransactionPolicy.h, TransactionPolicy.cpp</br>
#### (1) *PropagationTxn*</br>
Describe how to propagate transactions.<br>
#### (2) *LoadEthereumTxnSelection* and *LoadBitcoinTxnSelection*</br>
Describe how each miner selects transactions to include in the new block.</br>
Miners select transaction based on gas price in Ethereum and based on transaction fee in Bitcoin. </br>
#### (3) *LoadLongestRule* and *LoadGHOSTRule* </br>
Describe how to finalize blockchain, corresponding to Block Finalization in paper. </br>
The longest rule and GHOST rule have been included. Users can add new function to create new finalization rule.</br>

### 3. Topology Generation: TopologyFactory.h, TopologyFactory.cpp</br>

#### (1) *ProduceCompleteTopo*</br>
Generate a complete graph topology.<br>
#### (2) *ProduceSmallWorldTopo*</br>
Generate a small-world topology. See reference "Simple, distance-dependent formulation of the Watts-Strogatz model for directed and undirected small-world networks".
<br>
### 4. Transaction Generation: TransactionFactory.h, TransactionFactory.cpp</br>

Generate transactions based on average values, i.e. gas limit, used gas and so on, observed in blockchain. Users can modify this part to add more randomness or build transaction based on real transaction data. <br><br>

## How to use</br>
cd build/<br>
bash start.sh</br></br>

## Result </br>
see build/output.csv

