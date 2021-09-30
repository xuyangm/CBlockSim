#include "TransactionFactory.h"
#include "../Configs.h"
#include <stdlib.h>
#include <random>
#include <ctime>
#include <algorithm>

bool TimeCmpAscending(shared_ptr<Transaction> a, shared_ptr<Transaction> b) {
	return a->creationTimestamp < b->creationTimestamp;
}

void TransactionFactory::ProduceTXs(vector<shared_ptr<Transaction>>& txPool)
{
	srand((unsigned int)time(0));
	for (int i = 0; i < TX_NUM; i++) {
		auto tx = make_shared<Transaction>();
		tx->creationTimestamp = (double)(round(1.0 * rand() / RAND_MAX * SIM_TIME));
		tx->sender = (int)(round(1.0 * rand() / RAND_MAX * NODES_NUM));
		tx->size = TX_SIZE;
		tx->fee = USED_GAS * GAS_PRICE;
		tx->gasLimit = GAS_LIMIT;
		tx->gasPrice = GAS_PRICE;
		tx->gasUsed = USED_GAS;
		txPool.push_back(tx);
	}
	sort(txPool.begin(), txPool.end(), TimeCmpAscending);
	for (int i = 0; i < txPool.size(); i++) {
		txPool[i]->id = i;
	}
}
