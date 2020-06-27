import numpy as np
import mydef,ROOT

dat = np.loadtxt("Observed-dcvf-Kat.dat")

runList0 = dat[:,0]
EpList0  = dat[:,1]
rpx0     = dat[:,5]
rpy0     = dat[:,6]
rpz0     = dat[:,7]
rdx0     = dat[:,8]
rdy0     = dat[:,9]
rdz0     = dat[:,10]
gList0   = dat[:,11]
multiList = dat[:,12]
dt0 = dat[:,13]
dr0 = dat[:,14] 

runList = []
EpList  = []
rpx     = []
rpy     = []
rpz     = []
rdx     = []
rdy     = []
rdz     = []
gList   = []
dt      = []
dr      = []

for line in range(len(runList0)):
    if multiList[line] == 1:
        runList.append(runList0[line])
        EpList.append(EpList0[line])
        rpx.append(rpx0[line])
        rpy.append(rpy0[line])
        rpz.append(rpz0[line])
        rdx.append(rdx0[line])
        rdy.append(rdy0[line])
        rdz.append(rdz0[line])
        gList.append(gList0[line])
        dt.append(dt0[line])
        dr.append(dr0[line])
        FirstEvent = 0
        pass
    elif multiList[line] > 1 and FirstEvent == 0:
        runList.append(runList0[line])
        EpList.append(EpList0[line])
        rpx.append(rpx0[line])
        rpy.append(rpy0[line])
        rpz.append(rpz0[line])
        rdx.append(rdx0[line])
        rdy.append(rdy0[line])
        rdz.append(rdz0[line])
        dt.append(dt0[line])
        dr.append(dr0[line])
        gList.append(gList0[line])
        FirstEvent = multiList[line] - 1
    else:
        FirstEvent = FirstEvent - 1
        pass
    pass

runList = np.array(runList)
EpList  = np.array(EpList)
rpx     = np.array(rpx)/100
rpy     = np.array(rpy)/100
rpz     = np.array(rpz)/100
rdx     = np.array(rdx)/100
rdy     = np.array(rdy)/100
rdz     = np.array(rdz)/100
dt      = np.array(dt)
dr      = np.array(dr)


Ep30 = 30.0#MeV
EpList1 = EpList[np.where(EpList <= Ep30)]
runList1 = runList[np.where(EpList <= Ep30)]
dr1 = dr[np.where(EpList <= Ep30)]
dt1 = dt[np.where(EpList <= Ep30)]
rpx1 = rpx[np.where(EpList <= Ep30)]
rpy1 = rpy[np.where(EpList <= Ep30)]
rpz1 = rpz[np.where(EpList <= Ep30)]
rdx1 = rdx[np.where(EpList <= Ep30)]
rdy1 = rdy[np.where(EpList <= Ep30)]
rdz1 = rdz[np.where(EpList <= Ep30)]

Epu = 80.0#MeV
EpList2 = EpList[np.where(EpList > Epu)]
runList2 = runList[np.where(EpList > Epu)]
dr2 = dr[np.where(EpList > Epu)]
dt2 = dt[np.where(EpList > Epu)]
rpx2 = rpx[np.where(EpList > Epu)]
rpy2 = rpy[np.where(EpList > Epu)]
rpz2 = rpz[np.where(EpList > Epu)]
rdx2 = rdx[np.where(EpList > Epu)]
rdy2 = rdy[np.where(EpList > Epu)]
rdz2 = rdz[np.where(EpList > Epu)]


Nbins = 45
xbinmin = 7.5
xbinmax = 30
h0 =mydef.make_hist( EpList1, 'Ep (7.5-%sMeV)' %Ep30 , Nbins, xbinmin, xbinmax, 2, 2, False)

#Nbins = 46
Nbins = 71
xbinmin = 7.5
#xbinmax = 99.5
xbinmax = 149.5
h1 =mydef.make_hist( EpList, 'Ep (7.5-150MeV)', Nbins, xbinmin, xbinmax, 2, 2, False)

Nbins = 35
xbinmin = 80
xbinmax = 150
h2 =mydef.make_hist( EpList, 'Ep (80-150MeV)', Nbins, xbinmin, xbinmax, 2, 2, False)


Nbins = 200
xbinmin = 0
xbinmax = 1000
h_dt  =mydef.make_hist( dt, 'dt (7.5-150MeV)', Nbins, xbinmin, xbinmax, 2, 2, False)
h_dt1 =mydef.make_hist( dt1, 'dt (7.5-30MeV)', Nbins, xbinmin, xbinmax, 2, 2, False)
h_dt2 =mydef.make_hist( dt2, 'dt (80-150MeV)', Nbins, xbinmin, xbinmax, 4, 2, False)

