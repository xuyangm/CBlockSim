#include "BlockPolicy.h"
#include <random>
#include <queue>
#include <iostream>

BlockPolicy::BlockPolicy()
{
    nextBKID = 1;
}

void BlockPolicy::GenBlock(int miner, double currentTime, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine)
{
	random_device rd;
	mt19937 gen(rd());
	exponential_distribution<double> GenTime(nodePool[miner]->hashPower / BLOCK_INTERVAL);
    Event e;
	if (nodePool[miner]->hashPower > 0) {
		e.owner = miner;
		e.type = GENERATE_BLOCK;
		e.timestamp = currentTime + GenTime(gen);
		if (e.timestamp <= SIM_TIME) {
			e.block = make_shared<Block>();
			e.block->id = nextBKID++;
			e.block->depth = nodePool[miner]->mainchain.back()->depth + 1;
			e.block->diff = nodePool[miner]->mainchain.back()->diff + 1;
			e.block->miner = miner;
			e.block->prevBlock = nodePool[miner]->mainchain.back()->id;
			e.block->size = BLOCK_SIZE;
			e.block->timestamp = e.timestamp; 
            nodePool[miner]->miningEvent = evEngine.AddEvent(e);
			nodePool[miner]->hasMiningEvent = true;
		}
	}
}

void BlockPolicy::PropagateBlock(shared_ptr<Block> block, double topo[NODES_NUM][NODES_NUM], vector<unique_ptr<Node>> &nodePool, double delays[NODES_NUM])
{
	if(FLAG == 0) LoadBitcoinPropagation(block, topo, nodePool, delays);
    if(FLAG == 1) LoadEthereumPropagation(block, topo, nodePool, delays);
}

void BlockPolicy::LoadBitcoinPropagation(shared_ptr<Block> block, double topo[NODES_NUM][NODES_NUM], vector<unique_ptr<Node>> &nodePool, double delays[NODES_NUM])
{
    srand(time(0));
	vector<int> s;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> u;
	int v = block->miner;
	s.push_back(v);
	for (int i = 0; i < NODES_NUM; i++) {
		if (i == v) delays[i] = 0;
		else if (topo[v][i] != INF) {
			int loc_from = nodePool[v]->location, loc_to = nodePool[i]->location;
			double bandwidth; // in Mb
			if (loc_from == loc_to) bandwidth = min(UPLOAD_BANDWIDTHS[REGIONS_NUM], DOWNLOAD_BANDWIDTHS[REGIONS_NUM]);
			else {
				bandwidth = min(UPLOAD_BANDWIDTHS[loc_from], DOWNLOAD_BANDWIDTHS[loc_to]);
			}
			delays[i] = 3*topo[v][i] + BLOCK_VALIDATION * block->size;
			u.push(make_pair(delays[i], i));
		}
		else {
			delays[i] = INF;
		}
	}
	while (s.size() != NODES_NUM) {
		double dis = u.top().first;
		int update_nid = u.top().second;
		u.pop();
		s.push_back(update_nid);
		for (int i = 0; i < NODES_NUM; i++) {
			if (topo[update_nid][i] != INF) {
				int loc_from = nodePool[update_nid]->location, loc_to = nodePool[i]->location;
				double new_dis;
				new_dis = dis + 3*topo[update_nid][i] + BLOCK_VALIDATION*block->size;
				if (delays[i] > new_dis) {
					delays[i] = new_dis;
					u.push(make_pair(new_dis, i));
				}
			}
		}
	}
	random_device rd;
	mt19937 gen(rd());
	for(int i=0; i<NODES_NUM; i++) {
		exponential_distribution<double> GenTime(1.0 / delays[i]);
		delays[i] = GenTime(gen);
	}
}

