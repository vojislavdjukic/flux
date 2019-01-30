#ifndef CAPABILITY_MEAN_FLOW_H
#define CAPABILITY_MEAN_FLOW_H

#include "capabilityflow.h"

class CapabilityPredictionFlow : public CapabilityFlow {
public:
	CapabilityPredictionFlow(uint32_t predicted_flow_size, uint32_t id, double start_time, uint32_t size, Host *s, Host *d);
	double predicted_flow_size_pkt;
	int remaining_pkts();
};


#endif

