# -*- coding: utf-8 -*-

from itertools import combinations as permutations;

i = 1;
a = "";
targetList = [];
runtimeList = [2000,630,218,7,1245];
while ( i <= 5 ):
    perm = permutations(runtimeList,i);
    for j in perm:
        targetList.append(j);
    i+=1;

f = open("training.txt", "w")

i = 1;
while ( i <= 5 ) :
    perm = permutations(targetList,i);
    i+=1;
    for obj in perm:
        for j in obj :
            for k in j :
                if ( k == 2000 ):
                    a = "network"
                elif ( k == 630 ):
                    a = "io"
                elif ( k == 1245 ):
                    a = "db"
                elif ( k == 218 ):
                    a = "mem"
                else :
                    a = "math"
                a = a+"\n"; #"("+str(k)+") //" + a + "\n";
                f.write(a);
        f.write("====\n");
        
f.close()