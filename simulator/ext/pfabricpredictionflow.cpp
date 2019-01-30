#include "pfabricpredictionflow.h"

#include <iostream>

PFabricPredictionFlow::PFabricPredictionFlow(
    uint32_t predicted_flow_size, uint32_t id, double start_time, uint32_t size, Host *s, Host *d
) : PFabricFlow(id, start_time, size, s, d) {
    this->predicted_flow_size = predicted_flow_size;
    this->predicted_flow_size = std::max(1., this->predicted_flow_size);
}

uint32_t PFabricPredictionFlow::get_priority(uint32_t seq) {
	uint32_t currently_sent = last_unacked_seq + scoreboard_sack_bytes;
	while(predicted_flow_size < currently_sent){
		predicted_flow_size = (predicted_flow_size+1)*2;
	}

    double result = predicted_flow_size - currently_sent;
    result = result>1?result:1;

    if(this->flow_size_unknown){
    	result = seq;
	}

    return result;
}

void PFabricPredictionFlow::set_unknown() {
	this->flow_size_unknown = true;
}
