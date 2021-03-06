##Version of KamLAND plots for looking at events potentially coincident to gravitational waves
## Emmett Krupczak - 3 Mar 2016

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
import histogramtest

def read_file(path):
	with open(path) as f:
		rows = [map(float, line.strip().split(" ")) for line in f.readlines()]
	return zip(*rows)

#field75 = read_file("Observed-dcvf-Kat.dat") #7.5 MeV threshold
fieldGW = read_file("Observed-dcvf-Kat-GW.dat") #Events from Fall 2015 between 0.9-100 MeV
fieldGWhe = read_file("Observed-dcvf-Kat-GW-highEnergy.dat") #Events from Fall 2015 (no max)
fieldGWnomini = read_file("Observed-dcvf-Kat-GW-noMiniballoon.dat") #Events from Fall 2015 without miniballoon cut
fieldGWnolike = read_file("Observed-dcvf-Kat-GW-100MeV_noMiniballoon-noLikelihood_SepOct.dat") #Events from Fall 2015 without miniballoon cut or likelihood selection

run, energy, unixtime, eventnumber, delayed_eventnum, xp, yp, zp, xd, yd, zd, run_grade, multiplicity, dt, dl = range(15)

## Datetime of LIGO event GW150914
GW150914 = datetime.datetime(2015,9,14,9,50,45)   #GPS time: 1126259462.39; UTC:2015-09-14T09:50:45
GW150914utc = int(GW150914.strftime("%s"))
print "GW150914 time:", GW150914utc

## Datetime of LIGO event LVT151012  #October 12, 2015 at 09:54:43 UTC
LVT151012 = datetime.datetime(2015,10,12,9,54,43)
LVT151012utc = int(LVT151012.strftime("%s"))
print "LVT151012 time:", LVT151012utc

datetimeGW = []
for i in fieldGW[unixtime]:
    datetimeGW.append(datetime.datetime.fromtimestamp(i))

datetimeGWhe = []
for i in fieldGWhe[unixtime]:
    datetimeGWhe.append(datetime.datetime.fromtimestamp(i))
    
datetimeGWnomini = []
for i in fieldGWnomini[unixtime]:
    datetimeGWnomini.append(datetime.datetime.fromtimestamp(i))
    
datetimeGWnolike = []
for i in fieldGWnolike[unixtime]:
    datetimeGWnolike.append(datetime.datetime.fromtimestamp(i))

#print "max energy", max(fieldGWhe[energy])
#print "min energy", min(fieldGWhe[energy])






#Coincident events between two neutrinos
#Events are sorted by time
#timediff_dict = {}
#for i in xrange(len(datetime75)):
#    if i >1:
#        timediff_list.append(datetime75[i] - datetime75[i-1])
#    if i < len(datetime75)-1:
#        timediff_list.append(datetime75[i+1] - datetime75[i])
#print timediff_list
timediff_list2 = [b - a for a, b in zip(datetimeGW[:-1], datetimeGW[1:])]
#print "Peter's version", timediff_lis2t
#print timediff_list == timediff_lis2t
timediff_sec = []
for i in timediff_list2:
    timediff_sec.append(i.total_seconds())
#print timediff_sec
#print len(timediff_sec)
#print sorted(timediff_sec)
#plt.hist(timediff_sec, bins= [10, 60, 1000, 2000, 3000, 5000, 10000, 20000, 30000, 50000, 86400, 604800, 2678400, 31536000])
timediff_days = []
for i in timediff_sec:
    timediff_days.append(i/86400.)

timediff_10day = []
for i in timediff_days:
    if i <= 10:
        timediff_10day.append(i)

## Concidence list between gravitational wave and neutrino
GWtimediff = 5000#seconds
print "Time window", GWtimediff, "seconds"

print "For GW events below 100 MeV"
for i in xrange(len(fieldGW[unixtime])):
    if abs(fieldGWnolike[unixtime][i] - GW150914utc) <= GWtimediff:
        print "Event coincident with GW150914:", "energy:", fieldGW[energy][i], "time:", fieldGW[unixtime][i], "event number:", fieldGW[eventnumber][i]
        
    if abs(fieldGWnolike[unixtime][i] - LVT151012utc) <= GWtimediff:
        print "Event coincident with LVT151012:", "energy:", fieldGW[energy][i], "time:", fieldGW[unixtime][i], "event number:", fieldGW[eventnumber][i]
   
   
