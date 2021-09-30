#pragma once
#include "Models/Event.h"
#include "Models/Node.h"
#include "Configs.h"
#include "Factories/TransactionFactory.h"
#include "Factories/TopologyFactory.h"
#include "Policy/TransactionPolicy.h"
#include "Policy/BlockPolicy.h"
#include "EventEngine.h"
#include <set>

using namespace std;

class Scheduler
{
public:

	double halfBPD;
	double ninetyBPD;
	int count;

	int totalGenBlocks;
	int globalchain;
	TransactionFactory txGenerator;
	TopologyFactory topoGenerator;
	TransactionPolicy txnPolicy;
	BlockPolicy bkPolicy;
	EventEngine evEngine;
	
	double topo[NODES_NUM][NODES_NUM];
	vector<shared_ptr<Transaction>> txPool;
	vector<unique_ptr<Node>> nodePool;
	map<int, shared_ptr<Block>> blockPool;

	Scheduler();

	void GenerateInitialEvents();
	void HandleEvent(multiset<Event>::iterator e);

	void GenerateBlock(multiset<Event>::iterator e);
	void ReceiveBlock(multiset<Event>::iterator e);

	void GenBlock(int miner, double currentTime);
	void PropagateBlock(shared_ptr<Block> block);
	void ResolveFork();
	void DistributeRewards();
	double ShowStatistics();
};