# -*- coding: utf-8 -*-
"""
Created on Thu Dec 12 19:53:13 2019

@author: shash
"""

from itertools import combinations as permutations;

i = 1;
a = "";
targetList = [];
runtimeList = [500,450,40,12,300];
while ( i <= 5 ):
    perm = permutations(runtimeList,i);
    for j in perm:
        targetList.append(j);
    i+=1;

f = open("myfile.txt", "w")

i = 1;
while ( i <= 5 ) :
    perm = permutations(targetList,i);
    i+=1;
    for obj in perm:
        for j in obj :
            for k in j :
                if ( k == 500 ):
                    a = "network"
                elif ( k == 450 ):
                    a = "io"
                elif ( k == 300 ):
                    a = "db"
                elif ( k == 40 ):
                    a = "mem"
                else :
                    a = "math"
                a = "sleep("+str(k)+") //" + a + "\n";
                f.write(a);
        f.write("====\n");
        
f.close()