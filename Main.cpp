#include <iostream>
#include <ctime>
#include "Scheduler.h"

using namespace std;

int main()
{
	double rate = 0, half = 0, ninety = 0;
	for(int z = 0; z < ROUND; z++) {
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
	rate += scheduler->ShowStatistics();
	cout << "Execution Time: " << (double)(clock() - end) / CLOCKS_PER_SEC / 60 << " min" << endl;
	cout << "50% BPD: " << scheduler->halfBPD / scheduler->count << " s" << endl;
	cout << "90% BPD: " << scheduler->ninetyBPD / scheduler->count << " s" << endl;
	half += scheduler->halfBPD / scheduler->count;
	ninety += scheduler->ninetyBPD / scheduler->count;
	delete scheduler;
	cout << "z = " << z << endl;
	}
	cout << "rate: " << rate/ROUND << "%" << endl;
	cout << "50% BPD: " << half/ROUND << endl;
	cout << "90% BPD: " << ninety/ROUND << endl;
	return 0;
}
