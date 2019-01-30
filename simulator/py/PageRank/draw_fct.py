import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import math
import os

algorithms = [
    'fastpass',
    'pfabric',
    'phost',
    'fifo'
]

experiments = [
    'fastpass_perfect',
    'fastpass_20_00',
    'fastpass_20_01',
    'fastpass_50_00',
    'fastpass_50_01',
    'pfabric_perfect',
    'pfabric_20_00',
    'pfabric_20_01',
    'pfabric_50_00',
    'pfabric_50_01',
    'phost_perfect',
    'phost_20_00',
    'phost_20_01',
    'phost_50_00',
    'phost_50_01',
    'fifo',
    'buffer',
    'threshold',
    'acc_25',
    'acc_50',
    'acc_75',
]

average_dimension = []

def get_algorithm_name(txt):
    for experiment in experiments:
        if experiment in txt:
            return experiment

def get_color(txt):
    colors = [
        'r', 'r', 'r', 'r', 'r',
        'g', 'g', 'g', 'g', 'g',
        'y', 'y', 'y', 'y', 'y',
        'b', '#a54cff', '#c54cff',
        '#436cad','#2f5da8','#082451']
    for experiment in enumerate(experiments):
        if experiment[1] in txt:
            return colors[experiment[0]]


def get_linesyle(txt):
    if 'prediction' in txt:
        return '--'

def print_stats(data, column_name):
    data = sorted(data, key= lambda x:x[0])
    for i in data:
        print 'Mean and median (and 99th p)' \
            + column_name +' for ' + i[3] + ' is ' + str(i[0]) + ' -- ' + str(i[1]) \
            + ' -- ' + str(i[2])


def draw_cdf(raw_data, figure, file_name, column_name):
    global average_dimension
    #algorithm_name = get_algorithm_name(file_name)

    raw_data = sorted (raw_data)
    raw_data99 = raw_data[int(len(raw_data)*0.99)]
    data = map(lambda x: math.log10(x), raw_data)
    
    #print data
    #print
    y = np.arange(0.,1.,1./len(data))
    if len(y)!=len(data):
        y = y[:-1]
    plt.plot(data, y, label=file_name, linewidth=2, linestyle=get_linesyle(file_name), color=get_color(file_name))

    average_dimension.append((
        round(np.average(raw_data), 1),
        round(np.median(raw_data), 1),
        raw_data99,
        file_name
    ))
    return raw_data

def write_to_file(x, file_name):
    with open(file_name, 'w') as f:
        for i in x:
            f.write(str(i)+'\n')


def plot_column(column_number, column_name, f_write=False):
    global average_dimension
    average_dimension = []
    figure = plt.figure(figsize=(24.0, 15.0))

    for f in os.listdir('trace'):
        file_path = os.path.join('trace', f)
        df = pd.read_csv(file_path, skipfooter=4, header=None, delimiter=' ')
        #extract flow completion time from the data set
        fct = df[column_number].tolist()
        data = draw_cdf(fct, figure, f, column_name)
        if f_write:
            write_to_file(data, 'logs/'+column_name+'_'+f)

    print_stats(average_dimension, column_name)

    plt.xlabel(column_name + ' log(us)', fontsize=18)
    plt.ylabel('CDF', fontsize=18)
    plt.grid(True)
    plt.legend()
    figure.savefig('./data/'+column_name+".pdf", bbox_inches='tight')
    #plt.show()
    print

def main():
    plot_column(6, "fct", True)
    plot_column(8, "slowdown", True)

if __name__ == '__main__':
    main()