
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
import matplotlib.dates
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
fieldGWnolike = read_file("Observed-dcvf-Kat-GW-100MeV_noMiniballoon-noLikelihood.dat") #Events from Fall 2015 without miniballoon cut or likelihood selection
fieldGWSepOct = read_file("Observed-dcvf-Kat-GW-100MeV_noMiniballoon-noLikelihood_SepOct.dat")

run, energy, unixtime, eventnumber, delayed_eventnum, xp, yp, zp, xd, yd, zd, run_grade, multiplicity, dt, dl = range(15)

## Datetime of LIGO event GW150914
GW150914 = datetime.datetime(2015,9,14,9,50,45)   #GPS time: 1126259462.39; UTC:2015-09-14T09:50:45
#GW150914utc = int(GW150914.strftime("%s"))
GW150914utc = 1442224245 # By Koji
print "GW150914 time:", GW150914utc

## Datetime of LIGO event LVT151012  #October 12, 2015 at 09:54:43 UTC
LVT151012 = datetime.datetime(2015,10,12,9,54,43)
LVT151012utc = int(LVT151012.strftime("%s"))
print "LVT151012 time:", LVT151012utc

datetimeGW = []
for i in fieldGW[unixtime]:
    datetimeGW.append(i)

datetimeGWhe = []
for i in fieldGWhe[unixtime]:
    datetimeGWhe.append(i)
    
datetimeGWnomini = []
for i in fieldGWnomini[unixtime]:
    datetimeGWnomini.append(i)
    
datetimeGWnolike = []
for i in fieldGWnolike[unixtime]:
    datetimeGWnolike.append(i)

    
datetimeGWSepOct = []
for i in fieldGWSepOct[unixtime]:
    datetimeGWSepOct.append(i)


## Events within some window of GW 
GW09_energy_list = []
GW09_time_list = []
window = 60*60*24*1  #seconds in a day
for i in range(len(fieldGWSepOct[run])):
	if abs(GW150914utc - fieldGWSepOct[unixtime][i])<window:
		GW09_energy_list.append(fieldGWSepOct[energy][i])
		GW09_time_list.append(fieldGWSepOct[unixtime][i])
print "list of energies: ", GW09_energy_list
print "list of times", GW09_time_list
print "maximum energy for GW150914", max(GW09_energy_list)

datetimeGWtimelist = []
for i in GW09_time_list:
	datetimeGWtimelist.append(datetime.datetime.fromtimestamp(i))

LVT10_energy_list = []
LVT10_time_list = []
window = 60*60*24*1  #seconds in a day
for i in range(len(fieldGWSepOct[run])):
	if abs(LVT151012utc - fieldGWSepOct[unixtime][i])<window:
		LVT10_energy_list.append(fieldGWSepOct[energy][i])
		LVT10_time_list.append(fieldGWSepOct[unixtime][i])
#print "list of energies for LVT10: ", LVT10_energy_list
#print "list of times for LVT10", LVT10_time_list
print "maximum energy for LVT151012", max(LVT10_energy_list)

datetimeGWtimelist2 = []
for i in LVT10_time_list:
	datetimeGWtimelist2.append(datetime.datetime.fromtimestamp(i))

print "max energy", max(fieldGWSepOct[energy])
#print "min energy", min(fieldGWhe[energy])

print "num events between 0.9 and 100 MeV", len(fieldGW[eventnumber])
print "num events of any energy", len(fieldGWhe[eventnumber])
print "num events without miniballon cut", len(fieldGWnomini[eventnumber])
print "num events without miniballon or likelihood", len(fieldGWnolike[eventnumber])


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

  



#Plotting time with LIGO events zoomed in on Sep Oct - GW150914

matplotlib.rcParams.update({'font.size': 17})
plt.scatter(datetimeGWtimelist,GW09_energy_list , s=30, marker = "x") #s=30, lw=0,
plt.ylim([1,4])
plt.xlim([datetime.date(2015,9,14), datetime.date(2015,9,15)])
plt.plot([GW150914, GW150914], [1, 4], color='k', linestyle='-', linewidth=2)

