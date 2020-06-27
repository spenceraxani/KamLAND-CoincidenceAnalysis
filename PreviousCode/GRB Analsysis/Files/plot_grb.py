import numpy as np
import ROOT,mydef,RootLogon
import time

## Cosmo parameters
H0 = 67.15 #km/s/Mpc
Omega_m = 0.317
Omega_d = 0.683
c = 299792.458 #km/s

A = np.loadtxt("GRBlongList.dat",dtype={'names':('name','utc','T90','fluence','Instrument','redshift'),'formats':('S8','S8','f8','f8','S8','f8')})

xList = A['t90']
yList = A['fluence']

g1=mydef.make_graph( xList=xList, yList=yList, name='hoge',
                     title='cumulative sum', xtitle='date', ytitle='Number',
                     iColor=1, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None, xmin=None, xmax=None, ymin=None, ymax=None)




c0 = ROOT.TCanvas('plot1', 'plot1', 1200, 900)
#ROOT.gStyle.SetTimeOffset(-788918400)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)

g1.Draw('AP')

c0.cd(1).SetLogy()

#g3.GetXaxis().SetNdivisions(505)
#g3.GetXaxis().SetTimeFormat("%Y/%m");
