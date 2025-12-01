# -*- coding: utf-8 -*-

import json
import math
import random
import sys
from pathlib import Path

import numpy as np
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

DEBUG = False
weights = [2000,630,218,163,1245]
FEATURE_COLUMNS = ['network', 'io', 'mem', 'math', 'db', 'time']
MODEL_VERSION = 1
MODEL_FILE = Path("modelAi.model")
LOOKUP_FILE = Path("modelAiTable.table")


def _atomic_write_json(path: Path, payload: dict):
    """
    Persist JSON data safely to disk to avoid partially written artifacts.
    """
    tmp_path = path.with_suffix(path.suffix + ".tmp")
    with tmp_path.open("w", encoding="utf-8") as handle:
        json.dump(payload, handle)
    tmp_path.replace(path)


def _load_json(path: Path):
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def persist_model(clusterer, lookup):
    """
    Store the learned model in a JSON format to avoid unsafe pickle usage.
    """
    model_payload = {
        "version": MODEL_VERSION,
        "feature_columns": FEATURE_COLUMNS,
        "cluster_centers": clusterer.cluster_centers_.tolist(),
    }
    lookup_payload = {
        "version": MODEL_VERSION,
        "lookup": lookup,
    }
    _atomic_write_json(MODEL_FILE, model_payload)
    _atomic_write_json(LOOKUP_FILE, lookup_payload)


def clusterLoad():
    """
    Load the persisted model artifacts with structural validation.
    """
    try:
        model_payload = _load_json(MODEL_FILE)
        lookup_payload = _load_json(LOOKUP_FILE)
    except FileNotFoundError as exc:
        raise FileNotFoundError(
            "Model artifacts missing. Run the script in 'learn' mode first."
        ) from exc
    except json.JSONDecodeError as exc:
        raise ValueError(
            "Model artifacts are not in the expected format. Re-run learning."
        ) from exc

    if model_payload.get("version") != MODEL_VERSION:
        raise ValueError("Model artifact version mismatch.")

    feature_columns = model_payload.get("feature_columns")
    if feature_columns != FEATURE_COLUMNS:
        raise ValueError("Unexpected feature ordering in model artifacts.")

    centers = np.asarray(model_payload.get("cluster_centers", []), dtype=float)
    if centers.size == 0:
        raise ValueError("Cluster centers are missing from the model artifact.")

    lookup = lookup_payload.get("lookup")
    if lookup is None:
        raise ValueError("Lookup table missing from artifacts.")
    if len(lookup) != centers.shape[0]:
        raise ValueError("Lookup table size does not match number of clusters.")

    return centers, lookup


def assign_clusters(df, centers):
    """
    Predict clusters by assigning each row to the nearest centroid.
    """
    if df.empty:
        return np.array([], dtype=int)
    feature_matrix = df[FEATURE_COLUMNS].to_numpy(dtype=float, copy=True)
    distances = np.linalg.norm(
        feature_matrix[:, np.newaxis, :] - centers[np.newaxis, :, :],
        axis=2
    )
    return np.argmin(distances, axis=1)

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
    
    table = df[['cluster', 'time']].groupby('cluster', as_index=False).mean()
    
    lookup = table['time'].values.tolist()
    lookup = list(map(lambda x: round(x, 2), lookup))
    
    persist_model(clusterer, lookup)
    
    return clusterer, lookup

def learnModel(fp):
    data = readFile(fp)
    dataFinal = readData(data)
    df = dataFinal.sample(frac = 0.01, replace = False, random_state = 29)
    df = df.reset_index(drop=True)
    #print(df)
    clusterAndLearn(df)
    
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

def makeAssignment(fp):
    centers, lookup = clusterLoad()
    
    data = readFile(fp)
    df = readData(data)
    #print(df);
    #df = dataFinal.sample(n=9, replace = False, random_state = 1)
    #df = df.reset_index(drop=True)
    
    assignment = assign_clusters(df, centers)
    #print(assignment)
    #print(lookup)
    
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