void BlockPolicy::LoadEthereumPropagation(shared_ptr<Block> block, double topo[NODES_NUM][NODES_NUM], vector<unique_ptr<Node>> &nodePool, double delays[NODES_NUM])
{
    srand(time(0));
	vector<int> s;
	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> u;
	int v = block->miner;
	s.push_back(v);
	for (int i = 0; i < NODES_NUM; i++) {
		if (i == v) delays[i] = 0;
		else if (topo[v][i] != INF) {
			int loc_from = nodePool[v]->location, loc_to = nodePool[i]->location;
			double bandwidth; // in Mb
			if (loc_from == loc_to) bandwidth = min(UPLOAD_BANDWIDTHS[REGIONS_NUM], DOWNLOAD_BANDWIDTHS[REGIONS_NUM]);
			else {
				bandwidth = min(UPLOAD_BANDWIDTHS[loc_from], DOWNLOAD_BANDWIDTHS[loc_to]);
			}
			if(rand() % nodePool[v]->neighborNum + 1 <= (int)sqrt(nodePool[v]->neighborNum)) delays[i] = topo[v][i] + block->size / bandwidth + BLOCK_VALIDATION * block->size;
			else {
				delays[i] = 5 * topo[v][i] + block->size / bandwidth + 0.5 + BLOCK_VALIDATION * block->size;
			}
			u.push(make_pair(delays[i], i));
		}
		else {
			delays[i] = INF;
		}
	}
	while (s.size() != NODES_NUM) {
		double dis = u.top().first;
		int update_nid = u.top().second;
		u.pop();
		s.push_back(update_nid);
		for (int i = 0; i < NODES_NUM; i++) {
			if (topo[update_nid][i] != INF) {
				int loc_from = nodePool[update_nid]->location, loc_to = nodePool[i]->location;
				double bandwidth; // in Mb
				if (loc_from == loc_to) bandwidth = min(UPLOAD_BANDWIDTHS[REGIONS_NUM], DOWNLOAD_BANDWIDTHS[REGIONS_NUM]);
				else {
					bandwidth = min(UPLOAD_BANDWIDTHS[loc_from], DOWNLOAD_BANDWIDTHS[loc_to]);
				}
				double new_dis;
				if (rand() % nodePool[update_nid]->neighborNum + 1 <= (int)sqrt(nodePool[update_nid]->neighborNum)) new_dis = dis + topo[update_nid][i] + block->size / bandwidth + BLOCK_VALIDATION * block->size;
				else {
					new_dis = dis + 5 * topo[update_nid][i] + block->size / bandwidth + BLOCK_VALIDATION * block->size + 0.5;
				}
				if (delays[i] > new_dis) {
					delays[i] = new_dis;
					u.push(make_pair(new_dis, i));
				}
			}
		}
	}
	random_device rd;
	mt19937 gen(rd());
	for(int i=0; i<NODES_NUM; i++) {
		exponential_distribution<double> GenTime(1.0 / delays[i]);
		delays[i] = GenTime(gen);
	}
}

void BlockPolicy::FinalizeBlock(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine)
{
    if(LONGEST) LoadLongestRule(e, nodePool, evEngine);
	if(GHOST) LoadGHOSTRule(e, nodePool, evEngine);
}

void BlockPolicy::LoadLongestRule(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine)
{
	if (e->block->prevBlock == nodePool[e->owner]->mainchain.back()->id) {
		nodePool[e->owner]->AddBlock(e->block);
		UpdateTransactionPool(e->owner, e->block, nodePool);
		if (nodePool[e->owner]->hasMiningEvent) {
			evEngine.RemoveEvent(nodePool[e->owner]->miningEvent);
			nodePool[e->owner]->hasMiningEvent = false;
		}
		GenBlock(e->owner, e->timestamp, nodePool, evEngine);
		if(FLAG == 1) {
			for (auto iter = e->block->uncles.begin(); iter != e->block->uncles.end(); iter++) {
				nodePool[e->owner]->includedUncles[*iter] = true;
			}
		}
	}
	else if(e->block->depth > nodePool[e->owner]->mainchain.back()->depth) {
		int i, j;

		for (i = nodePool[e->owner]->mainchain.size()-1; i >= 0; i--) {
			if (nodePool[e->owner]->mainchain[i]->id == nodePool[e->block->miner]->mainchain[i]->id) break;
		}
		for (j = i+1; j < nodePool[e->owner]->mainchain.size(); j++) {
			if(FLAG == 1) {
				nodePool[e->owner]->unclechain.push_back(nodePool[e->owner]->mainchain[j]);
				for (auto iter = nodePool[e->owner]->mainchain[j]->uncles.begin(); iter != nodePool[e->owner]->mainchain[j]->uncles.end(); iter++) {
					auto uIter = nodePool[e->owner]->includedUncles.find(*iter);
					if (uIter != nodePool[e->owner]->includedUncles.end()) nodePool[e->owner]->includedUncles.erase(uIter);
				}
				if (!nodePool[e->block->miner]->mainchain[j]->uncles.empty()) {
					for (auto iter = nodePool[e->block->miner]->mainchain[j]->uncles.begin(); iter != nodePool[e->block->miner]->mainchain[j]->uncles.end(); iter++) {
						nodePool[e->owner]->includedUncles[*iter] = true;
					}
				}
			}
			ReleaseTransactions(e->owner, nodePool[e->owner]->mainchain[j], nodePool);
			UpdateTransactionPool(e->owner, nodePool[e->block->miner]->mainchain[j], nodePool);
			nodePool[e->owner]->mainchain[j] = nodePool[e->block->miner]->mainchain[j];
		}
		for(j = nodePool[e->owner]->mainchain.size(); j < nodePool[e->block->miner]->mainchain.size(); j++) {
			nodePool[e->owner]->AddBlock(nodePool[e->block->miner]->mainchain[j]);
			UpdateTransactionPool(e->owner, nodePool[e->block->miner]->mainchain[j], nodePool);
			if (!nodePool[e->block->miner]->mainchain[j]->uncles.empty()) {
				for (auto iter = nodePool[e->block->miner]->mainchain[j]->uncles.begin(); iter != nodePool[e->block->miner]->mainchain[j]->uncles.end(); iter++) {
					nodePool[e->owner]->includedUncles[*iter] = true;
				}
			}
		}
		if (nodePool[e->owner]->hasMiningEvent) {
			evEngine.RemoveEvent(nodePool[e->owner]->miningEvent);
			nodePool[e->owner]->hasMiningEvent = false;
		}
		GenBlock(e->owner, e->timestamp, nodePool, evEngine);
	} else {
		if(FLAG == 1) nodePool[e->owner]->unclechain.push_back(e->block);
	}
}

