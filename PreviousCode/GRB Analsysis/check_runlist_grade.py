import numpy as np
import ROOT,mydef,RootLogon
import time

## read GRB list
A = np.loadtxt("../grblist/GRBlongList.dat",dtype={'names':('name','utc','T90','fluence','Instrument','redshift'),'formats':('S8','S8','f8','f8','S8','f8')})   

nameList = A['name']
utcList = A['utc']
zList = A['redshift']

timeList = []

for line in range(len(nameList)):
    gtime = (int('20'+str(nameList[line][0:2])),int(nameList[line][2:4]),int(nameList[line][4:6]),int(utcList[line][0:2]),int(utcList[line][3:5]),int(utcList[line][6:8]),0,0,0)
    timeList.append(time.mktime(gtime) + 60*60*9)
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

nameList2 = []
timeList2 = []
runList2 = []
gradeList2 = []

nameList3 = []
timeList3 = []
runList3 = []
gradeList3 = []
for line in range(len(timeList)):
    EventFound = 0
    for line2 in range(len(runList)):
        if flag[line2] < 10:
            if tsList[line2]+60*10 < timeList[line] and tsList[line2]+dt[line2]*60*60 -60*10 > timeList[line]:
                if runList[line2] >= 1313 and runList[line2] < 6820:
                    print '%s %s %s %s 1' %(nameList[line],timeList[line],runList[line2],flag[line2])
                    nameList1.append(nameList[line])
                    timeList1.append(timeList[line])
                    runList1.append(runList[line2])
                    gradeList1.append(flag[line2])
                    EventFound = 1
                elif runList[line2] >= 6980 and runList[line2] < 10675:
                    print '%s %s %s %s 2' %(nameList[line],timeList[line],runList[line2],flag[line2])
                    nameList2.append(nameList[line])
                    timeList2.append(timeList[line])
                    runList2.append(runList[line2])
                    gradeList2.append(flag[line2])
                    EventFound = 1
                elif runList[line2] >= 11000  and runList[line2] < 11648:
                    print '%s %s %s %s 3' %(nameList[line],timeList[line],runList[line2],flag[line2])
                    nameList3.append(nameList[line])
                    timeList3.append(timeList[line])
                    runList3.append(runList[line2])
                    gradeList3.append(flag[line2])
                    EventFound = 1
                else:
                    print '%s %s %s %s 0' %(nameList[line],timeList[line],runList[line2],flag[line2])
                    EventFound = 1
                    pass
                pass
            pass
        pass
    if  EventFound == 0:
        print '%s %s 0 0 0' %(nameList[line],timeList[line])
    pass