print "For events of any energy without miniballoon cut"
for i in xrange(len(fieldGWnomini[unixtime])):
    if abs(fieldGWnomini[unixtime][i] - GW150914utc) <= GWtimediff:
        print "Event coincident with GW150914:", "energy:", fieldGWnomini[energy][i], "time:", fieldGWnomini[unixtime][i], "event number:", fieldGWnomini[eventnumber][i]
        
    if abs(fieldGWnomini[unixtime][i] - LVT151012utc) <= GWtimediff:
        print "Event coincident with LVT151012:", "energy:", fieldGWnomini[energy][i], "time:", fieldGWnomini[unixtime][i], "event number:", fieldGWnomini[eventnumber][i]

print "For events of any energy without miniballoon cut or likelihood selection"
for i in xrange(len(fieldGWnolike[unixtime])):
    if abs(fieldGWnolike[unixtime][i] - GW150914utc) <= GWtimediff:
        print "Event coincident with GW150914:", "energy:", fieldGWnolike[energy][i], "time:", fieldGWnolike[unixtime][i], "event number:", fieldGWnolike[eventnumber][i]
        print "Time difference", fieldGWnolike[unixtime][i] - GW150914utc
        
    if abs(fieldGWnolike[unixtime][i] - LVT151012utc) <= GWtimediff:
        print "Event coincident with LVT151012:", "energy:", fieldGWnolike[energy][i], "time:", fieldGWnolike[unixtime][i], "event number:", fieldGWnolike[eventnumber][i]
        print "Time difference", fieldGWnolike[unixtime][i] - LVT151012utc

  





### OLD PLOTS BELOW ###
"""
plt.hist(timediff_days, 10)
plt.show()
plt.gcf().clear()
"""

"""
#Time between events with curve
plt.hold(True)
xs, ys = histogramtest.smooth_points(timediff_days, 10, 500, 12)
fig, ax = plt.subplots()
counts, bins, patches = ax.hist(timediff_days, bins = [0,20, 40, 60,80, 100,120, 140, 160,180, 200, 220, 240])#bins = [0,10, 20, 30, 40,50, 60, 70,80,90, 100, 110,120,130, 140, 150,160, 170,180,190, 200, 210,220, 230])
ax.set_xticks(bins)
# Label the raw counts and the percentages below the x-axis...
bin_centers = 0.5 * np.diff(bins) + bins[:-1]
for count, x in zip(counts, bin_centers):
    # Label the raw counts
    ax.annotate(str(int(count)), xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -18), textcoords='offset points', va='top', ha='center')

    # Label the percentages
    percent = '%0.0f%%' % (100 * float(count) / counts.sum())
    ax.annotate(percent, xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -32), textcoords='offset points', va='top', ha='center')
plt.ylim(-2.5,20)
plt.plot(xs, ys, lw= 6, color="red")
plt.scatter(timediff_days, [-1]*len(timediff_days), marker="x",linewidth='1', s = 50, color = "red")
plt.ylabel("Counts")
plt.xlabel('Time between events (days)',labelpad=35)
plt.title("Histogram of time between adjacent events")
#plt.savefig("timegap_curve_normal.png") #histogram of event energies for 7.5 MeV
plt.subplots_adjust(bottom=0.15)
plt.show()
plt.gcf().clear()
"""

"""
#Time between events with curve for times less than 10 days
plt.hold(True)
xs, ys = histogramtest.smooth_points(timediff_10day, 5, 500, 10)
fig, ax = plt.subplots()
counts, bins, patches = ax.hist(timediff_10day, 10)#bins = [0,10, 20, 30, 40,50, 60, 70,80,90, 100, 110,120,130, 140, 150,160, 170,180,190, 200, 210,220, 230])
ax.set_xticks(bins)
# Label the raw counts and the percentages below the x-axis...
bin_centers = 0.5 * np.diff(bins) + bins[:-1]
for count, x in zip(counts, bin_centers):
    # Label the raw counts
    ax.annotate(str(int(count)), xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -18), textcoords='offset points', va='top', ha='center')

    # Label the percentages
    percent = '%0.0f%%' % (100 * float(count) / counts.sum())
    ax.annotate(percent, xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -32), textcoords='offset points', va='top', ha='center')
plt.ylim(-2.5,20)
plt.plot(xs, ys, lw= 6, color="red")
plt.scatter(timediff_10day, [-1]*len(timediff_10day), marker="x",linewidth='1', s = 50, color = "red")
plt.xlabel('Time between events',labelpad=35)
plt.title("Histogram of time between adjacent events")
#plt.savefig("timegap_curve_zoom.png") #histogram of event energies for 7.5 MeV
plt.subplots_adjust(bottom=0.15)
plt.show()
plt.gcf().clear()
"""
    
