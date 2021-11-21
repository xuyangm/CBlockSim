#include "Statistics.h"
#include <sys/stat.h>

Statistics::Statistics()
{
    struct stat buffer;
    if(stat("output.csv", &buffer) == 0) output.open("output.csv", ios::app);
    else {
        output.open("output.csv", ios::out);
        output << "Round" << "," << "Execution Time" << "," << "Block Time" << "," << "Block Size" << "," << "#Mainchain Blocks" << "," << "#Stale/Uncle Blocks" << "," << "50% Block Propagation Delay" << "," << "90% Block Propagation Delay" << "," << "Stale/Uncle Rate" << endl;
    }
}

Statistics::~Statistics()
{
    output.close();
}

void Statistics::write(int rd, double execTime, double blockTime, double blockSize, int mainBlocks, int orphans, double halfBPD, double ninetyBPD, double rate)
{
    output << rd << "," << execTime << "," << blockTime << "," << blockSize << "," << mainBlocks << "," << orphans << "," << halfBPD << "," << ninetyBPD << "," << rate << endl;
}