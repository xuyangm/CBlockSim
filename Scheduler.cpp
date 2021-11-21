#include "Scheduler.h"
#include <random>
#include <map>
#include <iostream>
#include <queue>
#include <algorithm>
#include <string.h>


Scheduler::Scheduler()
{
	halfBPD = 0;
	ninetyBPD = 0;

	globalchain = -1;
	totalGenBlocks = 1;
	count = 0;

	if(!NETWORK) topoGenerator.ProduceCompleteTopo(topo);
	if(DATA) txGenerator.ProduceTXs(txPool);
	if(NETWORK) topoGenerator.ProduceSmallWorldTopo(MAX_DISTANCE, BETA, DENSITY, topo);
	topoGenerator.ProduceNodesByDistrib(nodePool, topo);
}

void Scheduler::GenerateInitialEvents()
{
	for (int i = 0; i < NODES_NUM; i++) GenBlock(i, 0);
}

void Scheduler::HandleEvent(multiset<Event>::iterator e)
{
	if (e->type == GENERATE_BLOCK) GenerateBlock(e);
	else if (e->type == RECEIVE_BLOCK) ReceiveBlock(e);
}

void Scheduler::GenerateBlock(multiset<Event>::iterator e)
{
	if (e->block->prevBlock == nodePool[e->owner]->mainchain.back()->id) {
		if(DATA) {
			txnPolicy.PropagationTxn(e->timestamp, nodePool, txPool);
			txnPolicy.SelectTxn(e, nodePool, txPool);
		}
		if(FLAG == 1) nodePool[e->owner]->AddUncles(e->block);
		nodePool[e->owner]->AddBlock(e->block);
		bkPolicy.UpdateTransactionPool(e->owner, e->block, nodePool);
		PropagateBlock(e->block);
		nodePool[e->owner]->hasMiningEvent = false;
		GenBlock(e->owner, e->timestamp);
		totalGenBlocks++;
		if(e->timestamp > 0) blockPool[e->block->id] = e->block;
	}
}

void Scheduler::ReceiveBlock(multiset<Event>::iterator e)
{
	bkPolicy.FinalizeBlock(e, nodePool, evEngine);
}

void Scheduler::GenBlock(int miner, double currentTime)
{
	bkPolicy.GenBlock(miner, currentTime, nodePool, evEngine);
}

void Scheduler::PropagateBlock(shared_ptr<Block> block)
{
	random_device rd;
	mt19937 gen(rd());
	exponential_distribution<double> GenTime(1.0 / AVER_DELAY);
	double delay;
	double delays[NODES_NUM];
	if(NETWORK) bkPolicy.PropagateBlock(block, topo, nodePool, delays);
	
	vector<double> bpd;
	for (int i = 0; i < nodePool.size(); i++) {
		if (block->miner == nodePool[i]->id) continue;
		Event e;
		e.block = block;
		e.owner = nodePool[i]->id;
		if(!NETWORK) delay = GenTime(gen);
		if(NETWORK) delay = delays[i];
		bpd.push_back(delay);
		e.timestamp = block->timestamp + delay;
		e.type = RECEIVE_BLOCK;
		evEngine.AddEvent(e);
	}
	sort(bpd.begin(), bpd.end());
	if((nodePool.size()-1)%2 == 0) {
		halfBPD += (bpd[(nodePool.size()-1)/2-1] + bpd[(nodePool.size()-1)/2])/2;
	} else {
		halfBPD += bpd[(int)((nodePool.size()-1)/2)];
	}
	ninetyBPD += bpd[(int)((nodePool.size()-1)*0.9)];
	count++;
}