void BlockPolicy::LoadGHOSTRule(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, EventEngine &evEngine)
{
    if (e->block->prevBlock == nodePool[e->owner]->mainchain.back()->id) {
		nodePool[e->owner]->AddBlock(e->block);
		UpdateTransactionPool(e->owner, e->block, nodePool);
		if (nodePool[e->owner]->hasMiningEvent) {
			evEngine.RemoveEvent(nodePool[e->owner]->miningEvent);
			nodePool[e->owner]->hasMiningEvent = false;
		}
		GenBlock(e->owner, e->timestamp, nodePool, evEngine);
	}
	else {
		if (nodePool[e->owner]->mainchain.back()->diff >= e->block->diff) {
			nodePool[e->owner]->unclechain.push_back(e->block);
		}
		else {
			int i, j;
			for (i = nodePool[e->owner]->mainchain.size()-1; i >= 0; i--) {
				if (i < nodePool[e->block->miner]->mainchain.size()) {
					if (nodePool[e->owner]->mainchain[i]->id == nodePool[e->block->miner]->mainchain[i]->id) break;
				}
			}
			for (j = i+1; j < nodePool[e->owner]->mainchain.size(); j++) {
				nodePool[e->owner]->unclechain.push_back(nodePool[e->owner]->mainchain[j]);
				for (auto iter = nodePool[e->owner]->mainchain[j]->uncles.begin(); iter != nodePool[e->owner]->mainchain[j]->uncles.end(); iter++) {
					auto uIter = nodePool[e->owner]->includedUncles.find(*iter);
					if (uIter != nodePool[e->owner]->includedUncles.end()) nodePool[e->owner]->includedUncles.erase(uIter);
				}
			}
			for (j = i+1; j < nodePool[e->block->miner]->mainchain.size(); j++) {
				if (j < nodePool[e->owner]->mainchain.size()) {
					UpdateTransactionPool(e->owner, nodePool[e->block->miner]->mainchain[j], nodePool);
					nodePool[e->owner]->mainchain[j] = nodePool[e->block->miner]->mainchain[j];
					if (!nodePool[e->block->miner]->mainchain[j]->uncles.empty()) {
						for (auto iter = nodePool[e->block->miner]->mainchain[j]->uncles.begin(); iter != nodePool[e->block->miner]->mainchain[j]->uncles.end(); iter++) {
							nodePool[e->owner]->includedUncles[*iter] = true;
						}
					}
				}
				else {
					UpdateTransactionPool(e->owner, nodePool[e->block->miner]->mainchain[j], nodePool);
					nodePool[e->owner]->AddBlock(nodePool[e->block->miner]->mainchain[j]);
					if (!nodePool[e->block->miner]->mainchain[j]->uncles.empty()) {
						for (auto iter = nodePool[e->block->miner]->mainchain[j]->uncles.begin(); iter != nodePool[e->block->miner]->mainchain[j]->uncles.end(); iter++) {
							nodePool[e->owner]->includedUncles[*iter] = true;
						}
					}
				}
			}
			if (nodePool[e->owner]->hasMiningEvent) {
				evEngine.RemoveEvent(nodePool[e->owner]->miningEvent);
				nodePool[e->owner]->hasMiningEvent = false;
			}
			GenBlock(e->owner, e->timestamp, nodePool, evEngine);
		}
	}
}

void BlockPolicy::UpdateTransactionPool(int nodeID, shared_ptr<Block> block, vector<unique_ptr<Node>> &nodePool)
{
	if(DATA) nodePool[nodeID]->txPool &= (~block->txs);
}

void BlockPolicy::ReleaseTransactions(int nodeID, shared_ptr<Block> block, vector<unique_ptr<Node>> &nodePool)
{
	if(DATA && block->low <= block->high) {
		nodePool[nodeID]->txPool |= block->txs;
		nodePool[nodeID]->low = min(nodePool[nodeID]->low, block->low);
		nodePool[nodeID]->high = max(nodePool[nodeID]->high, block->high);
	}
}