##This file determines whether there are any neutrino events within some given time window of a gravitational wave event. 
## Emmett Krupczak - 4 Mar 2016


#! /usr/bin/python

#
import math
import random
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import FormatStrFormatter
from matplotlib import pyplot as plt
import datetime
from matplotlib import rcParams
rcParams.update({'figure.autolayout': True})
#import histogramtest


##Read in neutrino .dat file 
def read_file(path):
	with open(path) as f:
		rows = [map(float, line.strip().split(" ")) for line in f.readlines()]
	return zip(*rows)

## neutrino events 
neutrino = read_file("Observed-dcvf-Kat-GW.dat") #Events from Fall 2015
run, energy, unixtime, eventnumber, delayed_eventnum, xp, yp, zp, xd, yd, zd, run_grade, multiplicity, dt, dl = range(15)


## gravitational events 
#gw = read_file("gravitational-wave-event-list.dat")
gw = read_file("testfile.dat")
name, time, SNR = range(3)

print gw[name]
print neutrino[run]