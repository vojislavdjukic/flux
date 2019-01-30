import os
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import math
import numpy as np

SIMULATOR_PATH = '/home/voja/work/sigcomm/simulator/final/PageRank/50/t0t0/'
PATH_FLOW_DISTRIBUTION = './data/flow_distribution.txt'
PATH_EXECUTION_TIME = './data/execution_time.txt'
PATH_INCOMING_FLOW = './data/incoming_flow.txt'

def draw_flow_distribution():
    flows = []

    for f in os.listdir(SIMULATOR_PATH):
        df = pd.read_csv(SIMULATOR_PATH + f, index_col=False)
        flows += df['size'].tolist()
        
    flows = sorted(flows)

    #write file
    with open(PATH_FLOW_DISTRIBUTION,'w') as f:
        for flow in flows:
            f.write(str(flow)+"\n")


    #draw graph
    x_axis = map(lambda x: math.log10(x), flows)
    y_axis = np.arange(0.,1.,1./len(x_axis))
    if len(y_axis)!=len(x_axis):
        y_axis = y_axis[:-1]

    figure = plt.figure(figsize=(24.0, 15.0))
    plt.plot(x_axis, y_axis, label="Flow distribution", linewidth=2)
    plt.xlabel('Flow Size log(B)', fontsize=18)
    plt.ylabel('CDF', fontsize=18)
    plt.grid(True)
    plt.legend()
    figure.savefig("./data/flow_distribution.pdf", bbox_inches='tight')

def draw_execution_time():
    execution_time = []

    for f in os.listdir(SIMULATOR_PATH):
        df = pd.read_csv(SIMULATOR_PATH + f, index_col=False)
        execution_time.append(df['time'].tolist()[-2])
        
    execution_time = sorted(execution_time)
    #write file
    with open(PATH_EXECUTION_TIME,'w') as f:
        for time in execution_time:
            f.write(str(time)+"\n")


    #draw graph
    x_axis = map(lambda x: x/(1000), execution_time)
    y_axis = np.arange(0.,1.,1./len(x_axis))
    if len(y_axis)!=len(x_axis):
        y_axis = y_axis[:-1]

    figure = plt.figure(figsize=(24.0, 15.0))
    plt.plot(x_axis, y_axis, label="Execution time", linewidth=2)
    plt.xlabel('Execution time (s)', fontsize=18)
    plt.ylabel('CDF', fontsize=18)
    plt.grid(True)
    plt.legend()
    figure.savefig("./data/execution_time.pdf", bbox_inches='tight')

def draw_incoming_flow():
    freqs = []

    for f in os.listdir(SIMULATOR_PATH):
        df = pd.read_csv(SIMULATOR_PATH + f, index_col=False)
        time = df['time'].tolist()
        freqs.append((time[-2]/1000.)/len(time))
        
    freqs = sorted(freqs)

    #write file
    with open(PATH_INCOMING_FLOW,'w') as f:
        for freq in freqs:
            f.write(str(freq)+"\n")


    #draw graph
    x_axis = map(lambda x: x, freqs)
    y_axis = np.arange(0.,1.,1./len(x_axis))
    if len(y_axis)!=len(x_axis):
        y_axis = y_axis[:-1]

    figure = plt.figure(figsize=(24.0, 15.0))
    plt.plot(x_axis, y_axis, label="Incoming flows", linewidth=2)
    plt.xlabel('Flows per second (s)', fontsize=18)
    plt.ylabel('CDF', fontsize=18)
    plt.grid(True)
    plt.legend()
    figure.savefig("./data/incoming_flows.pdf", bbox_inches='tight')

draw_execution_time()
draw_flow_distribution()
draw_incoming_flow()