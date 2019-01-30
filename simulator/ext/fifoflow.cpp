#include "fifoflow.h"

FifoFlow::FifoFlow(
    uint32_t id, double start_time, uint32_t size, Host *s, Host *d
) : PFabricFlow(id, start_time, size, s, d) {
    
}

uint32_t FifoFlow::get_priority(uint32_t seq) {
    return 1;
}