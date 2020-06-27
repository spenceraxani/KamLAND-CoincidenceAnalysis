import numpy as np

dat = np.loadtxt('/data/kamland/const/live-time/livetime-6.0m.dat')
LiveRun   = dat[:,0]
LiveRatio = dat[:,4]


### Period 1 and 2 #####################################################
dat = np.loadtxt('coincidence_time.dat')

timeList = dat[:,0]
etimeList = dat[:,1]
runList = dat[:,4]
pList = dat[:,6]

runList1 = runList[np.where(pList==1)]
dtList1  = etimeList[np.where(pList==1)] - timeList[np.where(pList==1)]

runList2 = runList[np.where(pList==2)]
dtList2  = etimeList[np.where(pList==2)] - timeList[np.where(pList==2)]

RList1 = []
for line in range(len(runList1)):
    for line2 in range(len(LiveRun)):
        if runList1[line]==LiveRun[line2]:
            RList1.append(LiveRatio[line2])
            pass
        pass
    pass

RList2 = []
for line in range(len(runList2)):
    for line2 in range(len(LiveRun)):
        if runList2[line]==LiveRun[line2]:
            RList2.append(LiveRatio[line2])
            pass
        pass
    pass


RList1 = np.array(RList1)
RList2 = np.array(RList2)


print 'Total window time (Period1): %s h' %np.sum(dtList1/60/60)
print 'Total window time (Period2): %s h' %np.sum(dtList2/60/60)
print 'Total live time (Period1): %s h' %np.sum(RList1*dtList1/60/60)
print 'Total live time (Period2): %s h' %np.sum(RList2*dtList2/60/60)
print 'Total live time (Period 1 and 2): %s' %(np.sum(RList1*dtList1/60/60) + np.sum(RList2*dtList2/60/60))



### Period 3 #####################################################
dat = np.loadtxt('coincidence_time_0.9.dat')
#dat = np.loadtxt('coincidence_time.dat')

timeList = dat[:,0]
etimeList = dat[:,1]
runList = dat[:,4]
pList = dat[:,6]

runList3 = runList[np.where(pList==3)]
dtList3  = etimeList[np.where(pList==3)] - timeList[np.where(pList==3)]

RList3 = []
for line in range(len(runList3)):
    for line2 in range(len(LiveRun)):
        if runList3[line]==LiveRun[line2]:
            RList3.append(LiveRatio[line2])
            pass
        pass
    pass

RList3 = np.array(RList3)
print 'Total window time (Period3): %s h' %np.sum(dtList3/60/60)
print 'Total live time (Period3): %s h' %np.sum(RList3*dtList3/60/60)