"""
xs, ys = histogramtest.smooth_points(timediff_days, 10)
plt.hold(True)
plt.hist(timediff_days, 20)
plt.plot(xs, ys)
#plt.scatter(timediff_days, [random.uniform(-5, 0) for _ in xrange(len(timediff_days))])
plt.show()
"""
    
#Event energies
""" 
plt.hist(field75[energy], 10)
plt.xlabel('Energy')
plt.title("Histogram of event energies for 7.5 MeV Threshold")
plt.savefig("energy75MeV_10bins.png") #histogram of event energies for 7.5 MeV
#plt.show()
plt.gcf().clear()
"""
"""
#Plot with labeling axes ranges
fig, ax = plt.subplots()
counts, bins, patches = ax.hist(field75[energy], 10)
ax.set_xticks(bins)
# Label the raw counts and the percentages below the x-axis...
bin_centers = 0.5 * np.diff(bins) + bins[:-1]
for count, x in zip(counts, bin_centers):
    # Label the raw counts
    ax.annotate(str(count), xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -18), textcoords='offset points', va='top', ha='center')

    # Label the percentages
    percent = '%0.0f%%' % (100 * float(count) / counts.sum())
    ax.annotate(percent, xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -32), textcoords='offset points', va='top', ha='center')

plt.xlabel('Neutrino Energy',labelpad=35)
plt.title("Histogram of event energies for 7.5 MeV Threshold")
#plt.savefig("energy75MeV_100bins.png") #histogram of event energies for 7.5 MeV
plt.subplots_adjust(bottom=0.15)
plt.show()
plt.gcf().clear()
"""

"""
#Plot with manual axis range labels
xs, ys = histogramtest.smooth_points(field75[energy], 5, 500, 11)
fig, ax = plt.subplots()
counts, bins, patches = ax.hist(field75[energy], bins=[7.5, 17.5, 27.5, 37.5, 47.5, 57.5, 67.5, 77.5, 87.5, 97.5, 107.5])
#bins=[7.5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110]
ax.set_xticks(bins)
# Label the raw counts and the percentages below the x-axis...
bin_centers = 0.5 * np.diff(bins) + bins[:-1]
for count, x in zip(counts, bin_centers):
    # Label the raw counts
    ax.annotate(str(int(count)), xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -18), textcoords='offset points', va='top', ha='center')

    # Label the percentages
    percent = '%0.0f%%' % (100 * float(count) / counts.sum())
    ax.annotate(percent, xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -32), textcoords='offset points', va='top', ha='center')
plt.ylim(-2.5,25)
plt.plot(xs,ys, lw = 6, color = "red")
plt.ylabel("Counts")
plt.xlabel('Neutrino Energy (MeV)',labelpad=35)
plt.title("Event energies for 7.5 MeV Threshold")  #histogram of event energies for 7.5 MeV
#plt.scatter(field75[energy], [random.uniform(-5, 0.5) for _ in xrange(len(field75[energy]))])
#plt.scatter(field75[energy], [[-1,  -2, -3, -4][i%4] for i in xrange(len(field75[energy]))])
#plt.scatter(field75[energy], [-1*i for i in field75[energy]])
plt.scatter(field75[energy], [-1]*len(field75[energy]), marker="x",linewidth='1', s = 50, color = "red")
plt.subplots_adjust(bottom=0.15)
#plt.savefig("energyhist_curve.png")
plt.show()
plt.gcf().clear()
"""

"""
plt.yscale("log",nonposy='clip')
plt.hist(field09[energy], 100)
plt.xlabel('Energy')
print "Maximum 09[energy]:", max(field09[energy])
plt.title("Histogram of event energies for 0.9 MeV Threshold")
plt.savefig("energy0.9MeV.png") #histogram of event energies for 0.9 MeV
plt.gcf().clear()

"""

