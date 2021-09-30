#include "Block.h"

Block::Block()
{
	id = 0;
	depth = 0;
	diff = 1;
	miner = -1;
	size = BLOCK_SIZE;
	timestamp = 0;
	txs.reset();
	gasLimit = 0;
	low = TX_NUM-1;
	high = 0;
}
