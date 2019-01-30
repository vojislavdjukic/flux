#ifndef FASTPASS_MEAN_FLOW_H
#define FASTPASS_MEAN_FLOW_H

#include "fastpassflow.h"

class FastpassPredictionFlow : public FastpassFlow {
public:
	FastpassPredictionFlow(uint32_t predicted_flow_size, uint32_t id, double start_time, uint32_t size, Host *s, Host *d);
	double estimated_flow_size_packets;
	int remaining_flow_size_estimate();
	void send_data_pkt();
};

#endif
