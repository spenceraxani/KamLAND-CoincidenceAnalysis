import numpy as np
import mydef,ROOT,RootLogon

#
dat = np.loadtxt("/data/work/koji/GRB/kam_data/Observed-dcvf-Kat.dat")

runList= dat[:,0]
EpList = dat[:,1]

EpList1 =EpList[np.where( ((runList>=1313) & (runList<6820)) |((runList>=6980) & (runList<10675)) )]
runList1=runList[np.where( ((runList>=1313) & (runList<6820)) |((runList>=6980) & (runList<10675)) )]

EpList1 =EpList1[np.where( runList1!=5380 )]
runList1=runList1[np.where( runList1!=5380 )]

#
dat =np.loadtxt("/data/work/koji/GRB/kam_data/Observed-dcvf-Kat_0.9MeVwithLikelihood.dat")

runList= dat[:,0]
EpList = dat[:,1]

EpList2 =EpList[np.where( runList>11000 )]
runList2=runList[np.where( runList>11000 )]



Nbins = 20
xbinmin = 0
xbinmax = 100
h1  =mydef.make_hist( EpList1, 'Period I', Nbins, xbinmin, xbinmax, 2, 2, False)
h2  =mydef.make_hist( EpList2, 'Period II', Nbins, xbinmin, xbinmax, 2, 2, False)
h0  =mydef.make_hist( EpList1, 'Expected background', Nbins, xbinmin, xbinmax, 2, 2, False)

t1 = 6.91 * 365 * 24.
t3 = 1.2 * 365 * 24.
t = (t1 + t3)
tw1 = 18.3
tw3 = 6.82
tw = 25.2

e1 = 0.926179
Ngrb1 = 175
e3 = 0.773454
Ngrb3 = 38
hist = []
for line in range(Nbins):
    histdata = h1.GetBinContent(line+1)*tw1/t1 + h2.GetBinContent(line+1)*tw3/t3
    hist.append(histdata)
    h0.SetBinContent(line+1,histdata)
    pass

hist = np.array(hist)
print hist

# Result of make_upperlimitflux.py
dat2  = np.loadtxt("../coincidence_analysis/upperflux.dat")
EList = dat2[:,0]
dE = EList[2]-EList[1]
NList1 = dat2[:,1] /dE
NList2 = dat2[:,2] /dE
NList3 = dat2[:,3] /dE

f1=mydef.make_graph( xList=EList, yList=NList1*5, name='GRB',
                     title='Period I', xtitle='Enu [MeV]', ytitle='Flux/5MeV', iColor=4, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)

f2=mydef.make_graph( xList=EList, yList=NList2*5, name='GRB',
                     title='Period I', xtitle='Enu [MeV]', ytitle='Flux/5MeV', iColor=4, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)

f3=mydef.make_graph( xList=EList, yList=NList3*5, name='GRB',
                     title='Period I', xtitle='Enu [MeV]', ytitle='Flux/5MeV', iColor=4, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)



c0 = ROOT.TCanvas('plot1', 'plot1', 600, 600)
#ROOT.gStyle.SetOptStat(0)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)
c0.cd(1).SetLogy()
h0.Draw('H0')
f1.Draw('L')
f2.Draw('L')
f3.Draw('L')


h0.SetMaximum(1)
h0.SetMinimum(1e-4)

c0.Update()

