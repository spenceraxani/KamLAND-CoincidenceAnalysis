import numpy as np
import mydef,ROOT,RootLogon

## Read our result

[EList,FListL]=np.loadtxt('../coincidence_analysis/fluence_long.dat',unpack=True)
[EList,FListS]=np.loadtxt('../coincidence_analysis_short/fluence_short.dat',unpack=True)

g1=mydef.make_graph( xList=EList, yList=FListL, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=2, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=13, ymin=1e+6, ymax=1e+12)

g2=mydef.make_graph( xList=EList, yList=FListS, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=4, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=13, ymin=6, ymax=16)

## Read SNO result
[EListSNO,FListSNO]=np.loadtxt('sno_result.dat',unpack=True)
g3=mydef.make_graph( xList=EListSNO, yList=FListSNO, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=1, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=13, ymin=6, ymax=16)
## Read SK result
[EListSK,FListSK]=np.loadtxt('sk_result.dat',unpack=True)
g4=mydef.make_graph( xList=EListSK, yList=FListSK, name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=1, iSize=2, iStyle=3,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=13, ymin=6, ymax=16)




ROOT.gROOT.SetStyle('Plain')
#ROOT.gROOT.SetBatch(False)

c0 = ROOT.TCanvas('plot0', 'plot0', 600, 600)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)
c0.cd(1).SetLogy()
g1.Draw('AL')
g2.Draw('L')
g3.Draw('L')
g4.Draw('LP')
