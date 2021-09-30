#pragma once
#include "../Configs.h"
#include "../Models/Node.h"
#include <vector>
#include <memory>

class TopologyFactory
{
public:
	double totalHash = 0;

	void ProduceCompleteTopo(double topo[NODES_NUM][NODES_NUM]);
	void ProduceSmallWorldTopo(int maxDis, double prop, double dens, double topo[NODES_NUM][NODES_NUM]);
	void ProduceNodesByDistrib(std::vector<std::unique_ptr<Node>>& nodePool, double topo[NODES_NUM][NODES_NUM]);
};