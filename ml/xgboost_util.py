from sklearn.metrics import mean_squared_error, r2_score, mean_absolute_error
import pandas as pd
import numpy as np
from pandas import concat

def print_metrics(real, prediction):
    print 'MSE: %f' % mean_squared_error(real, prediction)
    print 'MAE: %f' % mean_absolute_error(real, prediction)
    print 'R2: %f' % r2_score(real, prediction)

def calculate_scaling(training_paths):
    scaling = {}
    #calculate scaling factors
    for f in training_paths:
        df = pd.read_csv(f, index_col=False)

        for column in df.columns:
            if column not in scaling:
               scaling[column] = 0.
            scaling[column] = max(scaling[column], float(df[column].max()))
    return scaling

def resize(s,scaling):
    return s/scaling[s.name]

def prepare_files(files, window_size, scaling, target_column='flow_size'):
    result = []

    for f in files:
        df = pd.read_csv(f, index_col=False)

        df = df.drop("index", axis=1)

        df = df.apply((lambda x: resize(x, scaling)), axis=0)
        flow_size = df[target_column]
        df[target_column] = flow_size
        #extend the window
        columns = list(df)
        final_df = df.copy()
        for sample_num in range(1, window_size):
            shifted = df.shift(sample_num)
            shifted.columns = map(lambda x: x+str(sample_num), shifted.columns)
            final_df = concat([shifted, final_df], axis=1)

        final_df = final_df.fillna(0)
        final_df = final_df.drop(target_column, axis=1)

        result.append((final_df, flow_size))

    return result

def make_io(data):
    inputs = None
    outputs = None
    for d in data:
        i_data = d[0].as_matrix()
        o_data = d[1].tolist()

        if inputs is None:
            inputs = i_data
            outputs = o_data
        else:
            inputs = np.append(inputs, i_data, axis=0)
            outputs = np.append(outputs, o_data)
    return (inputs, outputs)
