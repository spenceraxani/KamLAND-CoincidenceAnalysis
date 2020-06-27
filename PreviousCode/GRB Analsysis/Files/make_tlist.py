import numpy as np
import ROOT,mydef,RootLogon
import time

## Cosmo parameters
H0 = 67.15 #km/s/Mpc
Omega_m = 0.317
Omega_d = 0.683
c = 299792.458 #km/s

#A = np.loadtxt("GRBlongList.dat",dtype={'names':('name','utc','T90','fluence','Instrument','redshift'),'formats':('S8','S8','f8','f8','S8','f8')})
A = np.loadtxt("GRBshortList.dat",dtype={'names':('name','utc','T90','Instrument','redshift'),'formats':('S8','S8','f8','S8','f8')})




nList1 = A['name']
nList2 = A['utc']
nList3 = A['redshift']
uttime = []

for line in range(len(nList1)):
    gtime = (int('20'+str(nList1[line][0:2])),int(nList1[line][2:4]),int(nList1[line][4:6]),int(nList2[line][0:2]),int(nList2[line][3:5]),int(nList2[line][6:8]),0,0,0)
    print '%s %s' %(nList1[line], time.mktime(gtime) + 60*60*9)
    #uttime.append(time.mktime(gtime) + 60*60*9)
    pass