ax=plt.gca()
xfmt = matplotlib.dates.DateFormatter('%H')#('%Y-%m-%d %H:%M:%S')
ax.xaxis.set_major_formatter(xfmt)
plt.xlabel('Time of Event (Date: 14 Sep 2015)')
plt.ylabel('Event Energy (MeV)')
plt.text(datetime.datetime(2015,9,14,9),3, "GW150914", rotation='vertical')
#plt.title("Neutrino events between 0.9-100 MeV.  GW150914 marked.")
plt.title("Events between 0.9-100 MeV")
plt.savefig("./plots/TimeEnergy09MeV_noMini_noLike_GW150914only_v8.eps", format = "eps", dpi=1000)
#plt.savefig("../LIGOpaper/Figures_ligopaper/TimeEnergy09MeV_noMini_noLike_GW150914only_v7.eps", format = "eps", dpi=1000)
plt.show()
plt.gcf().clear()

#Plotting time with LIGO events zoomed in on Sep Oct - LVT151012

plt.scatter(datetimeGWtimelist2,LVT10_energy_list , s=30, marker = "x") #s=30, lw=0,
plt.ylim([1,4])
plt.xlim([datetime.date(2015,10,12), datetime.date(2015,10,13)])
#plt.plot([GW150914, GW150914], [1, 4], color='k', linestyle='-', linewidth=2)
plt.plot([LVT151012, LVT151012], [1, 4], color='k', linestyle='-', linewidth=2)
ax=plt.gca()
xfmt = matplotlib.dates.DateFormatter('%H')#('%Y-%m-%d %H:%M:%S')
ax.xaxis.set_major_formatter(xfmt)
#plt.xlabel('Date of Event (Oct 2015)')
plt.xlabel('Time of Event (Date: 12 Oct 2015)')
plt.ylabel('Event Energy (MeV)')
plt.text(datetime.datetime(2015,10,12,9),3.5, "LVT151012", rotation='vertical')
#plt.title("Neutrino events between 0.9-100 MeV.  LVT151012 marked.")
plt.title("Events between 0.9-100 MeV")
plt.savefig("./plots/TimeEnergy0.9MeV_noMini_noLike_LVT151012only_v6.png")
plt.show()
plt.gcf().clear()



#plt.scatter(datetimeGWnomini, fieldGWnomini[energy], s=30, marker = "x") #s=30, lw=0,
#plt.ylim([0,4200])
#plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.xlabel('Time of Event')
#plt.ylabel('Event Energy (MeV)')
##plt.title("Event energy vs time for Fall 2015 events (all energies) without miniballoon cut. GW150914 and LVT151012 marked.")
#plt.title("No miniballoon cut")
#plt.savefig("./plots/TimeEnergy0.9MeV_GW_nomini_withBothGW.png")
##plt.show()
#plt.gcf().clear()
#
#plt.scatter(datetimeGWnolike, fieldGWnolike[energy], s=30, marker = "x") #s=30, lw=0,
#plt.ylim([0,4200])
#plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.xlabel('Time of Event')
#plt.ylabel('Event Energy (MeV)')
##plt.title("Event energy vs time for Fall 2015 events (all energies) without miniballoon cut or likelihood selection. GW150914 and LVT151012 marked.")
#plt.title("No miniballoon cut or likelihood selection")
#plt.savefig("./plots/TimeEnergy0.9MeV_GW_nolike_withBothGW.png")
##plt.show()
#plt.gcf().clear()
#
##Plot of events with energy less than 30 MeV
#midenergy = []
#midenergyTime = []
#for i in xrange(len(field75[energy])):
#    if field75[energy][i] <= 30:
#        midenergy.append(field75[energy][i])
#        midenergyTime.append(datetime75[i])
#print len(midenergy)
#     
#plt.scatter(midenergyTime, midenergy, s=30, marker = "x") #s=30, lw=0,
#plt.ylim([0,35])
#plt.xlabel('Time of Event')
#plt.ylabel('Event Energy (MeV)')
#plt.title("Event energy vs time for 7.5 MeV < E < 30 MeV")
##plt.savefig("TimeEnergy7to30.png")
#plt.show()
#plt.gcf().clear()
#
#    
##Plot of events in a shorter period of time
#plt.scatter(field09[unixtime], field09[energy], s=2, lw=0) #How to select specific set of rows?
#plt.xlabel('Time')
#plt.ylabel('Energy')
#plt.title("Event energy vs time for 0.9 MeV threshold")
#plt.savefig("TimeEnergy0.9MeV_zoom.png")
#plt.gcf().clear()
##if field09[unixtime]

