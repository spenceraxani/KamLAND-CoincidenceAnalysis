import ROOT,mydef,RootLogon
import time
import numpy as np

H0 = 67.15 #km/s/Mpc
Omega_m = 0.317
Omega_d = 0.683
c = 299792.458 #km/s

conv = 3.08e+18 # pc -> cm
Nt= 5.98e+31
tEff=(1-0.097)

e1 = 0.926179
e3 = 0.773454

Ngrb1 =(77+98)*1.
Ngrb3 =(38)*1.

Nfc90 = 2.365

### OLD (20140808)
"""
EnuList = dat2[1:,0]
dE = EnuList[1]-EnuList[0]
CList = dat2[1:,1]

CList = CList[np.where(EnuList > 8.3)]
EnuList = EnuList[np.where(EnuList > 8.3)]

EpList= EnuList -0.78
EvList= EnuList -0.9



FList = Nfc90/ ((Ngrb1 * e1 *tEff * CList * Nt) +  (Ngrb3 *e3 *tEff * CList * Nt) )
#FList = Nfc90/ (Ngrb1 * e1 *tEff * CList * Nt) 

g0=mydef.make_graph( xList=EnuList, yList=np.log10(FList), name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=2, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=13, ymin=6, ymax=16)
"""

### New
dat2  = np.loadtxt("../kam_data/cross_section_table2.data")
dat3  = np.loadtxt("SelectionEfficiencyDS3withCompleteness.dat")
EnuList = dat2[:,0]
EpList = EnuList - 0.78
dE = EnuList[1]-EnuList[0]
CList = dat2[:,1]
eff   = dat3[:,1]
FList2 = []
EList2 = []

for line in range(len(EnuList)):
    if EnuList[line]>1.8:
        sigma = 0.064*np.sqrt(EpList[line])
        R = dE/(np.sqrt(2*3.14)*sigma) * np.exp(-(EpList - EpList[line])**2/(2*sigma**2))

        Ffunc1 = CList * R * e1
        R1 = Ffunc1[np.where(EpList > 7.5)]
        Ffunc3 = CList * R * eff
        R3 = Ffunc3[np.where(EpList > 0.9)]

        """
        CList1 = CList[np.where(EpList > 7.5)]
        R1 = R[np.where(EpList > 7.5)]
        
        CList3 = CList[np.where(EpList > 0.9)]
        R3 = R[np.where(EpList > 0.9)]
        e3 = eff[np.where(EpList > 0.9)]
        """
        
        FList2.append(Nfc90/ ( ((Ngrb1 * tEff * np.sum(R1) * Nt)) + ((Ngrb3 * tEff *np.sum(R3) * Nt)) ))
        EList2.append(EnuList[line])
        pass
    
FList2 = np.array(FList2)
EList2 = np.array(EList2)

f = open('fluence_long.dat', 'w')

for line in range(len(EList2)):
    f.writelines( '%s %s' %(EList2[line],FList2[line]) + str('\n'))
    pass
f.close

g1=mydef.make_graph( xList=EList2, yList=np.log10(FList2), name='GRB',
                     title='GRB', xtitle='Enu[MeV]', ytitle='Fluence [cm-2]', iColor=2, iSize=2, iStyle=1,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=0, xmax=13, ymin=6, ymax=16)






ROOT.gROOT.SetStyle('Plain')
#ROOT.gROOT.SetBatch(False)

c0 = ROOT.TCanvas('plot0', 'plot0', 600, 600)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)

g1.Draw('AL')



