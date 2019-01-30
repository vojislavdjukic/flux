#ifndef PFABRIC_SEQ_FLOW_H
#define PFABRIC_SEQ_FLOW_H

#include "pfabricflow.h"
#include "../coresim/node.h"
#include <vector>

class PFabricAgingFlow : public PFabricFlow {
    public:
	PFabricAgingFlow(uint32_t id, double start_time, uint32_t size, Host *s, Host *d);
        virtual uint32_t get_priority(uint32_t seq);
};

#endif
