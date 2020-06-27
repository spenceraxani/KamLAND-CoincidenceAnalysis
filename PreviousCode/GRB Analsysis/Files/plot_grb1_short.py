import numpy as np
import ROOT,mydef,RootLogon

## Cosmo parameters
H0 = 67.15 #km/s/Mpc
Omega_m = 0.317
Omega_d = 0.683
c = 299792.458 #km/s

#A = np.loadtxt("GRBlongList.dat",dtype={'names':('name','utc','T90','fluence','Instrument','redshift'),'formats':('S8','S8','f8','f8','S8','f8')})
A = np.loadtxt("GRBshortList.dat",dtype={'names':('name','utc','T90','Instrument','redshift'),'formats':('S8','S8','f8','S8','f8')})
B = np.loadtxt("/data/work/koji/GRB/coincidence_analysis_short/GRBshoftUsedList.dat")
zList0 = A['redshift']
zList1 = B

Nbins = 200
xbinmin = 0
xbinmax = 10
h0=mydef.make_hist( zList0, 'hoge1', Nbins, xbinmin, xbinmax, 2, 1, False)
h1=mydef.make_hist( zList1, 'hoge2', Nbins, xbinmin, xbinmax, 4, 1, False)

xList = []
yList0 = []
yList1 = []
YList0 = []
YList1 = []

for line in range(Nbins):
    xList.append(h0.GetBinCenter(line))
    yList0.append(h0.GetBinContent(line))
    yList1.append(h1.GetBinContent(line))
    pass

xList = np.array(xList)
yList0 = np.array(yList0)
yList1 = np.array(yList1)


for line in range(Nbins):
    YList0.append(np.sum(yList0[:line]))
    YList1.append(np.sum(yList1[:line]))
    pass

YList0 = np.array(YList0)
YList1 = np.array(YList1)

Nbins = 20
xbinmin = 0
xbinmax = 10
h0=mydef.make_hist( zList0, 'hoge1', Nbins, xbinmin, xbinmax, 2, 1, False)
h1=mydef.make_hist( zList1, 'hoge2', Nbins, xbinmin, xbinmax, 4, 1, False)


g0=mydef.make_graph( xList=xList, yList=YList0, name='hoge', 
                     title='cumulative sum', xtitle='Redshift', ytitle='Number', 
                     iColor=2, iSize=3, iStyle=1, 
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=10, ymin=None, ymax=None)

g1=mydef.make_graph( xList=xList, yList=YList1, name='hoge', 
                     title='cumulative sum', xtitle='Redshift', ytitle='Number', 
                     iColor=4, iSize=3, iStyle=1, 
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=10, ymin=None, ymax=None)


dList0 = []
for line in zList0:
    intzList = np.arange(0,line,line/2000)
    dz = intzList[1] - intzList[0]

    integral = c/H0 * dz / np.sqrt(Omega_m * (1+intzList)**3 + Omega_d)
    dList0.append( np.sum(integral))
    pass

dList0 = np.array(dList0) * (1+zList0)/1000

dList1 = []
for line in zList1:
    intzList = np.arange(0,line,line/2000)
    dz = intzList[1] - intzList[0]

    integral = c/H0 * dz / np.sqrt(Omega_m * (1+intzList)**3 + Omega_d)
    dList1.append( np.sum(integral))
    pass

dList1 = np.array(dList1) * (1+zList1)/1000

print "Effective distance (ALL) %s Mpc with %s GRB" %(1/np.sqrt(np.sum(1./dList0**2)),len(dList0))
print "Effective distance (Used) %s Mpc  %s GRB" %(1/np.sqrt(np.sum(1./dList1**2)),len(dList1))

Nbins = 200
xbinmin = 0
xbinmax = 100
h2=mydef.make_hist( dList0, 'hoge2', Nbins, xbinmin, xbinmax, 2, 1, False)
h3=mydef.make_hist( dList1, 'hoge2', Nbins, xbinmin, xbinmax, 4, 1, False)

aList = []
bList0 = []
bList1 = []
BList0 = []
BList1 = []

for line in range(Nbins):
    aList.append(h2.GetBinCenter(line))
    bList0.append(h2.GetBinContent(line))
    bList1.append(h3.GetBinContent(line))
    pass

aList = np.array(aList)
bList0 = np.array(bList0)
bList1 = np.array(bList1)


for line in range(Nbins):
    BList0.append(np.sum(bList0[:line]))
    BList1.append(np.sum(bList1[:line]))
    pass

BList0 = np.array(BList0)
BList1 = np.array(BList1)

Nbins = 20

h2=mydef.make_hist( dList0, 'hoge2', Nbins, xbinmin, xbinmax, 2, 1, False)
h3=mydef.make_hist( dList1, 'hoge2', Nbins, xbinmin, xbinmax, 4, 1, False)



g2=mydef.make_graph( xList=aList, yList=BList0, name='hoge', 
                     title='cumulative sum', xtitle='Distance [Gpc]', ytitle='Number', 
                     iColor=2, iSize=3, iStyle=1, 
                     errPlot=False,errxList=None, erryList=None, xmin=0, xmax=100, ymin=None, ymax=None)

g3=mydef.make_graph( xList=aList, yList=BList1, name='hoge', 
                     title='cumulative sum', xtitle='Distance [Gpc]', ytitle='Number', 
                     iColor=4, iSize=3, iStyle=1, 
                     errPlot=False,errxList=None, erryList=None, xmin=None, xmax=None, ymin=None, ymax=None)




c0 = ROOT.TCanvas('plot1', 'plot1', 900, 900)


h0.SetLineWidth(3)
h1.SetLineWidth(3)
h2.SetLineWidth(3)
h3.SetLineWidth(3)

c0.Divide(2,2)
c0.Draw()
c0.cd(1)
h0.Draw('H0')
h1.Draw('SAME')
c0.cd(3)
g0.Draw('AL')
g1.Draw('L')
c0.cd(2)
h2.Draw('H0')
h3.Draw('SAME')
c0.cd(4)
g2.Draw('AL')
g3.Draw('L')


