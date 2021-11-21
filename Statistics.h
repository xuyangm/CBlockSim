#include <fstream>
#include <sstream>

using namespace std;

class Statistics
{
public:
    Statistics();
    ~Statistics();

    void write(int rd, double execTime, double blockTime, double blockSize, int mainBlocks, int orphans, double halfBPD, double ninetyBPD, double rate);

    ofstream output;
};