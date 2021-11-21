#pragma once
#include "../Models/Node.h"
#include "../Models/Event.h"
#include "../Models/Block.h"
#include "../EventEngine.h"

using namespace std;

class BlockPolicy
{
public:
    int nextBKID;

    void GenBlock(int miner, double currentTime, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine);
    void LoadPoW(int miner, double currentTime, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine);
    void LoadPoS(int miner, double currentTime, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine);

    void PropagateBlock(shared_ptr<Block> block, double topo[NODES_NUM][NODES_NUM], vector<unique_ptr<Node>> &nodePool, double delays[NODES_NUM]);
    void LoadBitcoinPropagation(shared_ptr<Block> block, double topo[NODES_NUM][NODES_NUM], vector<unique_ptr<Node>> &nodePool, double delays[NODES_NUM]);
    void LoadEthereumPropagation(shared_ptr<Block> block, double topo[NODES_NUM][NODES_NUM], vector<unique_ptr<Node>> &nodePool, double delays[NODES_NUM]);
    
    void FinalizeBlock(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine);
    void LoadLongestRule(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine);
    void LoadGHOSTRule(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine);

    void UpdateTransactionPool(int nodeID, shared_ptr<Block> block, vector<unique_ptr<Node>> &nodePool);
    void ReleaseTransactions(int nodeID, shared_ptr<Block> block, vector<unique_ptr<Node>> &nodePool);

    BlockPolicy();
};