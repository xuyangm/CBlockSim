#pragma once

#include "../Models/Node.h"
#include "../Models/Transaction.h"
#include "../Models/Event.h"
#include <vector>
#include <set>

using namespace std;

class TransactionPolicy
{
public:
    int nextTxn;

	void PropagationTxn(double timestamp, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool);
    void SelectTxn(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool);
    void LoadEthereumTxnSelection(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool);
    void LoadBitcoinTxnSelection(multiset<Event>::iterator e, vector<unique_ptr<Node>> &nodePool, vector<shared_ptr<Transaction>> &txPool);

    TransactionPolicy();
};