void Scheduler::ResolveFork()
{
	if(FINALIZE_POLICY == 0) {
		map<int, int> counter;
		int maxDepth = 0;
		for (int i = 0; i < NODES_NUM; i++) {
			if (nodePool[i]->mainchain.back()->depth > maxDepth) maxDepth = nodePool[i]->mainchain.back()->depth;
		}
		for (int i = 0; i < NODES_NUM; i++) {
			if (nodePool[i]->mainchain.back()->depth == maxDepth) {
				if (counter.find(nodePool[i]->mainchain.back()->miner) == counter.end()) counter[nodePool[i]->mainchain.back()->miner] = 1;
				else {
					counter[nodePool[i]->mainchain.back()->miner]++;
				}
			}
		}
		int num = 0, nid;
		for (auto iter = counter.begin(); iter != counter.end(); iter++) {
			if (iter->second > num) {
				num = iter->second;
				nid = iter->first;
			}
		}
		for (int i = 0; i < NODES_NUM; i++) {
			if (nodePool[i]->mainchain.back()->depth == maxDepth && nodePool[i]->mainchain.back()->miner == nid) {
				globalchain = i;
				break;
			}
		}
	} else if(FINALIZE_POLICY == 1) {
		map<int, int> counter;
		int maxDiff = 0;
		for (int i = 0; i < NODES_NUM; i++) {
			if (nodePool[i]->mainchain.back()->diff > maxDiff) maxDiff = nodePool[i]->mainchain.back()->diff;
		}
		for (int i = 0; i < NODES_NUM; i++) {
			if (nodePool[i]->mainchain.back()->diff == maxDiff) {
				if (counter.find(nodePool[i]->mainchain.back()->miner) == counter.end()) counter[nodePool[i]->mainchain.back()->miner] = 1;
				else {
					counter[nodePool[i]->mainchain.back()->miner]++;
				}
			}
		}
		int num = 0, nid;
		for (auto iter = counter.begin(); iter != counter.end(); iter++) {
			if (iter->second > num) {
				num = iter->second;
				nid = iter->first;
			}
		}
		for (int i = 0; i < NODES_NUM; i++) {
			if (nodePool[i]->mainchain.back()->diff == maxDiff && nodePool[i]->mainchain.back()->miner == nid) {
				globalchain = i;
				break;
			}
		}
	}
}

void Scheduler::DistributeRewards()
{
	for(auto iter=nodePool[globalchain]->mainchain.begin()+1; iter != nodePool[globalchain]->mainchain.end(); iter++) {
		nodePool[(*iter)->miner]->balance += BLOCK_REWARD;
		if(DATA) {
			for(int i=(*iter)->low; i<=(*iter)->high; i++) {
				if((*iter)->txs.test(i)) nodePool[(*iter)->miner]->balance += txPool[i]->fee;
			}
			if(FLAG == 1) {
				nodePool[(*iter)->miner]->balance += (*iter)->uncles.size() * UNCLE_REWARD;
				for(int i=0; i<(*iter)->uncles.size(); i++) {
					int uncleDepth = blockPool[(*iter)->uncles[i]]->depth;
					int blockDepth = (*iter)->depth;
					nodePool[blockPool[(*iter)->uncles[i]]->miner]->balance += ((uncleDepth - blockDepth + 8) * BLOCK_REWARD / 8);
				}
			}
		}
	}
}

double Scheduler::ShowStatistics(int rd, double t)
{
	double res;
	set<int> check;
	int mainBlocks = nodePool[globalchain]->mainchain.size();
	int uncleBlocks = 0;
	double averBlockSize = 0;
	for(auto iter = nodePool[globalchain]->mainchain.begin(); iter != nodePool[globalchain]->mainchain.end(); iter++) {
		uncleBlocks += (*iter)->uncles.size();
		averBlockSize += (*iter)->size;
	}
	int staleBlocks = totalGenBlocks - mainBlocks;
	double uncleRate = (double)uncleBlocks / (mainBlocks+uncleBlocks);
	double staleRate = (double)staleBlocks / totalGenBlocks;
	averBlockSize /= mainBlocks;
	if(FLAG == 0) st.write(rd, t, (double)SIM_TIME/totalGenBlocks, averBlockSize, mainBlocks, staleBlocks, halfBPD/count, ninetyBPD/count, staleRate);
	else {
		st.write(rd, t, (double)SIM_TIME/totalGenBlocks, averBlockSize, mainBlocks, uncleBlocks, halfBPD/count, ninetyBPD/count, uncleRate);
	}
	return res;
}

