##Calculation of Fluence using the most naieve possible formulas
## Emmett Krupczak - 9 Mar 2016

#! /usr/bin/python

#
import math
import random
import numpy as np

background = 0.0015 #background events expected between 23 Apr 2015 and 10 Nov 2015. From Ishidoshiro: (#number observed events / livetime)*1000 sec window
#comes from root program which calculates FeldmanCousins upper and lower limits for a given number of observed events and given background
#N_FC_upper = 2.435 # including miniballoon cut and likelihood 
N_FC_upper = 2.235 #without miniballoon and likelihood cuts
N_target = 5.98e31 #number of protons in kamland using 6m cut

print "This file has been edited" 

#E = 2.01 #MeV

##Depends on energy. Using the table from 
## units cm^s
#cross_sec = 0.00351e-41 # E = 2.01 MeV
#cross_sec = 0.832e-41 # E = 11.0 MeV
#cross_sec = 11e-41 # E = 40.9 MeV
#cross_sec = 47.3e-41 # E = 104 MeV
cross_sec = [(2.01, 0.00351e-41), (11.0,0.832e-41),(40.9,11e-41),(104,47.3e-41) ]

fluence = []
for i in cross_sec:
 fluence.append((i[0],N_FC_upper / (i[1] * N_target)))

print "fluence:", fluence
