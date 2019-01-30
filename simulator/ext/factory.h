#ifndef FACTORY_H
#define FACTORY_H

#include "../run/params.h"
#include "../coresim/flow.h"
#include "../coresim/node.h"
#include "../coresim/queue.h"

/* Queue types */
#define DROPTAIL_QUEUE 1
#define PFABRIC_QUEUE 2
#define PROB_DROP_QUEUE 4
#define DCTCP_QUEUE 5

/* Flow types */
#define NORMAL_FLOW 1
#define PFABRIC_FLOW 2
#define PFABRIC_PREDICTION_FLOW 201
#define PFABRIC_MEAN_FLOW 202
#define PFABRIC_PROBABILITY_FLOW 203
#define PFABRIC_AGING_FLOW 205
#define VANILLA_TCP_FLOW 42
#define DCTCP_FLOW 43
#define CAPABILITY_FLOW 112
#define CAPABILITY_PREDICTION_FLOW 212
#define CAPABILITY_MEAN_FLOW 213
#define FASTPASS_FLOW 114
#define FASTPASS_PREDICTION_FLOW 214
#define FASTPASS_MEAN_FLOW 215
#define FIFO_FLOW 200

/* Host types */
#define NORMAL_HOST 1
#define SCHEDULING_HOST 2
#define CAPABILITY_HOST 12
#define FASTPASS_HOST 14
#define FASTPASS_ARBITER 10

extern DCExpParams params;

class Factory {
    public:
        static int flow_counter;
        static Flow *get_flow(
                uint32_t id, 
                double start_time, 
                uint32_t size,
                Host *src, 
                Host *dst, 
                uint32_t flow_type,
                double paced_rate,
				uint32_t predicted_flow_size,
				std::vector<double>* probabilities = NULL,
				std::vector<double>* centers = NULL
                );

        static Flow *get_flow(
                double start_time, 
                uint32_t size,
                Host *src, 
                Host *dst, 
                uint32_t flow_type,
                double paced_rate,
				uint32_t predicted_flow_size,
				std::vector<double>* probabilities = NULL,
				std::vector<double>* centers = NULL
                );

        static Queue *get_queue(
                uint32_t id, 
                double rate,
                uint32_t queue_size, 
                uint32_t type,
                double drop_prob, 
                int location
                );

        static Host* get_host(
                uint32_t id, 
                double rate, 
                uint32_t queue_type, 
                uint32_t host_type
                );
};

#endif
