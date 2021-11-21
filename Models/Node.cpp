#include "Node.h"
#include <algorithm>
#include "../Configs.h"

Node::Node()
{
	neighborNum = 0;
	hasMiningEvent = false;
	auto block = make_shared<Block>();
	AddBlock(block);
	high = -1;
	low = -1;
	txPool.set();
}

void Node::AddBlock(shared_ptr<Block> block)
{
	mainchain.push_back(block);
	if (!block->uncles.empty()) {
		for (auto iter = block->uncles.begin(); iter != block->uncles.end(); iter++) {
			includedUncles[*iter] = true;
		}
	}
}

void Node::AddUncles(shared_ptr<Block> block)
{
	int maxUncles = MAX_UNCLES;
	for (auto iter = unclechain.begin(); iter != unclechain.end() && maxUncles > 0; iter++) {
		int baseDepth = block->depth;
		int uncleDepth = (*iter)->depth;
		if (uncleDepth > baseDepth - MIN_UNCLE_GENERATION + 1 || uncleDepth < baseDepth - MAX_UNCLE_GENERATION + 1) continue;
		if (mainchain[uncleDepth]->id == (*iter)->id) continue;
		if (mainchain[uncleDepth]->prevBlock != (*iter)->prevBlock) continue;
		if (includedUncles.find((*iter)->id) != includedUncles.end()) continue;
		block->uncles.push_back((*iter)->id);
		includedUncles[(*iter)->id] = true;
		block->diff++;
		maxUncles--;
		iter = unclechain.erase(iter);
		iter--;
	}
}
