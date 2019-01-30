
# LSTM for international airline passengers problem with regression framing
import numpy
import pandas as pd
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import Activation
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import r2_score
import os

# fix random seed for reproducibility
numpy.random.seed(7)

TEST_NAME = "KMeans"
#TEST_NAME = "PageRank"
#TEST_NAME = "SGD"
#TEST_NAME = "tensorflow"
#TEST_NAME = "web_server"
look_back = 5

TRAIN_PATH = '../data/ml/' + TEST_NAME +'/training/'
TEST_PATH = '../data/ml/' + TEST_NAME +'/test/'
VALIDATION_PATH = '../data/ml/' + TEST_NAME +'/validation/'


def create_dataset(dataset, look_back=1):
    dataX, dataY = [], []
    for i in range(len(dataset)-look_back-1):
        a = dataset[i:(i+look_back), :].copy()
        a[-1,-1] = 0
        dataX.append(a.flatten())
        dataY.append(dataset[i + look_back - 1, -1])
    return numpy.array(dataX), numpy.array(dataY)

def load_dataset(path, ):
    dfs = []
    for f in os.listdir(path):
        df = pd.read_csv(path + f, engine='python', skipfooter=1)
        df = df.drop(columns=['index'])
        dfs.append(df)

    df = pd.concat(dfs, ignore_index=True)
    dataset = df.values
    dataset = dataset.astype('float32')

    dataset = dataset[:-1,:]

    return dataset

# normalize the dataset
scaler = MinMaxScaler(feature_range=(0, 1))

train = load_dataset(TRAIN_PATH)
train = scaler.fit_transform(train)

test = load_dataset(TEST_PATH)
test = scaler.transform(test)

validation = load_dataset(VALIDATION_PATH)
validation = scaler.transform(validation)


trainX, trainY = create_dataset(train, look_back)
testX, testY = create_dataset(test, look_back)
validationX, validationY = create_dataset(validation, look_back)

model = Sequential()
model.add(Dense(5, input_dim=trainX.shape[1], activation='relu'))
model.add(Dense(5, activation='relu'))
model.add(Dense(1, activation='sigmoid'))
model.compile(loss='mean_absolute_error', optimizer='adam')
model.fit(trainX, trainY, epochs=250, batch_size=10, verbose=2)

# make predictions
trainPredict = model.predict(trainX)
testPredict = model.predict(testX)
validationPredict = model.predict(validationX)

trainScore = r2_score(trainY.flatten(), trainPredict.flatten())
print('Train Score: %.2f R2' % (trainScore))
testScore = r2_score(testY.flatten(), testPredict.flatten())
print('Test Score: %.2f R2' % (testScore))
validationScore = r2_score(validationY.flatten(), validationPredict.flatten())
print('Validation Score: %.2f R2' % (validationScore))