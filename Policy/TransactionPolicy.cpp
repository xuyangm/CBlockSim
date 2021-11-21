#include "TransactionPolicy.h"
#include <random>
#include <iostream>
#include <algorithm>

bool PriceCmpDescending(pair<double, int> a, pair<double, int> b) {
	return a.first > b.first;
}

TransactionPolicy::TransactionPolicy()
{
    nextTxn = 0;   
}

void TransactionPolicy::PropagationTxn(double timestamp, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool)
{
    int newLow = nextTxn, newHigh = -1;
	while (txPool[nextTxn]->creationTimestamp <= timestamp) nextTxn++;
    if(newLow == nextTxn) return;
    newHigh = nextTxn-1;
	for (auto iter = nodePool.begin(); iter != nodePool.end(); iter++) {
		if ((*iter)->low == -1 && (*iter)->high == -1) {
			(*iter)->low = newLow;
			(*iter)->high = newHigh;
		}
		else {
			(*iter)->low = min((*iter)->low, newLow);
			(*iter)->high = max((*iter)->high, newHigh);
		}
	}    
}

void TransactionPolicy::SelectTxn(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool)
{
	if(FLAG == 0) LoadBitcoinTxnSelection(e, nodePool, txPool);
    if(FLAG == 1) LoadEthereumTxnSelection(e, nodePool, txPool);
}

void TransactionPolicy::LoadEthereumTxnSelection(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool)
{
	int id;
	double receivingTimestamp;
	int blockLimit = GAS_LIMIT;
	vector<pair<double, int>> pool;

	e->block->gasLimit = 0;
	e->block->size = 0;

    if(nodePool[e->owner]->low != -1) {
        for (int i = nodePool[e->owner]->low; i <= nodePool[e->owner]->high; i++) {
            if (nodePool[e->owner]->txPool.test(i)) {
                if (e->owner != txPool[i]->sender) receivingTimestamp = txPool[i]->creationTimestamp + AVER_DELAY;
                else {
                    receivingTimestamp = txPool[i]->creationTimestamp;
                }
                if (receivingTimestamp <= e->timestamp) {
                    pool.push_back(make_pair(txPool[i]->gasPrice, i));
                }
            }
        }
    }

	sort(pool.begin(), pool.end(), PriceCmpDescending);

	for (auto iter = pool.begin(); iter != pool.end(); iter++) {
		id = iter->second;
		if (blockLimit >= txPool[id]->gasUsed * 1000000000) {
			blockLimit -= txPool[id]->gasUsed * 1000000000;
			e->block->txs.set(id);
			e->block->low = min(id, e->block->low);
			e->block->high = min(id, e->block->high);
			e->block->gasLimit += txPool[id]->gasUsed * 1000000000;
			e->block->size += txPool[id]->size;
		}
	}        
}

void TransactionPolicy::LoadBitcoinTxnSelection(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool)
{
	int id;
	double receivingTimestamp;
	vector<pair<double, int>> pool;

	e->block->size = 0;

    if(nodePool[e->owner]->low != -1) {
        for (int i = nodePool[e->owner]->low; i <= nodePool[e->owner]->high; i++) {
            if (nodePool[e->owner]->txPool.test(i)) {
                if (e->owner != txPool[i]->sender) receivingTimestamp = txPool[i]->creationTimestamp + AVER_DELAY;
                else {
                    receivingTimestamp = txPool[i]->creationTimestamp;
                }
                if (receivingTimestamp <= e->timestamp) {
                    pool.push_back(make_pair(txPool[i]->fee, i));
                }
            }
        }
    }

	sort(pool.begin(), pool.end(), PriceCmpDescending);

	for (auto iter = pool.begin(); iter != pool.end(); iter++) {
		id = iter->second;
		e->block->txs.set(id);
		e->block->low = min(id, e->block->low);
		e->block->high = min(id, e->block->high);
		e->block->size += txPool[id]->size;
	}        
}
