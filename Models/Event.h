#pragma once
#include "Block.h"
#include <memory>

class Event
{
public:
	int type;
	int owner;
	double timestamp;
	std::shared_ptr<Block> block;
};