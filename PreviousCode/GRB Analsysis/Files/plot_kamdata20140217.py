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

EpList0 = np.r_[EpList1,EpList2]

Nbins = 20
xbinmin = 0
xbinmax = 100
h1  =mydef.make_hist( EpList0, 'Period I', Nbins, xbinmin, xbinmax, 2, 2, False)
h2  =mydef.make_hist( EpList0, 'Period II', Nbins, xbinmin, xbinmax, 2, 2, False)
h2_  =mydef.make_hist( EpList0, 'Period_ II', Nbins, xbinmin, xbinmax, 2, 2, False)

#
NList = np.array([81.9141,6.20866,1.85299,5.70035])
h3  =mydef.make_hist( NList, 'cList', Nbins, xbinmin, xbinmax, 8, 2, False)


t1 = 6.91
t3 = 1.2
t = (t1 + t3) * 365 * 24.
tw1 = 18.3
tw3 = 6.82
tw = 25.2

e1 = 0.926179
Ngrb1 = 175
e3 = 0.773454
Ngrb3 = 38


for line in range(Nbins):
    h2.SetBinContent(line+1,h1.GetBinContent(line+1)*tw/t)
    h2_.SetBinContent(line+1,h1.GetBinContent(line+1)*tw/t/(Ngrb1 + Ngrb3))
    pass


#
dat2  = np.loadtxt("../kam_data/cross_section_table2.data")
EnuList = dat2[:,0]
dE = EnuList[1]-EnuList[0]
CList = dat2[:,1]
EpList= EnuList -0.78
Nt= 5.98e+31
tEff=(1-0.097)

conv = 3.08e+18 # pc -> cm
D0 = 109e+6
D  = D0*conv

conv_erg2MeV = 624150.974
L5 = 8.23e+58 * conv_erg2MeV
T5  = 5.
F5 = 1/(4*3.14*D**2)*  L5/(5.68*T5**4) * EnuList**2/(np.exp(EnuList/T5)+1) *dE

L10 = 4.6e+58 * conv_erg2MeV
T10  = 10.
F10 = 1/(4*3.14*D**2)*  L10/(5.68*T10**4) * EnuList**2/(np.exp(EnuList/T10)+1) *dE

L15 = 4.05e+58 * conv_erg2MeV
T15  = 15.
F15 = 1/(4*3.14*D**2)*  L15/(5.68*T15**4) * EnuList**2/(np.exp(EnuList/T15)+1) *dE

EvList5  = tEff*Nt*CList*F5
EvList10 = tEff*Nt*CList*F10
EvList15 = tEff*Nt*CList*F15

EvList5 = (Ngrb1*e1 +Ngrb3 *e3)/(Ngrb1 + Ngrb3)**2 * EvList5
EvList10 = (Ngrb1*e1 +Ngrb3 *e3)/(Ngrb1 + Ngrb3)**2 * EvList10
EvList15 = (Ngrb1*e1 +Ngrb3 *e3)/(Ngrb1 + Ngrb3)**2 * EvList15

EvList5  = tEff*Nt*CList*F5
EvList10 = tEff*Nt*CList*F10
EvList15 = tEff*Nt*CList*F15

EvList5_ = (Ngrb1*e1 +Ngrb3 *e3)/(Ngrb1 + Ngrb3)**2 * EvList5
EvList10_ = (Ngrb1*e1 +Ngrb3 *e3)/(Ngrb1 + Ngrb3)**2 * EvList10
EvList15_ = (Ngrb1*e1 +Ngrb3 *e3)/(Ngrb1 + Ngrb3)**2 * EvList15


print "Event : %s" %np.sum(EvList5)

f1=mydef.make_graph( xList=EnuList, yList=F5*5/dE, name='GRB',
                     title='Period I', xtitle='Enu [MeV]', ytitle='Flux/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)
f2=mydef.make_graph( xList=EnuList, yList=F10*5/dE, name='GRB',
                     title='Period I', xtitle='Enu [MeV]', ytitle='/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)
f3=mydef.make_graph( xList=EnuList, yList=F15*5/dE, name='GRB',
                     title='Period I', xtitle='Enu [MeV]', ytitle='/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)

g1=mydef.make_graph( xList=EpList, yList=EvList5*5/dE, name='GRB',
                     title='Period I', xtitle='Ep [MeV]', ytitle='Events/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)
g2=mydef.make_graph( xList=EpList, yList=EvList10*5/dE, name='GRB',
                     title='Period I', xtitle='Ep [MeV]', ytitle='Events/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)
g3=mydef.make_graph( xList=EpList, yList=EvList15*5/dE, name='GRB',
                     title='Period I', xtitle='Ep [MeV]', ytitle='Events/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)

g1_=mydef.make_graph( xList=EpList, yList=EvList5_*5/dE, name='GRB',
                     title='Period I', xtitle='Ep [MeV]', ytitle='Events/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)
g2_=mydef.make_graph( xList=EpList, yList=EvList10_*5/dE, name='GRB',
                     title='Period I', xtitle='Ep [MeV]', ytitle='Events/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)
g3_=mydef.make_graph( xList=EpList, yList=EvList15_*5/dE, name='GRB',
                     title='Period I', xtitle='Ep [MeV]', ytitle='Events/5MeV', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=100, ymin=None, ymax=None)


f1.SetMinimum(1e+5)
f1.SetMaximum(1e+10)

g1.SetMinimum(1e-4)
g1.SetMaximum(1)

g1_.SetMinimum(1e-6)
g1_.SetMaximum(1e-2)

#


c0 = ROOT.TCanvas('plot1', 'plot1', 1200, 1200)
#ROOT.gStyle.SetOptStat(0)
c0.Divide(2,2)
c0.Draw()
c0.cd(1)
c0.cd(1).SetLogy()
h1.Draw('H0')
h3.Draw('SAME')

c0.cd(2).SetLogy()
f1.Draw('AL')
f2.Draw('L')
f3.Draw('L')

c0.cd(3).SetLogy()
g1.Draw('AL')
g2.Draw('L')
g3.Draw('L')
h2.Draw('SAME')

c0.cd(4).SetLogy()
g1_.Draw('AL')
g2_.Draw('L')
g3_.Draw('L')
h2_.Draw('SAME')




