import numpy as np
import ROOT,mydef,RootLogon
import time


dat = np.loadtxt('coincidence_time_0.9.dat')

timeList = dat[:,0]
etimeList = dat[:,1]
runList = dat[:,4]
pList = dat[:,6]

timeList1  = timeList[np.where(pList==1)]
etimeList1 = etimeList[np.where(pList==1)]
runList1   = runList[np.where(pList==1)]

timeList2  = timeList[np.where(pList==2)]
etimeList2 = etimeList[np.where(pList==2)]
runList2   = runList[np.where(pList==2)]

timeList3  = timeList[np.where(pList==3)]
etimeList3 = etimeList[np.where(pList==3)]
runList3   = runList[np.where(pList==3)]

#edat = np.loadtxt('../kam_data/Observed-dcvf-Kat.dat')
edat = np.loadtxt('../kam_data/Observed-dcvf-Kat_0.9MeV.dat')
#edat = np.loadtxt('../kam_data/Observed-dcvf-Kat_0.9MeVwithLikelihood.dat') 
erunList = edat[:,0]
EpList   = edat[:,1]
utimeList= edat[:,2]


## Period 1
for line in range(len(timeList1)):
    for line2 in range(len(utimeList)):
        if timeList1[line] <= utimeList[line2] and utimeList[line2] <= etimeList1[line]:
            print "run %s %s" %(runList1[line],erunList[line2])
            print "DC event: %s" %(utimeList[line2])
            print "GRB window: %s - %s" %(timeList1[line],etimeList1[line])
            pass
        if runList1[line]==erunList[line2]:
            #print "coincidence run %s %s" %(runList1[line],erunList[line2])
            #
            pass
        pass
    
    pass

## Period 2
for line in range(len(timeList2)):
    for line2 in range(len(utimeList)):
        if timeList2[line] <= utimeList[line2] and utimeList[line2] <= etimeList2[line]:
            print "run %s %s" %(runList2[line],erunList[line2])
            print "DC event: %s" %(utimeList[line2])
            print "GRB window: %s - %s" %(timeList2[line],etimeList2[line])
            pass
        if runList2[line]==erunList[line2]:
            #print "coincidence run %s %s" %(runList2[line],erunList[line2])
            
            pass
        pass
    pass

## Period 3
for line in range(len(timeList3)):
    for line2 in range(len(utimeList)):
        if timeList3[line] <= utimeList[line2] and utimeList[line2] <= etimeList3[line]:
            print "Perido3"
            print "run %s %s" %(runList3[line],erunList[line2])
            print "DC event: %s" %(utimeList[line2])
            print "GRB window: %s - %s" %(timeList3[line],etimeList3[line])
            pass
        if runList3[line]==erunList[line2]:
            #print "coincidence run %s %s" %(runList3[line],erunList[line2])
            pass
        pass
    pass


