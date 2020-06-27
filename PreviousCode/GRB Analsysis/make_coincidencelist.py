import numpy as np
import ROOT,mydef,RootLogon
import time


## read GRB list
A = np.loadtxt("../grblist/GRBlongList.dat",dtype={'names':('name','utc','T90','fluence','Instrument','redshift'),'formats':('S8','S8','f8','f8','S8','f8')})   

nameList = A['name']

utcList = A['utc']
T90List = A['T90']
zList = A['redshift']

timeList = []
nameList_= []
for line in range(len(nameList)):
    gtime = (int('20'+str(nameList[line][0:2])),int(nameList[line][2:4]),int(nameList[line][4:6]),int(utcList[line][0:2]),int(utcList[line][3:5]),int(utcList[line][6:8]),0,0,0)
    timeList.append(time.mktime(gtime) + 60*60*9)
    nameList_.append(nameList[line][0:6])
    pass

timeList = np.array(timeList)
            
## Check run grade and period
f= open('/data/kamland/const/run-quality/run-info.table', 'r')

tsList  = []
dt = []
runList = []
flag = []
for line in f:
    itemList = line.split()
    runList.append(int(itemList[0]))
    dt.append(float(itemList[2]))
    tsList.append(int(itemList[3]))
    flag.append(int(itemList[5]))
    pass

runList = np.array(runList)
tsList = np.array(tsList)
dt = np.array(dt)
flag = np.array(flag)


nameList1 = []
timeList1 = []
runList1 = []
gradeList1 = []
T90List1 = []
zList1 = []
pList1 = []

for line in range(len(timeList)):
    for line2 in range(len(runList)):
        if flag[line2] < 10:
            if tsList[line2]+60*10< timeList[line] and tsList[line2]+dt[line2]*60*60 -60*5-T90List[line] > timeList[line]:
                if runList[line2] >= 1313 and runList[line2] < 6820:
                    #print '%s %s %s %s 1' %(nameList[line],timeList[line],runList[line2],flag[line2])
                    nameList1.append(nameList_[line])
                    timeList1.append(timeList[line])
                    T90List1.append(T90List[line])
                    zList1.append(zList[line])
                    runList1.append(runList[line2])
                    gradeList1.append(flag[line2])
                    pList1.append(1)

                elif runList[line2] >= 6980 and runList[line2] < 10675:
                    nameList1.append(nameList_[line])
                    timeList1.append(timeList[line])
                    T90List1.append(T90List[line])
                    zList1.append(zList[line])
                    runList1.append(runList[line2])
                    gradeList1.append(flag[line2])
                    pList1.append(2)

                elif runList[line2] >= 11000:#  and runList[line2] < 11648:
                    nameList1.append(nameList_[line])
                    timeList1.append(timeList[line])
                    T90List1.append(T90List[line])
                    zList1.append(zList[line])
                    runList1.append(runList[line2])
                    gradeList1.append(flag[line2])
                    pList1.append(3)

                    pass
                pass
            pass
        pass
    pass
pList1 = np.array(pList1)

print 'Num of GRBs (Period1) %s' %len(pList1[np.where(pList1==1)])
print 'Num of GRBs (Period2) %s' %len(pList1[np.where(pList1==2)])
print 'Num of GRBs (Period3) %s' %len(pList1[np.where(pList1==3)])
    
    
## Definition coincidence window
dat = np.loadtxt('/data/work/koji/GRB/tool/delaylist.dat')


z = dat[:,0]
delaytime = dat[:,1]

etimeList1 = []
for line in range(len(timeList1)):
    etimeList1.append(timeList1[line] + T90List1[line] + delaytime[np.argmin(abs(zList1[line] - z))] )
    pass

offsettime = 150 #sec

## Save data
f = open('coincidence_time.dat', 'w')
for line in range(len(timeList1)):
    f.writelines( '%s %s %s %s %s %s %s %s' %(timeList1[line]-offsettime,etimeList1[line]+offsettime,T90List1[line],zList1[line],runList1[line],gradeList1[line],pList1[line],nameList1[line]) + str('\n'))    
    #print '%s' %zList1[line]
    pass
f.close


## Definition coincidence window
dat = np.loadtxt('/data/work/koji/GRB/tool/delaylist_0.9.dat')
z = dat[:,0]
delaytime = dat[:,1]

etimeList1 = []
for line in range(len(timeList1)):
    etimeList1.append(timeList1[line] + T90List1[line] + delaytime[np.argmin(abs(zList1[line] - z))] )
    pass

#offsettime = 150 #sec
offsettime = 60*60*6 #sec

## Save data
f = open('coincidence_time_0.9.dat', 'w')
for line in range(len(timeList1)):
    f.writelines( '%s %s %s %s %s %s %s %s' %(timeList1[line]-offsettime,etimeList1[line]+offsettime,T90List1[line],zList1[line],runList1[line],gradeList1[line],pList1[line],nameList1[line]) + str('\n'))    
    #print '%s' %zList1[line]
    pass
f.close

## Save data
f = open('GRBlongUsedList.dat', 'w')
for line in range(len(timeList1)):
    
    f.writelines( '%s' %zList1[line] + str('\n'))    
    
    pass
f.close

