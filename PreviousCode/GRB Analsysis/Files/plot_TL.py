import numpy as  np
import mydef,ROOT,RootLogon

## Read our result


#[TList,LListS]=np.loadtxt('../coincidence_analysis_short/TLresult2.dat',unpack=True)
#[TList,LListL]=np.loadtxt('../coincidence_analysis_short/TLresult.dat',unpack=True)



[TList,LListL]=np.loadtxt('../coincidence_analysis/TLresult2.dat',unpack=True)
[TList,LListS]=np.loadtxt('../coincidence_analysis_short/TLresult2.dat',unpack=True)

g1=mydef.make_graph( xList=TList, yList=LListL, name='GRB',
                     title='GRB', xtitle='Temperature[MeV]', ytitle='Luminosity [erg]', iColor=2, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=15, ymin=1e+51, ymax=1e+61)

g2=mydef.make_graph( xList=TList, yList=LListS, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=15, ymin=6, ymax=16)

## Theoritical prediction
# Phys.Rev.D 80, 123004 (2009) and  PTP 108, 789 (2002)
x1 = np.array([5., 7.4])
y1 = np.array([1e+52,3.6e+52])

# Phys.Rev.Lett 107, 051102 (2011)
x3 = np.array([6.3,9.5])
y3 = np.array([3e+53,3e+53])

x4 = np.array([6.3,9.5])
y4 = np.array([9e+53,9e+53])

g3=mydef.make_graph( xList=x1, yList=y1, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=4, iSize=3, iStyle=3,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=15, ymin=6, ymax=16)
g4=mydef.make_graph( xList=x3, yList=y3, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=4, iSize=3, iStyle=4,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=15, ymin=6, ymax=16)
g5=mydef.make_graph( xList=x4, yList=y4, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=4, iSize=3, iStyle=4,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=15, ymin=6, ymax=16)


ROOT.gROOT.SetStyle('Plain')
#ROOT.gROOT.SetBatch(False)

c0 = ROOT.TCanvas('plot0', 'plot0', 600, 600)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)
c0.cd(1).SetLogy()

g1.Draw('AL')
g2.Draw('L')
g3.Draw('P')
g4.Draw('L')
g5.Draw('L')
