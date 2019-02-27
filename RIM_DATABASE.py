import os.path
import sys
import h5py
import numpy as np
import random

NB_TRACES = 10000
SAMPLES = 3000
SIZE = 16

def read_traces(filename = "../data/TRACES_MOD"):
    traces = []

    f = open(filename, "r")

    for i in range(NB_TRACES):
        traces.append(list(map(lambda x: float(x), f.readline().split(" ")[:-1])))
        
    return np.array(traces)

#print(read_traces("./data/TRACES_MOD").shape)

def read_plain_texts(filename = "../data/PLAIN_TEXT_MOD"):
    plaintexts = []

    f = open(filename, "r")

    for i in range(NB_TRACES):
        plaintexts.append(list(map(lambda x: int(x, 16), f.readline().split(" ")[:-1])))
        
    return np.array(plaintexts)

#print(read_plain_texts("./data/PLAIN_TEXT_MOD").shape)

