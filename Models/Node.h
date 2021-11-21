#pragma once
#include "Block.h"
#include "Event.h"
#include <set>
#include <bitset>
#include <map>
#include <memory>

using namespace std;

class Node
{
public:
	int id;
	int location;
	int neighborNum;
	double hashPower;
	double stake;
	double stakeTime;
	double balance;
	int high, low;
	bitset<TX_NUM> txPool;
	vector<shared_ptr<Block>> mainchain;
	vector<shared_ptr<Block>> unclechain;
	map<int, bool> includedUncles;
	bool hasMiningEvent;
	multiset<Event>::iterator miningEvent;

	Node();
	void AddBlock(shared_ptr<Block> block);
	void AddUncles(shared_ptr<Block> block);
};