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

dat2  = np.loadtxt("../kam_data/cross_section_table.data")
EnuList = dat2[:,0]

dE = EnuList[1]-EnuList[0]
CList = dat2[:,1]

EpList= EnuList -0.78
EvList= EnuList -0.9


def luminosity_distance(z):
    zList = np.arange(0.0001,10,0.0001)
    dz = zList[1] - zList[0]
    
    integral = c/H0 * dz / np.sqrt(Omega_m * (1+zList)**3 + Omega_d)
    dl = np.sum(integral[np.where(zList<z)])* (1+z)
    return dl





if __name__ == "__main__":
     
    dat = np.loadtxt('coincidence_time.dat')
    
    zList = dat[:,3]
    pList = dat[:,6]
    
    zList1  = zList[np.where(pList<3)]
    zList3  = zList[np.where(pList==3)]
    
    Nfc90 = 2.42

    ### GRB parameter
    T = 0.5e

    L0 = 1e+58
    L  = L0 * 624150.974
    
    Nevent= []
    dList = []
    # Period1 and 2
    for line in range(len(zList1)):
    
        d_l = luminosity_distance(zList1[line])*1e+6
        dList.append(d_l)
        
        EnuList_z = EnuList *  (1.+zList1[line])
        dE_z = dE *  (1.+zList1[line])

        R = conv *d_l
        Flux  = (1+zList1[line])*L/(5.68*T**4) * EnuList_z**2/(np.exp(EnuList_z/T)+1) *dE_z
        NList = Nt * Flux * CList * tEff /(4*3.14*R**2)
        #print np.sum(NList)
        Nevent.append(e1 * np.sum(NList[np.where( (EnuList > 7.5+0.78) & (EnuList< 100+0.78))]))
        """
        DetectList = []
        for line1 in range(len(EvList)):
            sigma = 0.064*np.sqrt(EvList[line1])
            Rfunc = dE/(np.sqrt(2*3.14)*sigma) * np.exp(-(EvList[line1] - EpList)**2/(2*sigma**2))
            DetectList.append(np.sum(NList*Rfunc))
            pass
        DetectList = e1*np.array(DetectList)
        """
        #print np.sum(DetectList)
        #Nevent.append(np.sum(DetectList[np.where( (EvList > 7.5) & (EvList< 100))]))
        pass

    # Period3
