
#ifndef TRACE_CDF_GEN_H
#define TRACE_CDF_GEN_H

#include "../coresim/event.h"
#include "../coresim/topology.h"
#include <iostream>
#include <vector>
using namespace std;

class TraceCDFGenerator {
protected:
    double meanFlowSize;
    virtual void make_flows(uint32_t num_flows, Topology *topo);
    void read_trace(string trace_path);
    vector<vector<int>> trace;
public:
	TraceCDFGenerator(uint32_t num_flows, Topology *topo, std::string filename);
};


class TraceRandomVariable{
protected:
    vector<vector<int>>* flow_size_prediction;
    long long numberOfFlows;
public:
  virtual vector<int> value();
  TraceRandomVariable(vector<vector<int>>* p_flow_size_prediction);
};

//A flow arrival event Only used for FlowCreation
class TraceFlowCreationForInitializationEvent : public Event {
public:
	TraceFlowCreationForInitializationEvent(
		double time,
		Host *src,
		Host *dst,
		TraceRandomVariable *nv_bytes,
		ExponentialRandomVariable *nv_intarr
	);
	~TraceFlowCreationForInitializationEvent();
	void process_event();
	Host *src;
	Host *dst;
	TraceRandomVariable *nv_bytes;
	ExponentialRandomVariable *nv_intarr;
};

#endif
