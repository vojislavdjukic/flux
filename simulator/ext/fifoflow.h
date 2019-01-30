#ifndef FIFO_FLOW_H
#define FIFO_FLOW_H

#include "pfabricflow.h"
#include "../coresim/node.h"

class FifoFlow : public PFabricFlow {
    public:
        FifoFlow(uint32_t id, double start_time, uint32_t size, Host *s, Host *d);
        virtual uint32_t get_priority(uint32_t seq);
};

#endif
