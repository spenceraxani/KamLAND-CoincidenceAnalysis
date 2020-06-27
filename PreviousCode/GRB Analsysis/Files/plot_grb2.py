import numpy as np
import ROOT,mydef,RootLogon
import time

## Cosmo parameters
H0 = 67.15 #km/s/Mpc
Omega_m = 0.317
Omega_d = 0.683
c = 299792.458 #km/s

A = np.loadtxt("GRBlongList.dat",dtype={'names':('name','utc','T90','fluence','Instrument','redshift'),'formats':('S8','S8','f8','f8','S8','f8')})

nList = A['name']
dList = A['Instrument']
uttime1 = []
uttime2 = []
uttime3 = []

for line in range(len(nList)):
    gtime = (int('20'+str(nList[line][0:2])),int(nList[line][2:4]),int(nList[line][4:6]),0,0,0,0,0,0)
    if dList[line]=='Swift':
        uttime1.append(time.mktime(gtime) + 60*60*9)
    elif dList[line]=='HETE':
        uttime2.append(time.mktime(gtime) + 60*60*9)
    else:
        uttime3.append(time.mktime(gtime) + 60*60*9)
        pass
    pass


uttime1 = np.array(uttime1)
uttime2 = np.array(uttime2)
uttime3 = np.array(uttime3)

Nbins = 500
xbinmin = 1009810800
xbinmax = 1356966000
h1=mydef.make_hist( uttime1, 'hoge1', Nbins, xbinmin, xbinmax, 2, 1, False)
h2=mydef.make_hist( uttime2, 'hoge2', Nbins, xbinmin, xbinmax, 2, 1, False)
h3=mydef.make_hist( uttime3, 'hoge3', Nbins, xbinmin, xbinmax, 2, 1, False)

xList = []
yList1 = []
yList2 = []
yList3 = []
YList1 = []
YList2 = []
YList3 = []
for line in range(Nbins):
    xList.append(h1.GetBinCenter(line))
    yList1.append(h1.GetBinContent(line))
    yList2.append(h2.GetBinContent(line))
    yList3.append(h3.GetBinContent(line))
    pass
xList = np.array(xList)
yList1 = np.array(yList1)
yList2 = np.array(yList2)
yList3 = np.array(yList3)
for line in range(Nbins):
    YList1.append(np.sum(yList1[:line]))
    YList2.append(np.sum(yList2[:line]))
    YList3.append(np.sum(yList3[:line]))
    pass
YList1 = np.array(YList1)
YList2 = np.array(YList2)
YList3 = np.array(YList3)

g1=mydef.make_graph( xList=xList, yList=YList1+YList2, name='hoge',
                     title='cumulative sum', xtitle='date', ytitle='Number',
                     iColor=1, iSize=3, iStyle=1,
                     errPlot=False,errxList=None, erryList=None, xmin=xbinmin, xmax=xbinmax, ymin=None, ymax=None)
g2=mydef.make_graph( xList=xList, yList=YList1+YList2+YList3, name='hoge',
                     title='cumulative sum', xtitle='date', ytitle='Number',
                     iColor=2, iSize=3, iStyle=1,
                     errPlot=False,errxList=None, erryList=None, xmin=xbinmin, xmax=xbinmax, ymin=None, ymax=None)
g3=mydef.make_graph( xList=xList, yList=YList2, name='hoge',
                     title='cumulative sum', xtitle='date', ytitle='Number',
                     iColor=4, iSize=3, iStyle=1,
                     errPlot=False,errxList=None, erryList=None, xmin=xbinmin, xmax=xbinmax, ymin=0, ymax=250)





c0 = ROOT.TCanvas('plot1', 'plot1', 1200, 900)
ROOT.gStyle.SetTimeOffset(-788918400)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)
g3.Draw('AL')
g1.Draw('L')
g2.Draw('L')

g3.GetXaxis().SetTimeDisplay(1)


#g3.GetXaxis().SetNdivisions(505)
#g3.GetXaxis().SetTimeFormat("%Y/%m");
g3.GetXaxis().SetTimeFormat("%Y/%m");
g3.GetXaxis().SetLabelSize(0.05)
g3.Draw()
