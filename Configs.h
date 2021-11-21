#pragma once
#include <string>

constexpr auto DATA = false;
constexpr auto NETWORK = true;
constexpr auto FLAG = 1; //0 for Bitcoin, 1 for Ethereum
constexpr auto PROPAGATION_POLICY = 1; //0 for CBR, 1 for Eth Wire
constexpr auto FINALIZE_POLICY = 0; //0 for Longest rule, 1 for GHOST rule
constexpr auto PROPOSAL_POLICY = 0; //0 for PoW, 1 for PoS

constexpr auto GENERATE_BLOCK = 0;
constexpr auto RECEIVE_BLOCK = 1;
constexpr auto INF = 100000.0;
constexpr auto ROUND = 10;

constexpr auto NODES_NUM = 8223;//11000;
constexpr auto BLOCK_INTERVAL = 13.05;//8;
constexpr auto BLOCK_VALIDATION = 0.34; // for 1Mb block
constexpr auto BLOCK_SIZE = 0.18;//0.8;  // Mb
constexpr auto SIM_TIME = 86400;//246857.0;//86400.0;//6600000;//3600;//86400.0;    // s
constexpr auto AVER_DELAY = 0.7;// s

constexpr auto TX_PER_SEC = 8;
constexpr auto TX_NUM = 1;//TX_PER_SEC * SIM_TIME; // When DATA is disable, set TX_NUM to 1 to save memory
constexpr auto TX_SIZE = 0.0018; // Mb

constexpr auto GAS_LIMIT = 15000000;
constexpr auto GAS_PRICE = 10.5548; // GWei
constexpr auto USED_GAS = 0.000072736;  // gas/1000000000

constexpr auto MAX_UNCLES = 2;
constexpr auto MIN_UNCLE_GENERATION = 2;
constexpr auto MAX_UNCLE_GENERATION = 7;

// Geographical Settings
constexpr auto REGIONS_NUM = 6;
extern const std::string REGIONS_NAME[REGIONS_NUM];
extern const double REGIONS_DISTRIBUTION[REGIONS_NUM];

// Latency&Bandwidth Settings
extern const double LATENCIES[REGIONS_NUM][REGIONS_NUM]; // LATENCIES[i][j] means the latency(s) between region i and j
extern const double DOWNLOAD_BANDWIDTHS[REGIONS_NUM + 1];  // download bandwidth(bps) of each region, the last one is the inter - regional bandwidth, from SimBlock
extern const double UPLOAD_BANDWIDTHS[REGIONS_NUM + 1];  // upload bandwidth(bps) of each region, the last one is the inter - regional bandwidth, from SimBlock

// Small World Topology Parameters 
constexpr auto BETA = 0.24;
constexpr auto AVER_DEGREE = 80;
extern const double DENSITY;  // from "Simple, distance-dependent formulation of the Watts-Strogatz model for directed and undirected small-world networks"
extern const int MAX_DISTANCE;  // from "Simple, distance-dependent formulation of the Watts-Strogatz model for directed and undirected small-world networks"

constexpr auto BLOCK_REWARD = 12.5;
constexpr auto UNCLE_REWARD = BLOCK_REWARD/32;
