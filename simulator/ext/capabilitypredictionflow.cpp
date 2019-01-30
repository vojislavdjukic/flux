#include "capabilitypredictionflow.h"

#include <iostream>
#include <math.h>

CapabilityPredictionFlow::CapabilityPredictionFlow(
	uint32_t predicted_flow_size,uint32_t id, double start_time, uint32_t size, Host *s, Host *d
): CapabilityFlow(id, start_time, size, s, d) {
    this->predicted_flow_size = predicted_flow_size;
    predicted_flow_size_pkt = (int)ceil((double)this->predicted_flow_size/1460);
    this->capability_goal = (int)(ceil(this->predicted_flow_size_pkt * 1.00));
    this->remaining_pkts_at_sender = this->predicted_flow_size_pkt;

    this->predicted_flow_size = predicted_flow_size;
}

int CapabilityPredictionFlow::remaining_pkts(){
	//return std::max((int)0, (int)(this->size_in_pkt - this->received_count));
	while((predicted_flow_size_pkt)<received_count){
		predicted_flow_size_pkt = (predicted_flow_size_pkt+1)*2;
		this->capability_goal = (int)(ceil(this->predicted_flow_size_pkt * 1.00));
		this->remaining_pkts_at_sender = this->predicted_flow_size_pkt;
	}
	return std::max(0, (int)(predicted_flow_size_pkt - received_count));
}
