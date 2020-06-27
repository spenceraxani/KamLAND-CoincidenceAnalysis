import numpy as np
import mydef,ROOT,RootLogon

## Read GRB data
toffset0 = 788918400
dt = 60*60*24*90
N  = 44
xmin = 1028300400 - toffset0
xmax = xmin + dt*N

lGRB_data= np.loadtxt('../coincidence_analysis/coincidence_time.dat')
lGRB_t   = lGRB_data[:,0] - toffset0
lGRB_z   = lGRB_data[:,3]
[x,yl]=mydef.make_histdata(lGRB_t,xmin,xmax,N)

sGRB_data= np.loadtxt('../coincidence_analysis_short/coincidence_time.dat')
sGRB_t   = sGRB_data[:,0] - toffset0
sGRB_z   = sGRB_data[:,3]
[x,ys]=mydef.make_histdata(sGRB_t,xmin,xmax,N)


g1=mydef.make_graph( xList=x, yList=yl, name='hoge',
                     title='GRB list', xtitle='Date', ytitle='Num of GRB/month', iColor=2, iSize=1, iStyle=3,
                     errPlot=True,errxList=dt/2.*np.ones(len(x)), erryList=np.zeros(len(x)), xmin=xmin-60*60*24*63, xmax=xmax, ymin=None, ymax=None)

g2=mydef.make_graph( xList=x, yList=ys, name='hoge',
                     title='GRB list', xtitle='Date', ytitle='Redshift', iColor=4, iSize=1, iStyle=4,
                     errPlot=True,errxList=dt/2.*np.ones(len(x)), erryList=np.zeros(len(x)), xmin=xmin, xmax=xmax, ymin=None, ymax=None)

## Read DC events
edat = np.loadtxt('../kam_data/Observed-dcvf-Kat.dat')
runList  = edat[:,0] 
timeList = edat[:,2] - toffset0

timeList1 = timeList[np.where( runList< 10675 )] 

edat = np.loadtxt('../kam_data/Observed-dcvf-Kat_0.9MeVwithLikelihood.dat')
runList  = edat[:,0]
timeList = edat[:,2] - toffset0

timeList2 = timeList[np.where( runList > 11000 )] 

ctimeList = np.r_[timeList1,timeList2]

[x,ydc]=mydef.make_histdata(ctimeList,xmin,xmax,N)




g3=mydef.make_graph( xList=x, yList=ydc, name='hoge',
                     title='DC list', xtitle='Date', ytitle='Redshift', iColor=6, iSize=1, iStyle=5,
                     errPlot=True,errxList=dt/2.*np.ones(len(x)), erryList=np.zeros(len(x)), xmin=xmin, xmax=xmax, ymin=None, ymax=None)


g1.SetLineStyle(1)
g2.SetLineStyle(2)
g3.SetLineStyle(3)

c1 = ROOT.TCanvas('plot1', 'plot1', 600, 600)
c1.Divide(1,2)
c1.Draw()
c1.cd(1)

g1.Draw('PA')

"""
g1.SetLabelSize(0, "X");
g1.GetXaxis().SetTickLength(0.02);
g1.GetYaxis().SetTickLength(0.1);
g1.GetXaxis().CenterTitle();
g1.GetXaxis().SetTitleSize(0);
g1.GetXaxis().SetLabelSize(0);
g1.GetYaxis().CenterTitle();
g1.GetYaxis().SetTitleSize(0.045);
g1.GetYaxis().SetTitleOffset(1.4);
g1.GetYaxis().SetLabelSize(0.036);
g1.GetYaxis().SetLabelOffset(0.01);
"""

                              
g1.GetXaxis().SetTimeDisplay(1)
g1.GetXaxis().SetNdivisions(510)
g1.GetXaxis().SetTimeFormat("%y\/%m")
g2.Draw('P')

c1.cd(2)
g3.Draw('AP')
g3.GetXaxis().SetTimeDisplay(1)
g3.GetXaxis().SetNdivisions(510)
g3.GetXaxis().SetTimeFormat("%y\/%m")

