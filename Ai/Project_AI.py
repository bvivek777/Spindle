# -*- coding: utf-8 -*-

import json
import math
import random
import re
import sys
from pathlib import Path

import numpy as np
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

DEBUG = False
weights = [2000,630,218,163,1245]
MODEL_FILE = Path(__file__).resolve().parent / "modelAi.json"
FEATURE_COLUMNS = ['network','io','mem','math','db','time']

def readFile(fp):
    f = open(fp, "r")
    listOfCodes = f.readlines()
    #listOfCodes.append('====')
    data = []
    network, io, mem, mat, db, time = 0, 0, 0, 0, 0, 0
    for i in listOfCodes:
        if i.find('network') >=0: network+=1; time += weights[0]
        elif i.find('io') >=0: io+=1; time += weights[1]
        elif i.find('mem') >=0: mem+=1; time += weights[2]
        elif i.find('math') >=0: mat+=1; time += weights[3]
        elif i.find('db') >=0: db+=1; time += weights[4]
        #r1 = re.findall(r"([0-9]+)",i)
        #if len(r1) > 0:
        #    time += int(r1[0])
        if i.find('====') >=0:
            data.append((network, io, mem, mat, db, time))
            network, io, mem, mat, db, time = 0, 0, 0, 0, 0, 0
    f.close()
    #f = open("timefile.txt", "r")
    #listOfTimes = f.readlines();
    #timeData = [int(str.strip()) for str in listOfTimes]
    return data[2:]

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
    
    return clusterer, lookup

def learnModel(fp):
    data = readFile(fp)
    dataFinal = readData(data)
    df = dataFinal.sample(frac = 0.01, replace = False, random_state = 29)
    df = df.reset_index(drop=True)
    #print(df)
    clusterer, lookup = clusterAndLearn(df)
    saveModel(clusterer, lookup)
    
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

def saveModel(clusterer, lookup):
    payload = {
        "cluster_centers": clusterer.cluster_centers_.tolist(),
        "lookup": lookup,
        "features": FEATURE_COLUMNS,
        "n_clusters": int(clusterer.n_clusters)
    }
    MODEL_FILE.write_text(json.dumps(payload), encoding="utf-8")

def loadModel():
    try:
        payload = json.loads(MODEL_FILE.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise RuntimeError("Model file missing. Run 'learn' first.") from exc
    except json.JSONDecodeError as exc:
        raise RuntimeError("Model file is corrupted.") from exc

    centers = np.array(payload.get("cluster_centers"), dtype=float)
    lookup = payload.get("lookup")
    features = payload.get("features", [])
    if centers.ndim != 2 or centers.shape[1] != len(FEATURE_COLUMNS):
        raise RuntimeError("Model centers have unexpected shape.")
    if not isinstance(lookup, list) or len(lookup) != centers.shape[0]:
        raise RuntimeError("Model lookup table is invalid.")
    if features != FEATURE_COLUMNS:
        raise RuntimeError("Model features mismatch current version.")
    return centers, lookup

def predictWithCenters(df, centers):
    feature_matrix = df[FEATURE_COLUMNS].to_numpy(dtype=float)
    # Calculate Euclidean distance to each center
    distances = np.linalg.norm(
        feature_matrix[:, np.newaxis, :] - centers[np.newaxis, :, :],
        axis=2
    )
    return np.argmin(distances, axis=1)

def makeAssignment(fp):
    centers, lookup = loadModel()
    
    data = readFile(fp)
    df = readData(data)
    
    assignment = predictWithCenters(df, centers)
    
    threadAssignment = ThreadAssignment(assignment.tolist(), lookup)
    print(threadAssignment)
    

if len(sys.argv) < 2:
    print ( " Enter Mode of Execution" )
elif sys.argv[1] == "learn":
    learnModel("training.txt")
elif sys.argv[1] == "assign":
    makeAssignment("myfile.txt")
else :
    print("invalid choice")

#learnModel("training.txt")
#makeAssignment("myfile.txt")
#readData(3,3)      
#print(readFile());   