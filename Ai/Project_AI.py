# -*- coding: utf-8 -*-
"""
Created on Sat Dec  7 15:27:49 2019

@author: shash
"""
import numpy as np
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
import re
import pickle
import math
import random

DEBUG = False

def readFile():
    f = open("myfile.txt", "r")
    listOfCodes = f.readlines()
    listOfCodes.append('====')
    data = []
    network, io, mem, mat, db, time = 0, 0, 0, 0, 0, 0
    for i in listOfCodes:
        if i.find('network') >=0: network+=1
        elif i.find('io') >=0: io+=1
        elif i.find('mem') >=0: mem+=1
        elif i.find('math') >=0: mat+=1
        elif i.find('db') >=0: db+=1
        r1 = re.findall(r"([0-9]+)",i)
        if len(r1) > 0:
            time += int(r1[0])
        if i.find('====') >=0:
            data.append((network, io, mem, mat, db, time))
            network, io, mem, mat, db, time = 0, 0, 0, 0, 0, 0
    f.close()
    #f = open("timefile.txt", "r")
    #listOfTimes = f.readlines();
    #timeData = [int(str.strip()) for str in listOfTimes]
    return data

def readData(code):
    data = pd.DataFrame(code,
                        columns=['network',
                                 'io',
                                 'mem',
                                 'math',
                                 'db',
                                 'time']);
    test = data.sample(frac = 1).reset_index(drop=True)
    if DEBUG : print(test)
    return data

def clusterAndLearn(df):
    clusterer = KMeans(n_clusters=8, random_state=29).fit(df)
    preds = clusterer.predict(df)
    centers = clusterer.cluster_centers_
    
    score = silhouette_score(df, clusterer.labels_, metric='euclidean')
    print("KMeans score", score)
    
    df['cluster'] = preds
    
    #print(df)

    table = df[['cluster', 'time']].groupby('cluster', as_index=False).mean()
    
    lookup = table['time'].values.tolist()
    lookup = list(map(lambda x: round(x, 2), lookup))
    pickle.dump(clusterer, open('modelAi.model', 'wb'))
    pickle.dump(lookup, open('modelAiTable.table', 'wb'))
    #loaded_model = pickle.load(open(filename, 'rb'))
    
    return lookup

def learnModel():
    data = readFile()
    dataFinal = readData(data)
    df = dataFinal.sample(frac = 0.01, replace = False, random_state = 29)
    df = df.reset_index(drop=True)
    #print(df)
    table = clusterAndLearn(df)
    

def probabilityFunction(x):
    return math.exp(-x/4)

def SortClusterRunTimes(cluster_run_times):     # Method sorts the clusters based on runtime
    cluster_run_times.sort(key = lambda x: float(x[2]), reverse = True)
    return cluster_run_times

def ThreadAssignment(cluster_list, run_time_list):
    cluster_run_times = []

    i = 0
    for clusters in cluster_list:                   # Find the cluster run times from test set
        cluster_run_times.append(["f"+str(i),clusters, run_time_list[int(clusters)]])
        i += 1

    sorted_cluster_run_times = SortClusterRunTimes(cluster_run_times)

    number_of_threads = 0
    probability_thread_creation = probabilityFunction(number_of_threads)
    thread_assignment_list = []

    while len(sorted_cluster_run_times) > 0:
        x = random.randint(0, 100)
        if x/100 <= probability_thread_creation:
            number_of_threads += 1
            thread_assignment_list.append([sorted_cluster_run_times[0][0], number_of_threads])
            sorted_cluster_run_times = sorted_cluster_run_times[1:]
            probability_thread_creation = probabilityFunction(number_of_threads)
        else:
            thread_assignment_list.append([sorted_cluster_run_times[0][0], number_of_threads])
            sorted_cluster_run_times = sorted_cluster_run_times[1:]

    thread_assignment_list.sort(key = lambda x: int(x[0][1]))

    final_thread_assignment_list = []
    
    for items in thread_assignment_list:
        final_thread_assignment_list.append(items[1])

    return(final_thread_assignment_list)


def clusterLoad():
    clusterer = pickle.load(open('modelAi.model', 'rb'))
    lookup = pickle.load(open('modelAiTable.table', 'rb'))
    return clusterer, lookup

def makeAssignment():
    clusterer, lookup = clusterLoad()
    
    data = readFile()
    dataFinal = readData(data)
    df = dataFinal.sample(n=10, replace = False, random_state = 1)
    df = df.reset_index(drop=True)
    
    assignment = clusterer.predict(df)
    print(assignment)
    print(lookup)
    
    threadAssignment = ThreadAssignment(assignment, lookup)
    print(threadAssignment)
    

#learnModel()
    
makeAssignment()



#readData(3,3)
        
    