#pragma once

class Transaction
{
public:
	int id;                    // Id, starting from 0
	int sender;                // Node id of transaction sender
	double creationTimestamp;  
	double receivingTimestamp;
	double size;               // Transaction size (Mb)
	double fee;                // Transaction fee
	int gasLimit;
	double gasPrice;
	double gasUsed;
};