#pragma once
#include "Transaction.h"
#include <vector>
#include <bitset>
#include "../Configs.h"

using namespace std;

class Block
{
public:
	int id;
	int depth;
	int diff;
	int prevBlock;
	int miner;
	double timestamp;
	double size;
	int low, high;
	bitset<TX_NUM> txs;
	vector<int> uncles;

	int gasLimit;

	Block();
};