#!/usr/bin/env python

import xgboost
import os
import xgboost_util
import math

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import random

random.seed(0)

NUMBER_OF_TREES = 50
WINDOW_SIZE = 5

TEST_NAME = 'PageRank'
#TEST_NAME = 'KMeans'
#TEST_NAME = 'SGD'
#TEST_NAME = 'tensorflow'
#TEST_NAME = 'web_server'

TARGET_COLUMN = 'flow_size'

TRAINING_PATH = '../data/ml/' + TEST_NAME + '/training/'
TEST_PATH = '../data/ml/' + TEST_NAME + '/test/'
VALIDATION_PATH = '../data/ml/' + TEST_NAME + '/validation/'

training_files = [os.path.join(TRAINING_PATH, f) for f in os.listdir(TRAINING_PATH)]
test_files = [os.path.join(TEST_PATH, f) for f in os.listdir(TEST_PATH)]
validation_files = [os.path.join(VALIDATION_PATH, f) for f in os.listdir(VALIDATION_PATH)]

scaling = xgboost_util.calculate_scaling(training_files)
data = xgboost_util.prepare_files(training_files, WINDOW_SIZE, scaling, TARGET_COLUMN)

inputs, outputs = xgboost_util.make_io(data)

# fit model no training data
param = {
    'num_epochs' : NUMBER_OF_TREES,
    'max_depth' : 10,
    'objective' : 'reg:linear',
    'booster' : 'gbtree',
    'base_score' : 2,
    'silent': 1,
    'eval_metric': 'mae'
}

training = xgboost.DMatrix(inputs, outputs, feature_names = data[0][0].columns)
print len(outputs)
print 'Training started'
model = xgboost.train(param, training, param['num_epochs'])

def print_performance(files, write_to_simulator=False):
    real = []
    predicted = []
    for f in files:
        data = xgboost_util.prepare_files([f], WINDOW_SIZE, scaling, TARGET_COLUMN)
        inputs, outputs = xgboost_util.make_io(data)

        y_pred = model.predict(xgboost.DMatrix(inputs, feature_names = data[0][0].columns))
        pred = y_pred.tolist()

        real += outputs
        predicted += pred

    xgboost_util.print_metrics(real, predicted)

print 'TRAINING'
print_performance(training_files)
print

print 'TEST'
print_performance(test_files)
print

print 'VALIDATION'
print_performance(validation_files)