Nbins = 50
xbinmin = 0
xbinmax = 250
h_dr  =mydef.make_hist( dr, 'dr (7.5-150MeV)', Nbins, xbinmin, xbinmax, 2, 2, False)
h_dr1 =mydef.make_hist( dr1, 'dr (7.5-30MeV)', Nbins, xbinmin, xbinmax, 4, 2, False)
h_dr2 =mydef.make_hist( dr2, 'dr (80-150MeV)', Nbins, xbinmin, xbinmax, 4, 2, False)

Nbins = 120
xbinmin = 0
xbinmax = 12000
h_run  =mydef.make_hist( runList, 'runList (7.5-150MeV)', Nbins, xbinmin, xbinmax, 2, 2, False)
h_run1 =mydef.make_hist( runList1, 'runList (7.5-30MeV)', Nbins, xbinmin, xbinmax, 4, 2, False)
h_run2 =mydef.make_hist( runList2, 'runList (80-150MeV)', Nbins, xbinmin, xbinmax, 4, 2, False)


g0=mydef.make_graph( xList=rpx**2 + rpy**2, yList=rpz, name='hoge',
                     title='Vertex prompt (7.5--150MeV)', xtitle='rx^2 + ry^2', ytitle='rz', iColor=2, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=50, ymin=-7, ymax=+7)

g1=mydef.make_graph( xList=rdx**2 + rdy**2, yList=rdz, name='hoge',
                     title='Vertex delayed (7.5--150MeV)', xtitle='rx^2 + ry^2', ytitle='rz', iColor=4, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=50, ymin=-7, ymax=+7)


g2=mydef.make_graph( xList=rpx1**2 + rpy1**2, yList=rpz1, name='hoge',
                     title='Vertex prompt (7.5--30MeV)', xtitle='rx^2 + ry^2', ytitle='rz', iColor=2, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=50, ymin=-7, ymax=+7)

g3=mydef.make_graph( xList=rdx1**2 + rdy1**2, yList=rdz1, name='hoge',
                     title='Vertex delayed (7.5--30MeV)', xtitle='rx^2 + ry^2', ytitle='rz', iColor=4, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=50, ymin=-7, ymax=+7)


g4=mydef.make_graph( xList=rpx2**2 + rpy2**2, yList=rpz2, name='hoge',
                     title='Vertex prompt (80--150MeV)', xtitle='rx^2 + ry^2', ytitle='rz', iColor=2, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=50, ymin=-7, ymax=+7)

g5=mydef.make_graph( xList=rdx2**2 + rdy2**2, yList=rdz2, name='hoge',
                     title='Vertex delayed (80--150MeV)', xtitle='rx^2 + ry^2', ytitle='rz', iColor=4, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=50, ymin=-7, ymax=+7)


zbList =  np.arange(-6,6,0.1)
rbList = 6**2*np.ones( len(zbList)) - zbList**2

gr=mydef.make_graph( xList=rbList, yList=zbList, name='hoge',
                     title='Vertex delayed', xtitle='rx^2 + ry^2', ytitle='rz', iColor=3, iSize=1.2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=None, xmax=None, ymin=None, ymax=None)

c0 = ROOT.TCanvas('plot1', 'plot2', 1200, 1200)
ROOT.gStyle.SetOptStat(0)
c0.Divide(3,3)
c0.Draw()
## 7.5 -- 30MeV
c0.cd(1)
h1.Draw('H0')

c0.cd(4)
g0.Draw('AP')
gr.Draw('L')

c0.cd(7)
g1.Draw('AP')
gr.Draw('L')

## 7.5 -- 100MeV
c0.cd(2)
h0.Draw('H0')

c0.cd(5)
g2.Draw('AP')
gr.Draw('L')

c0.cd(8)
g3.Draw('AP')
gr.Draw('L')

## 80 -- 100MeV
c0.cd(3)
h2.Draw('H0')

c0.cd(6)
g4.Draw('AP')
gr.Draw('L')

c0.cd(9)
g5.Draw('AP')
gr.Draw('L')



## NEXT
c1 = ROOT.TCanvas('plot3', 'plot4', 1200, 1200)

c1.Divide(3,3)

c1.cd(1)
h_dt.Draw('H0')
c1.cd(4)
h_dr.Draw('H0')
c1.cd(7)
h_run.Draw('H0')

c1.cd(2)
h_dt1.Draw('H0')
c1.cd(5)
h_dr1.Draw('H0')
c1.cd(8)
h_run1.Draw('H0')

c1.cd(3)
h_dt2.Draw('H0')
c1.cd(6)
h_dr2.Draw('H0')
c1.cd(9)
h_run2.Draw('H0')
