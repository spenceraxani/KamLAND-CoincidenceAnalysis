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
    Nevent2 = []
    dList  = []
    
    # Period1 and 2
    for line in range(len(zList1)):
    #for line in range(1):
    
        d_l = luminosity_distance(zList1[line])*1e+6
        dList.append(d_l)

        EnuList_z = EnuList *  (1.+zList1[line])
        dE_z = dE *  (1.+zList1[line])

        R = conv *d_l
        Flux  = (1+zList1[line])*L/(5.68*T**4) * EnuList_z**2/(np.exp(EnuList_z/T)+1) *dE_z
        NList = Nt * Flux * CList * tEff /(4*3.14*R**2)
        hoge = e1 * np.sum(NList[np.where( (EnuList > 7.5+0.78) & (EnuList< 100+0.78))])
        #print "Expected event %s w/o resokution" %hoge
        Nevent.append(hoge)
        Nevent2.append(hoge)

        """
        DetectList = []
        for line1 in range(len(EpList)):
            sigma = 0.064*np.sqrt(EpList[line1])
            Rfunc = dE/(np.sqrt(2*3.14)*sigma) * np.exp(-(EpList[line1] - EpList)**2/(2*sigma**2))
            
            DetectList.append(np.sum(NList*Rfunc))
            
            pass
        DetectList = e1 * np.array(DetectList)
        DetectList = DetectList[np.where( (EpList > 7.5) & (EpList <= 100) ) ]

        Nevent.append(np.sum(DetectList))
        """

        pass


    # Period3
    for line in range(len(zList3)):
    
        d_l = luminosity_distance(zList3[line])*1e+6
        dList.append(d_l)

        EnuList_z = EnuList *  (1.+zList3[line])
        dE_z = dE *  (1.+zList3[line])

        
        R = conv *d_l
        Flux  = (1+zList3[line])*L/(5.68*T**4) * EnuList_z**2/(np.exp(EnuList_z/T)+1) *dE_z
        NList = Nt * Flux * CList * tEff /(4*3.14*R**2)

        hoge = eff*NList
        hoge = hoge[np.where( (EnuList > 0.9+0.78) & (EnuList< 100+0.78))]

        Nevent2.append(np.sum(hoge))

        """
        DetectList = []
        for line1 in range(len(EpList)):
            sigma = 0.064*np.sqrt(EpList[line1])
            Rfunc = dE/(np.sqrt(2*3.14)*sigma) * np.exp(-(EpList[line1] - EpList)**2/(2*sigma**2))
            
            DetectList.append(np.sum(NList*Rfunc))
            pass
        DetectList = eff*np.array(DetectList)
        DetectList = DetectList[np.where( (EpList > 0.9) & (EpList <= 100) ) ]

        Nevent.append(np.sum(DetectList))
        """
        
        pass
    

    dList  = np.array(dList)
    Nevent = np.array(Nevent)
    Nevent2 = np.array(Nevent2)


    #print "total expected number of event: %s w/o resolution" %np.sum(Nevent2)
    #print "total expected number of event: %s with resolution" %np.sum(Nevent)
    
    return Nevent,Nevent2,dList

if __name__ == "__main__":
     
    dat = np.loadtxt('coincidence_time.dat')
    
    zList = dat[:,3]
    pList = dat[:,6]
    
    zList1  = zList[np.where(pList<3)]
    zList3  = zList[np.where(pList==3)]
    
    Nfc90 = 2.365

    ### GRB parameter
    #T = 15/3.15

    L0 = 8.23e+58
    L  = L0 * 624150.974

    TList = []
    LconList = []
    LconList2 = []

    T = 5
    [Nevent,Nevent2,dList]=cal_number_event(T,L,zList1,zList3)
    N = np.sum(Nevent)
    N2= np.sum(Nevent2)
    
    deff =  1./np.sqrt(np.sum(1/dList**2)) /1e+6
    print "Effective distance :%s Mpc" %deff
