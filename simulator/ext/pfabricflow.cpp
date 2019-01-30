#include "pfabricflow.h"
#include <iostream>

/* Implementation for pFabric Flow */
extern double get_current_time();

PFabricFlow::PFabricFlow(uint32_t id, double start_time, uint32_t size, Host *s, Host *d)
    : Flow(id, start_time, size, s, d) {
        //Congestion window parameters
        this->ssthresh = 100000*mss;
        this->count_ack_additive_increase = 0;
    }

void PFabricFlow::increase_cwnd() {
    if (cwnd_mss < ssthresh) { // slow start
        cwnd_mss += 1*mss;
    }
    else { // additive increase
        if ((++count_ack_additive_increase)*mss >= cwnd_mss) {
            count_ack_additive_increase = 0;
            cwnd_mss += 1*mss;
        }
    }
    // Check if we exceed max_cwnd
    if (cwnd_mss > max_cwnd) {
        cwnd_mss = max_cwnd;
    }
}

void PFabricFlow::handle_timeout() {
    ssthresh = cwnd_mss / 2;
    if (ssthresh < 2*mss) {
        ssthresh = 2*mss;
    }
    Flow::handle_timeout();
}

