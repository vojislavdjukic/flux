#ifndef PFABRIC_MEAN_FLOW_H
#define PFABRIC_MEAN_FLOW_H

#include "pfabricflow.h"
#include "../coresim/node.h"

class PFabricPredictionFlow : public PFabricFlow {
    public:
		PFabricPredictionFlow(uint32_t predicted_flow_size, uint32_t id, double start_time, uint32_t size, Host *s, Host *d);
        virtual uint32_t get_priority(uint32_t seq);
        virtual void set_unknown();
};

#endif
