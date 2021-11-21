#include <iostream>
#include <ctime>
#include "Scheduler.h"

using namespace std;

int main()
{
	double rate = 0, half = 0, ninety = 0;
	for(int rd = 0; rd < ROUND; rd++) {
		clock_t end = clock();
		int num = 0;
		double globalTime = 0;
		Scheduler* scheduler = new Scheduler();
		scheduler->GenerateInitialEvents();
		while (!scheduler->evEngine.IsEmpty() && globalTime <= SIM_TIME) {
			auto nextEvent = scheduler->evEngine.GetEvent();
			globalTime = nextEvent->timestamp;
			if (nextEvent->type == GENERATE_BLOCK) {
				num++;
				cout << "GENERATE " << num << " BLOCKS, CLOCK: " << globalTime << ", AVERAGE BLOCK TIME: " << (double)globalTime / num << " s" << endl;
			}
			scheduler->HandleEvent(nextEvent);
			scheduler->evEngine.RemoveEvent(nextEvent);
		}
		scheduler->ResolveFork();
		scheduler->DistributeRewards();
		rate += scheduler->ShowStatistics(rd+1, (double)(clock() - end) / CLOCKS_PER_SEC / 60);
		delete scheduler;
	}
	return 0;
}
