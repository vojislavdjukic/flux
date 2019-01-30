#include "trace_cdf_generator.h"

#include <cstdint>
#include <cstdlib>
#include <deque>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "../coresim/random_variable.h"
#include "../coresim/topology.h"
#include "../ext/factory.h"
#include "params.h"

extern DCExpParams params;
extern void add_to_event_queue(Event *);
extern std::priority_queue<Event*, std::vector<Event*>, EventComparator> event_queue;
extern double current_time;
extern std::deque<Flow *> flows_to_schedule;

TraceRandomVariable::TraceRandomVariable(vector<vector<int>>* p_flow_size_prediction){
    flow_size_prediction = p_flow_size_prediction;
    numberOfFlows = (*flow_size_prediction).size();
}

vector<int> TraceRandomVariable::value() { // never return 0
  long long index = rand()%numberOfFlows;
  return (*flow_size_prediction)[index];
}

TraceCDFGenerator::TraceCDFGenerator(
	uint32_t num_flows,
	Topology *topo,
	string filename
){
	read_trace(filename);
    make_flows(num_flows, topo);
}

void TraceCDFGenerator::read_trace(string trace_path){
	ifstream input(trace_path);
	string line;

	//skip first line
	getline(input, line);

	double sum = 0;
	double counter = 0;
	while (getline(input, line)) {
		vector<int> flow;
		std::stringstream ss(line);
		int i;
		while (ss >> i) {
			flow.push_back(i);
			ss.ignore(1);
		}
		sum += flow[0];
		counter++;
		trace.push_back(flow);
	}

	meanFlowSize = sum/counter;
}

void TraceCDFGenerator::make_flows(uint32_t num_flows, Topology *topo){
	srand(5);

    TraceRandomVariable *nv_bytes;
    nv_bytes = new TraceRandomVariable(&trace);

    params.mean_flow_size = meanFlowSize;

    double lambda = params.bandwidth * params.load / (params.mean_flow_size * 8.);
    double lambda_per_host = lambda / (params.number_of_hosts - 1);

    ExponentialRandomVariable *nv_intarr;
    nv_intarr = new ExponentialRandomVariable(1.0 / lambda_per_host);
    for (uint32_t i = 0; i < params.number_of_hosts; i++) {
        for (uint32_t j = 0; j < params.number_of_hosts; j++) {
            if (i != j) {
                double first_flow_time = 1.0 + nv_intarr->value();
                add_to_event_queue(
                    new TraceFlowCreationForInitializationEvent(
                        first_flow_time,
                        topo->hosts[i],
                        topo->hosts[j],
                        nv_bytes,
                        nv_intarr
                    )
                );
            }
        }
    }

    while (event_queue.size() > 0) {
        Event *ev = event_queue.top();
        event_queue.pop();
        current_time = ev->time;
        if (flows_to_schedule.size() < num_flows) {
            ev->process_event();
        }
        delete ev;
    }

    for(Flow* f:flows_to_schedule){
    	params.total_bytes_scheduled+=f->size*(1500./1460);
    }

    current_time = 0;
}


/* Flow Arrival */
TraceFlowCreationForInitializationEvent::TraceFlowCreationForInitializationEvent(
        double time,
        Host *src,
        Host *dst,
        TraceRandomVariable *nv_bytes,
        ExponentialRandomVariable *nv_intarr
    ) : Event(FLOW_CREATION_EVENT, time) {
    this->src = src;
    this->dst = dst;
    this->nv_bytes = nv_bytes;
    this->nv_intarr = nv_intarr;
}

TraceFlowCreationForInitializationEvent::~TraceFlowCreationForInitializationEvent() {}

void TraceFlowCreationForInitializationEvent::process_event() {
    vector<int> flow;
    int size, prediction;
    bool unknown = false;
    int id = flows_to_schedule.size();
    flow = nv_bytes->value();
    size = (int) flow[0];
    prediction = (int) flow[1];

    if (size != 0) {
    	Flow* new_flow = Factory::get_flow(id, time, size, src, dst, params.flow_type, 0, prediction);
        flows_to_schedule.push_back(new_flow);
    }

    double tnext = time + nv_intarr->value();

    add_to_event_queue(new TraceFlowCreationForInitializationEvent(
		tnext,
		src,
		dst,
		nv_bytes,
		nv_intarr
	));
}