"""
#Distance between prompt and delayed events
distance = [((x1 - x2)**2 + (y1-y2)**2 + (z1-z2)**2)**0.5 for x1, x2, y1, y2, z1, z2 in zip(field75[xp], field75[xd], field75[yp], field75[yd], field75[zp], field75[zd])]
plt.hist(distance, 100)
plt.xlabel('Distance')
plt.title("Histogram of distance between prompt and delayed events for 7.5 MeV Threshold")
plt.savefig("Distance7.5MeV.png")
plt.gcf().clear()

distance = [((x1 - x2)**2 + (y1-y2)**2 + (z1-z2)**2)**0.5 for x1, x2, y1, y2, z1, z2 in zip(field09[xp], field09[xd], field09[yp], field09[yd], field09[zp], field09[zd])]
plt.hist(distance, 100)
plt.xlabel('Distance')
plt.title("Histogram of distance between prompt and delayed events for 0.9 MeV Threshold")
plt.savefig("Distance0.9MeV.png")
plt.gcf().clear()
"""

#Heat map of events

"""
#0.9 MeV x and y prompt 
heatmap, xedges, yedges = np.histogram2d(field09[xp], field09[yp], bins=200)
extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
plt.clf()
plt.xlabel('X')
plt.ylabel('Y')
plt.imshow(heatmap, extent=extent)
plt.title("Heatmap of prompt events for 0.9MeV threshold (x vs y)")
plt.savefig("heatmap_xy_prompt_0.9.png")
plt.gcf().clear()

#0.9 MeV x and z prompt
heatmap, xedges, yedges = np.histogram2d(field09[xp], field09[zp], bins=200)
extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
plt.clf()
plt.xlabel('X')
plt.ylabel('Z')
plt.imshow(heatmap, extent=extent)
plt.title("Heatmap of prompt events for 0.9MeV threshold (x vs z)")
plt.savefig("heatmap_xz_prompt_0.9.png")
plt.gcf().clear()

#0.9 MeV y and z prompt
heatmap, xedges, yedges = np.histogram2d(field09[yp], field09[zp], bins=200)
extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
plt.clf()
plt.xlabel('Y')
plt.ylabel('Z')
plt.imshow(heatmap, extent=extent)
plt.title("Heatmap of prompt events for 0.9MeV threshold (y vs z)")
plt.savefig("heatmap_yz_prompt_0.9.png")
plt.gcf().clear()

"""





"""
#3d plot of location of all prompt events 
plt.hold(True)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(field09[xp], field09[yp], field09[zp], s=1,lw=0)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
#plt.title("Locations of prompt events for 0.9MeV threshold") 
#plt.show()
#plt.savefig("3d_scatterplot_dawg.png")
"""
"""
#3d plot of 7.5 MeV events only
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(field75[xp], field75[yp], field75[zp], c="r", marker="o")
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.title("Detector locations of prompt events for 7.5 MeV threshold")
#plt.title("Locations of prompt events for 0.9 MeV threshold; 7.5 MeV threshold in red") 
#plt.show()
plt.savefig("PromptLocation3d75.png")
#plt.hold(False)
plt.gcf().clear()
"""



'''

#Plot of distance between prompt and delayed event

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
xdist = [x1 - x2 for x1, x2, in zip(field75[xp], field75[xd])]
ydist = [y1 - y2 for y1, y2, in zip(field75[yp], field75[yd])]
zdist = [z1 - z2 for z1, z2, in zip(field75[zp], field75[zd])]
ax.scatter(xdist, ydist, zdist, s=1,lw=0)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.title("Distance between prompt event and delayed event for 7.5 MeV") 
#plt.show()
plt.savefig("DistancePromptDelayed7.5_3d.png")
plt.gcf().clear()

def average(l):
	return sum(l) / float(len(l))

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
xdist = [x1 - x2 for x1, x2, in zip(field09[xp], field09[xd])]
ydist = [y1 - y2 for y1, y2, in zip(field09[yp], field09[yd])]
zdist = [z1 - z2 for z1, z2, in zip(field09[zp], field09[zd])]
print "Averages:", map(average, [xdist, ydist, zdist])
ax.scatter(xdist, ydist, zdist, s=1,lw=0)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.title("Distance between prompt event and delayed event for 0.9 MeV") 
plt.show()
plt.savefig("DistancePromptDelayed0.9_3d.png")
plt.gcf().clear()

'''


