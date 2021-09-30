#pragma once
#include "../Models/Transaction.h"
#include <vector>
#include <memory>

using namespace std;

class TransactionFactory
{
public:
	void ProduceTXs(vector<shared_ptr<Transaction>>& txPool);
};
