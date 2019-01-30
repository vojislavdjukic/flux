
#include "pfabricaging.h"

#include <iostream>
#include <algorithm>
#include "../run/params.h"

extern DCExpParams params;

PFabricAgingFlow::PFabricAgingFlow(
		uint32_t id, double start_time, uint32_t size, Host *s, Host *d
) : PFabricFlow(id, start_time, size, s, d) {

}

uint32_t PFabricAgingFlow::get_priority(uint32_t seq) {
	return seq;
}