#Time range for each limit
#print "0.9:", min(field09[unixtime]), "to", max(field09[unixtime])
#print "GW:", min(fieldGWhe[unixtime]), "to", max(fieldGWhe[unixtime])
#print "time range:", min(datetimeGW), "to", max(datetimeGW)

			  #plt.scatter(field[2], field[3])
			  #plt.savefig("output2.png")

#plt.scatter(datetimeGWnomini, fieldGWnomini[energy], s=30, marker = "x") #s=30, lw=0,
#plt.ylim([0,4200])
#plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.xlabel('Time of Event')
#plt.ylabel('Event Energy (MeV)')
##plt.title("Event energy vs time for Fall 2015 events (all energies) without miniballoon cut. GW150914 and LVT151012 marked.")
#plt.title("No miniballoon cut")
#plt.savefig("./plots/TimeEnergy0.9MeV_GW_nomini_withBothGW.png")
##plt.show()
#plt.gcf().clear()
#
#plt.scatter(datetimeGWnolike, fieldGWnolike[energy], s=30, marker = "x") #s=30, lw=0,
#plt.ylim([0,4200])
#plt.plot([GW150914, GW150914], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.plot([LVT151012, LVT151012], [0, 4200], color='k', linestyle='-', linewidth=2)
#plt.xlabel('Time of Event')
#plt.ylabel('Event Energy (MeV)')
##plt.title("Event energy vs time for Fall 2015 events (all energies) without miniballoon cut or likelihood selection. GW150914 and LVT151012 marked.")
#plt.title("No miniballoon cut or likelihood selection")
#plt.savefig("./plots/TimeEnergy0.9MeV_GW_nolike_withBothGW.png")
##plt.show()
#plt.gcf().clear()
#
##Plot of events with energy less than 30 MeV
#midenergy = []
#midenergyTime = []
#for i in xrange(len(field75[energy])):
#    if field75[energy][i] <= 30:
#        midenergy.append(field75[energy][i])
#        midenergyTime.append(datetime75[i])
#print len(midenergy)
#     
#plt.scatter(midenergyTime, midenergy, s=30, marker = "x") #s=30, lw=0,
#plt.ylim([0,35])
#plt.xlabel('Time of Event')
#plt.ylabel('Event Energy (MeV)')
#plt.title("Event energy vs time for 7.5 MeV < E < 30 MeV")
##plt.savefig("TimeEnergy7to30.png")
#plt.show()
#plt.gcf().clear()
#
#    
##Plot of events in a shorter period of time
#plt.scatter(field09[unixtime], field09[energy], s=2, lw=0) #How to select specific set of rows?
#plt.xlabel('Time')
#plt.ylabel('Energy')
#plt.title("Event energy vs time for 0.9 MeV threshold")
#plt.savefig("TimeEnergy0.9MeV_zoom.png")
#plt.gcf().clear()
##if field09[unixtime]

#Time range for each limit
#print "0.9:", min(field09[unixtime]), "to", max(field09[unixtime])
#print "GW:", min(fieldGWhe[unixtime]), "to", max(fieldGWhe[unixtime])
print "time range:", min(datetimeGW), "to", max(datetimeGW)

			  #plt.scatter(field[2], field[3])
			  #plt.savefig("output2.png")