#Plotting time of events 

"""
plt.scatter(datetimeGW, fieldGW[energy], s=2, lw=0)
plt.xlabel('Time')
plt.ylabel('Event Energy (MeV)')
plt.title("Event energy vs time for Fall 2015 events (0.9 MeV - 100 MeV)")
plt.savefig("./plots/TimeEnergy0.9MeV_GW_1.png")
plt.gcf().clear()



plt.scatter(datetimeGW, fieldGW[energy], s=30, marker = "x") #s=30, lw=0,
plt.ylim([0,110])
plt.xlabel('Time of Event')
plt.ylabel('Event Energy (MeV)')
plt.title("Event energy vs time for Fall 2015 events (0.9 MeV - 100 MeV)")
plt.savefig("./plots/TimeEnergy0.9MeV_GW_bigdots.png")
#plt.show()
plt.gcf().clear()
"""
"""
#Plotting time of events with LIGO event line printed on top
plt.scatter(datetimeGWhe, fieldGWhe[energy], s=30, marker = "x") #s=30, lw=0,
plt.ylim([0,4200])
plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
plt.xlabel('Time of Event')
plt.ylabel('Event Energy (MeV)')
plt.title("All energies. GW150914 and LVT151012 marked.")
plt.savefig("./plots/TimeEnergy0.9MeV_GW_he_withBothGW_v2.png")
#plt.show()
plt.gcf().clear()

plt.scatter(datetimeGWnomini, fieldGWnomini[energy], s=30, marker = "x") #s=30, lw=0,
plt.ylim([0,4200])
plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
plt.xlabel('Time of Event')
plt.ylabel('Event Energy (MeV)')
#plt.title("Event energy vs time for Fall 2015 events (all energies) without miniballoon cut. GW150914 and LVT151012 marked.")
plt.title("No miniballoon cut")
plt.savefig("./plots/TimeEnergy0.9MeV_GW_nomini_withBothGW.png")
#plt.show()
plt.gcf().clear()

plt.scatter(datetimeGWnolike, fieldGWnolike[energy], s=30, marker = "x") #s=30, lw=0,
plt.ylim([0,4200])
plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
plt.xlabel('Time of Event')
plt.ylabel('Event Energy (MeV)')
#plt.title("Event energy vs time for Fall 2015 events (all energies) without miniballoon cut or likelihood selection. GW150914 and LVT151012 marked.")
plt.title("No miniballoon cut or likelihood selection")
plt.savefig("./plots/TimeEnergy0.9MeV_GW_nolike_withBothGW.png")
#plt.show()
plt.gcf().clear()
"""

"""
#Plot of events with energy less than 30 MeV
midenergy = []
midenergyTime = []
for i in xrange(len(field75[energy])):
    if field75[energy][i] <= 30:
        midenergy.append(field75[energy][i])
        midenergyTime.append(datetime75[i])
print len(midenergy)
     
plt.scatter(midenergyTime, midenergy, s=30, marker = "x") #s=30, lw=0,
plt.ylim([0,35])
plt.xlabel('Time of Event')
plt.ylabel('Event Energy (MeV)')
plt.title("Event energy vs time for 7.5 MeV < E < 30 MeV")
#plt.savefig("TimeEnergy7to30.png")
plt.show()
plt.gcf().clear()

"""
    
"""
#Plot of events in a shorter period of time
plt.scatter(field09[unixtime], field09[energy], s=2, lw=0) #How to select specific set of rows?
plt.xlabel('Time')
plt.ylabel('Energy')
plt.title("Event energy vs time for 0.9 MeV threshold")
plt.savefig("TimeEnergy0.9MeV_zoom.png")
plt.gcf().clear()
"""
#if field09[unixtime]

#Time range for each limit
#print "0.9:", min(field09[unixtime]), "to", max(field09[unixtime])
#print "GW:", min(fieldGWhe[unixtime]), "to", max(fieldGWhe[unixtime])
#print "GW range:", min(datetimeGWhe), "to", max(datetimeGWhe)

			  #plt.scatter(field[2], field[3])
			  #plt.savefig("output2.png")

