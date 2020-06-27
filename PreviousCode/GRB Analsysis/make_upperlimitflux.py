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

dat2  = np.loadtxt("../kam_data/cross_section_table2.data")
EnuList = dat2[:,0]
dE = EnuList[1]-EnuList[0]
CList = dat2[:,1]
EpList= EnuList -0.78



dat3  = np.loadtxt("SelectionEfficiencyDS3withCompleteness.dat")
eff   = dat3[:,1]




def luminosity_distance(z):
    zList = np.arange(0.0001,10,0.0001)
    dz = zList[1] - zList[0]
    
    integral = c/H0 * dz / np.sqrt(Omega_m * (1+zList)**3 + Omega_d)
    dl = np.sum(integral[np.where(zList<z)])* (1+z)
    return dl

def cal_number_event(T,L,zList1,zList3):


    Nevent = []
    dList  = []
    
    # Period1 and 2
    first_line = 1
    for line in range(len(zList1)):
        
        d_l = luminosity_distance(zList1[line])*1e+6


        EnuList_z = EnuList *  (1.+zList1[line])
        dE_z = dE *  (1.+zList1[line])

        R = conv *d_l
        Flux  = (1+zList1[line])*L/(5.68*T**4) * EnuList_z**2/(np.exp(EnuList_z/T)+1) *dE_z
        NList = e1 * Nt * Flux * CList * tEff /(4*3.14*R**2)
        
        NList = NList[np.where( (EnuList > 0.9+0.78) & (EnuList< 100+0.78))]
        #NList = NList[np.where( (EnuList > 7.5+0.78) & (EnuList< 100+0.78))]

        if first_line == 1:
            reNList0 = NList
            first_line = 0            
        else:
            reNList0 = reNList0 + NList
            pass
        pass
    
    reNList0 = np.array(reNList0)
    EnuList0 = EnuList[np.where( (EnuList > 7.5+0.78) & (EnuList< 100+0.78))]
    
    # Period3
    first_line = 1
    for line in range(len(zList3)):
    
        d_l = luminosity_distance(zList3[line])*1e+6
        dList.append(d_l)

        EnuList_z = EnuList *  (1.+zList3[line])
        dE_z = dE *  (1.+zList3[line])

        
        R = conv *d_l
        Flux  = (1+zList3[line])*L/(5.68*T**4) * EnuList_z**2/(np.exp(EnuList_z/T)+1) *dE_z
        NList = eff * Nt * Flux * CList * tEff /(4*3.14*R**2)
        NList = NList[np.where( (EnuList > 0.9+0.78) & (EnuList< 100+0.78))]

        if first_line == 1:
            reNList1 = NList
            first_line = 0            
        else:
            reNList1 = reNList1 + NList
            pass
        pass

    reNList1 = np.array(reNList1)
    EnuList1 = EnuList[np.where( (EnuList > 0.9+0.78) & (EnuList< 100+0.78))] 
    

    
    #print "total expected number of event: %s w/o resolution" %np.sum(Nevent2)
    #print "total expected number of event: %s with resolution" %np.sum(Nevent)
    
    return reNList0,EnuList0,reNList1,EnuList1

if __name__ == "__main__":
     
    dat = np.loadtxt('coincidence_time.dat')
    
    zList = dat[:,3]
    pList = dat[:,6]
    
    zList1  = zList[np.where(pList<3)]
    zList3  = zList[np.where(pList==3)]

    conv_erg2MeV = 624150.974
    L5 = 8.23e+58 * conv_erg2MeV
    T5 = 5
    L10 =4.6e+58 * conv_erg2MeV
    T10 = 10
    L15 = 4.05e+58 * conv_erg2MeV
    T15 = 15

                    
    [reNList0,EList0,reNList1,EList1]=cal_number_event(T5,L5,zList1,zList3)
    event = np.sum(reNList0) + np.sum(reNList1)
    print event
    NList5 = reNList0 + reNList1
    #NList1 = reNList1[660:] + reNList0
    #NList2 = reNList1[:660]
    #NList5 = np.r_[NList2,NList1]


    [reNList0,EList0,reNList1,EList1]=cal_number_event(T10,L10,zList1,zList3)
    event = np.sum(reNList0) + np.sum(reNList1)
    #NList1 = reNList1[660:] + reNList0
    #NList2 = reNList1[:660]
    #NList10 = np.r_[NList2,NList1]
    print event
    NList10 = reNList0 + reNList1


    [reNList0,EList0,reNList1,EList1]=cal_number_event(T15,L15,zList1,zList3)
    event = np.sum(reNList0) + np.sum(reNList1)
    #NList1 = reNList1[660:] + reNList0
    #NList2 = reNList1[:660]
    #NList15 = np.r_[NList2,NList1]
    print event
    NList15 = reNList0 + reNList1

    
    ## Save data
    f = open('upperflux.dat', 'w')

    for line in range(len(EList1)):
        f.writelines( '%s %s %s %s' %(EList1[line],NList5[line],NList10[line],NList15[line]) + str('\n'))
        pass
    f.close



                                            


    
    g0=mydef.make_graph( xList=EList1, yList=NList5, name='GRB',
                         title='GRB', xtitle='T[MeV]', ytitle='L [erg]', iColor=2, iSize=2, iStyle=1,
                         errPlot=False,errxList=None, erryList=None,
                         xmin=None, xmax=None, ymin=None, ymax=None)

    g1=mydef.make_graph( xList=EList1, yList=NList10, name='GRB',
                         title='GRB', xtitle='T[MeV]', ytitle='L [erg]', iColor=3, iSize=2, iStyle=2,
                         errPlot=False,errxList=None, erryList=None,
                         xmin=None, xmax=None, ymin=None, ymax=None)
    g2=mydef.make_graph( xList=EList1, yList=NList15, name='GRB',
                         title='GRB', xtitle='T[MeV]', ytitle='L [erg]', iColor=4, iSize=2, iStyle=2,
                         errPlot=False,errxList=None, erryList=None,
                         xmin=None, xmax=None, ymin=None, ymax=None)

        

    ROOT.gROOT.SetStyle('Plain')
    #ROOT.gROOT.SetBatch(False)

    c0 = ROOT.TCanvas('plot0', 'plot0', 1200, 600)
    c0.Divide(1,1)
    c0.Draw()
    c0.cd(1)
    g0.Draw('AL')
    g1.Draw('L')
    g2.Draw('L')

