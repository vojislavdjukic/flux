#include "factory.h"

#include "pfabricqueue.h"
#include "pfabricflow.h"
#include "schedulinghost.h"

#include "capabilityhost.h"
#include "capabilityflow.h"
#include "capabilitypredictionflow.h"

#include "fastpassflow.h"
#include "fastpasshost.h"

#include "fifoflow.h"

#include "tcpflow.h"

#include "dctcpQueue.h"
#include "dctcpFlow.h"
#include "fastpasspredictionflow.h"
#include "pfabricaging.h"

#include "pfabricpredictionflow.h"

extern DCExpParams params;

/* Factory method to return appropriate queue */
Queue* Factory::get_queue(
        uint32_t id, 
        double rate,
        uint32_t queue_size, 
        uint32_t type,
        double drop_prob, 
        int location
        ) { // Default drop_prob is 0.0

    switch(type) {
        case DROPTAIL_QUEUE:
            return new Queue(id, rate, queue_size, location);
        case PFABRIC_QUEUE:
            return new PFabricQueue(id, rate, queue_size, location);
        case PROB_DROP_QUEUE:
            return new ProbDropQueue(id, rate, queue_size, drop_prob, location);
        case DCTCP_QUEUE:
            return new DctcpQueue(id, rate, queue_size, location);
    }
    assert(false);
    return NULL;
}

int Factory::flow_counter = 0;

Flow* Factory::get_flow(
        double start_time, 
        uint32_t size,
        Host *src, 
        Host *dst, 
        uint32_t flow_type,
        double rate,
		uint32_t predicted_flow_size,
		std::vector<double>* probabilities,
		std::vector<double>* centers
        ) {
    return Factory::get_flow(Factory::flow_counter++, start_time, size, src, dst, flow_type, rate, predicted_flow_size,
    		probabilities, centers);
}

Flow* Factory::get_flow(
        uint32_t id, 
        double start_time, 
        uint32_t size,
        Host *src, 
        Host *dst, 
        uint32_t flow_type,
        double rate,
		uint32_t predicted_flow_size,
		std::vector<double>* probabilities,
		std::vector<double>* centers
        ) { // Default rate is 1.0
    switch (flow_type) {
        case NORMAL_FLOW:
            return new Flow(id, start_time, size, src, dst);
            break;
        case PFABRIC_FLOW:
            //return new PFabricFlow(id, start_time, size, src, dst);
        	return new PFabricPredictionFlow(size, id, start_time, size, src, dst);
            break;
        case PFABRIC_PREDICTION_FLOW:
			return new PFabricPredictionFlow(predicted_flow_size, id, start_time, size, src, dst);
			break;
        case PFABRIC_MEAN_FLOW:
			return new PFabricPredictionFlow(params.mean_flow_size, id, start_time, size, src, dst);
			break;
        case PFABRIC_AGING_FLOW:
			return new PFabricAgingFlow(id, start_time, size, src, dst);
			break;
        case CAPABILITY_FLOW:
            return new CapabilityFlow(id, start_time, size, src, dst);
            break;
        case CAPABILITY_PREDICTION_FLOW:
			return new CapabilityPredictionFlow(predicted_flow_size, id, start_time, size, src, dst);
			break;
        case CAPABILITY_MEAN_FLOW:
			return new CapabilityPredictionFlow(params.mean_flow_size, id, start_time, size, src, dst);
			break;
        case FASTPASS_FLOW:
            //return new FastpassFlow(id, start_time, size, src, dst);
        	return new FastpassPredictionFlow(size, id, start_time, size, src, dst);
            break;
        case FASTPASS_PREDICTION_FLOW:
			return new FastpassPredictionFlow(predicted_flow_size, id, start_time, size, src, dst);
			break;
        case FASTPASS_MEAN_FLOW:
			return new FastpassPredictionFlow(params.mean_flow_size, id, start_time, size, src, dst);
			break;
        case VANILLA_TCP_FLOW:
            return new TCPFlow(id, start_time, size, src, dst);
            break;
        case DCTCP_FLOW:
            return new DctcpFlow(id, start_time, size, src, dst);
            break;
        case FIFO_FLOW:
            return new FifoFlow(id, start_time, size, src, dst);
            break;
    }
    assert(false);
    return NULL;
}

Host* Factory::get_host(
        uint32_t id, 
        double rate, 
        uint32_t queue_type, 
        uint32_t host_type
        ) {
    switch (host_type) {
        case NORMAL_HOST:
            return new Host(id, rate, queue_type, NORMAL_HOST);
            break;
        case SCHEDULING_HOST:
            return new SchedulingHost(id, rate, queue_type);
            break;
        case CAPABILITY_HOST:
            return new CapabilityHost(id, rate, queue_type);
            break;
        case FASTPASS_HOST:
            return new FastpassHost(id, rate, queue_type);
            break;
    }
    std::cerr << host_type << " unknown\n";
    assert(false);
    return NULL;
}

