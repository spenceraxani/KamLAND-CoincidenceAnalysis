

import numpy as np
import os
from scipy.integrate import quad
import scipy
import matplotlib.pyplot as plt
class DataPaths:
    def __init__(self, location):
        self.location = location
        self.xsLocation = location+'/Resources/XS/'
        self.pdfLocation = os.getcwd()+'/Figures/'
        return
    
    def RealoadPaths(self,location):
        self.location = location
        self.xsLocation = location+'/Resources/XS/'
        return
    

class Constants:
    def __init__(self):
        self.H0 = 67.66                #Hubble constant (km/s/Mpc). From Planck (2018) 
        self.OmegaLambda = 0.6889      # Cosmilogical constant
        self.OmegaMatter = 0.315       # Omega matter density 
        self.c = 299792458             # Speed of light
        self.NTargetProtons = 5.98e31  # Number of target protons. Update from Joe.
        self.neutrinoMass = 1.0        # Average neutrino mass. 
        self.NeutrinoEmissionRange = [-500,500] # Range of allowed neutrino emission time
        self.maxNeutrinoEnergy_MeV = 100
        self.minNeutrinoEnergy_MeV = 1.8
        
        self.LiveTime = float(3600*24*365*0.9) # This is the livetime of the data.
        self.RealTime = float(3600*24*365)     # The realtime of the data. 
        self.LiveTimeEfficiency = self.LiveTime/self.RealTime 
        self.NeutrinoCenterEnergies = np.linspace(1.8,100,983)
        self.CrossSection        = np.asarray(self.NeutrinoCenterEnergies**2/11.)*1E-42 # FIX!!
        self.DetectionEfficiency = np.asarray([(0.5*i)/(100)+0.5 for i in self.NeutrinoCenterEnergies])
        
        
        em2_spectrum = np.asarray([i**-2 for i in self.NeutrinoCenterEnergies])
        em2_spectrum_pdf = em2_spectrum/sum(em2_spectrum)
        flat_spectrum = np.asarray([1. for i in self.NeutrinoCenterEnergies])
        flat_spectrum_pdf = flat_spectrum/sum(flat_spectrum)
        self.NeutrinoSpectrumPDF = flat_spectrum_pdf
        
        return
    
    def setSeed(self,seed):
        np.random.seed(seed)
        return
        
    def setCrossSection(self, cross_section):
        if len(cross_section)!= len(self.NeutrinoCenterEnergies):
            print('Error')
        else:
            self.CrossSection = cross_section
        return 

    def setDetectionEfficiency(self, detection_efficiency):
        if len(detection_efficiency)!= len(self.NeutrinoCenterEnergies):
            print('Error')
        else:
            self.DetectionEfficiency = detection_efficiency
        return 

    def setNeutrinoSpectrumPDF(self, neutrino_energy_spectrum):
        if len(neutrino_energy_spectrum)!= len(self.NeutrinoCenterEnergies):
            print('Error')
        else:
            nu_e_spectrum = nu_e_spectrum/sum(nu_e_spectrum)
            self.NeutrinoSpectrumPDF = nu_e_spectrum
        return 

class FitParameters:
    def __init__(self):
        self.GWDistance     = 0
        self.NeutrinoEnergy = 0
        return
    
class GravitationalWaveClass:
    def __init__(self,GWArray):
        self.Name           = GWArray[:,0]
        self.Time           = np.asarray(GWArray[:,1]).astype(float)
        self.Distance       = np.asarray(GWArray[:,2]).astype(float)
        self.Distance_m     = np.asarray(GWArray[:,3]).astype(float)
        self.Distance_p     = np.asarray(GWArray[:,4]).astype(float)
        return
    
class NeutrinoClass:
    def __init__(self,NeutrinoArray):
        mask = np.asarray(NeutrinoArray[:,1]>1.8)
        for i in range(len(mask)):
            if not mask[i]:
                print('Neutrino with energy less than 0.9MeV found. Removed. Energy: ')
                print(NeutrinoArray[:,1][i],'MeV')
                
        self.Time         = np.asarray(NeutrinoArray[:,0][mask]).astype(float)
        self.Energy       = np.asarray(NeutrinoArray[:,1][mask]).astype(float)
        self.Energy_m     = np.asarray(NeutrinoArray[:,2][mask]).astype(float)
        self.Energy_p     = np.asarray(NeutrinoArray[:,3][mask]).astype(float)
        return
    
class KLZFit:
    def __init__(self, DataPaths, Constants,FitParameters):
        self.DataPaths = DataPaths
        self.Constants = Constants
        self.FitParameters = FitParameters
        self.explainNeutrinos = 'Each row is a neutirno event. Col: [time (UTC),e-,e,e+]'
        self.Neutrinos = 'None'
        self.explainGravitationalWaves = 'Each row is a neutirno event. Col: [name, time (UTC),e-,e,e+]'
        self.GravitationalWaves = 'None'
        
    def setNeutrinos(self,uploadNeutrinos):
        NeutrinoEvents            = NeutrinoClass(uploadNeutrinos)
        self.Neutrinos            = NeutrinoEvents
        return
    
    def getNeutrinos(self):
        return self.Neutrinos
    
    def setGravitationalWaves(self,GravitationalWaves):
        GWEvents                               = GravitationalWaveClass(GravitationalWaves)
        self.GravitationalWaves                = GWEvents
        self.GravitationalWavesExpectedRate    = len(self.GravitationalWaves.Time)/self.Constants.LiveTime
        
    def getGravitationalWaves(self):
        return self.GravitationalWaves
    
    def calculateTimeRange(self, FitParameters):
        def neutrinoTimeOfFlight(distance_Mpc,Energy_MeV):
            mn = self.Constants.neutrinoMass
            H0 = self.Constants.H0 * 1000/(3.08568e22)
            Ol = self.Constants.OmegaLambda
            Om =  self.Constants.OmegaMatter
            c = self.Constants.c
            v = H0 *distance_Mpc * 3.08568e22; 
            z = v/c
            E = Energy_MeV*1e6
            const = mn**2/(2*H0*E**2)
            def func(x):
                return const * 1./((1+x)**2 * np.sqrt(Ol+Om*(1+x)**3))
            return quad(func, 0., z)[0]

        GWNeutrinoTOF = []
        for i in range(len(self.GravitationalWaves.Time)):
            GWTime    = self.GravitationalWaves.Time[i]
            if FitParameters.GWDistance < 0:
                GWDist    = self.GravitationalWaves.Distance[i] + FitParameters.GWDistance * self.GravitationalWaves.Distance_m[i]
            else:
                GWDist    = self.GravitationalWaves.Distance[i] + FitParameters.GWDistance * self.GravitationalWaves.Distance_p[i]
                
            neutrino_energies = []
            for j in range(len(self.Neutrinos.Time)):
                #print(self.Neutrinos.Time[j])
                NTime = self.Neutrinos.Time[j]
                if FitParameters.NeutrinoEnergy < 0:
                    neutrino_energies.append(self.Neutrinos.Energy[j] + self.FitParameters.NeutrinoEnergy * self.Neutrinos.Energy_m[j])
                else:
                    neutrino_energies.append(self.Neutrinos.Energy[j] + self.FitParameters.NeutrinoEnergy * self.Neutrinos.Energy_p[j])

            TOF_neutrinos   = np.asarray([neutrinoTimeOfFlight(GWDist, j) for j in neutrino_energies])
            GWNeutrinoTOF.append(TOF_neutrinos)
        self.GravitationalWaves_TimeRange    = np.asarray(GWNeutrinoTOF)
        return 
    
    
    def setNeutrinoEnergyPDF(self, NeutrinoEnergyPDF):
        if len(NeutrinoEnergyPDF) != len(self.Constants.NeutrinoCenterEnergies):
            print('You need to generate the pdf at the neutrino energies self.Constants.NeutrinoCenterEnergies.')
        NeutrinoEnergyPDF             = NeutrinoEnergyPDF/sum(NeutrinoEnergyPDF) # Normalize to make it a pdf.
        self.NeutrinoEnergyPDF        = np.asarray(NeutrinoEnergyPDF)
        self.NeutrinoEnergyAverage    = sum(self.NeutrinoEnergyPDF * self.Constants.NeutrinoCenterEnergies)
        print('Average Neutrino Energy in sample: '+str(np.round(self.NeutrinoEnergyAverage,2))+'MeV')
        return 
    

    
    def getNeutrinoRealization(self,Rate):
        # Rate = neutrino rate that you want.
        try:
            NeutrinoEnergyPDF = self.NeutrinoEnergyPDF
        except:
            print('You need to define the self.NeutrinoEnergyPDF before getting a realization.')
        
        # Generate neutrinos from the NeutrinoEnergyPDF
        cdf             = np.cumsum(NeutrinoEnergyPDF)
        cdf             = cdf / cdf[-1]
        random_values   = np.random.rand(int(self.Constants.LiveTime*Rate)) 
        value_bins      = np.searchsorted(cdf, random_values) 
        random_neutrino_energies = self.Constants.NeutrinoCenterEnergies[value_bins]
        NeutrinoEvents = []
        for i in range(len(random_neutrino_energies)):
            # Get a random neutrino energy, and smear it by some amount. 
            ee = np.random.normal(random_neutrino_energies[i],random_neutrino_energies[i]*0.3)
            if ee > 1.8:
                # Assign a random time, and give uncertainty on the energy.
                # Spencer: update the energy uncertainty later.
                NeutrinoEvents.append([np.random.uniform(low=0, high=self.Constants.LiveTime, size=(1,))[0],ee,-0.1*ee,0.1*ee])
        
        self.NeutrinoRateExpectation = len(random_neutrino_energies)/self.Constants.LiveTime
        self.NeutrinoRateExpectationUncertainty = np.sqrt(len(random_neutrino_energies))/self.Constants.LiveTime
        return np.asarray(NeutrinoEvents) 
    

    def plotEvents(self):
        import matplotlib.pyplot as plt
        for i in range(len(self.GravitationalWaves)):
            t0 = self.GravitationalWaves[i][0]
            
            for j in range(len(self.Neutrinos)):
                n  = self.Neutrinos[j]
                x  = [n[0]-t0]
                y  = [n[1]]
                ym = [-n[2]]
                yp = [n[3]]
        
                p = plt.errorbar(x, y,yerr=[ym,yp], xerr = [0], fmt='o',ms=3, mew=1)
                c = p[0].get_color()
                
                n_emission  = self.Constants.NeutrinoEmissionRange
                
                n_TOF       = self.GravitationalWaves_NeutrinoTOF[i][j]
                time_window = self.Constants.TimeWindow
                plt.axvspan( n_emission + n_TOF,
                             n_emission + n_TOF + time_window,facecolor=c,alpha=0.1)

            plt.title('GWEvent'+str(i)+' '+str(int(self.GravitationalWaves[i][1]))+'Mpc')
            plt.axvspan(self.Constants.NeutrinoEmissionRange, 0, facecolor='k', alpha=0.2)
            plt.axis([self.Constants.NeutrinoEmissionRange - 400,3000,0,20])
            plt.xlabel('Time [s]')
            plt.ylabel('Energy [MeV]')
            #plt.legend()
            plt.show()
            
        return 
    
    def plotAllEvents(self):
        import matplotlib.pyplot as plt
        
        ymax = 20
        xmax = self.Constants.LiveTime
        xmin = 0
        
        def neutrinoTimeOfFlight(distance_Mpc,Energy_MeV):
            mn = self.Constants.neutrinoMass
            H0 = self.Constants.H0 * 1000/(3.08568e22)
            Ol = self.Constants.OmegaLambda
            Om =  self.Constants.OmegaMatter
            c = self.Constants.c
            v = H0 *distance_Mpc * 3.08568e22; 
            z = v/c
            E = Energy_MeV*1e6
            const = mn**2/(2*H0*E**2)
            def func(x):
                return const * 1./((1+x)**2 * np.sqrt(Ol+Om*(1+x)**3))
            return quad(func, 0., z)[0]

        
        NExp_accidental_events= 0
        for i in range(len(self.GravitationalWaves.Time)):
            GW_distance = self.GravitationalWaves.Distance[i]
            average_time_window = (self.Constants.NeutrinoEmissionRange[1]-self.Constants.NeutrinoEmissionRange[0])+neutrinoTimeOfFlight(GW_distance,self.NeutrinoEnergyAverage)
            GW_time = self.GravitationalWaves.Time[i]
            plt.vlines(GW_time, ymin = 0,ymax = 100,linestyles='--')
            plt.text(GW_time + self.Constants.LiveTime/100.,0.98*ymax,
                     str(int(self.GravitationalWaves.Distance[i]))+'Mpc',rotation = 90,va = 'top',size=8)
            
            plt.axvspan( self.Constants.NeutrinoEmissionRange[0]+GW_time,
                        self.Constants.NeutrinoEmissionRange[1]+GW_time, facecolor='k',alpha=0.2)
            NExp_accidental_events += 2 * self.NeutrinoRateExpectation * average_time_window 
        
        neutrino_colors = []
        

        for j in range(len(self.Neutrinos.Time)):
                x  = [self.Neutrinos.Time[j]]
                y  = [self.Neutrinos.Energy[j]]
                ym = [-self.Neutrinos.Energy_m[j]]
                yp = [self.Neutrinos.Energy_p[j]]
                p = plt.errorbar(x, y,yerr=[ym,yp], xerr = [0], fmt='o',ms=3, mew=1)
                neutrino_colors.append(p[0].get_color())
        
        plt.text(xmax*0.98,ymax*1.05, 'Total Accidental Exp. = '+str(np.round(NExp_accidental_events,1)) +' Counts',
                 rotation = 0,va = 'top',ha='right',size=10,color = 'k')
        
        self.NeutrinoColors = np.asarray(neutrino_colors)
        
        plt.axis([0,self.Constants.LiveTime,0,ymax])
        plt.xlabel('Time [s]')
        plt.ylabel('Energy [MeV]')
        #plt.legend()
        plt.savefig(self.DataPaths.pdfLocation+'/plotAllEvents.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'/plotAllEvents.pdf')
        
        plt.show()
            
        return 
    
    
    def evalLLH(self):
        
        # Require:
        # 1. 
        return
    
    def plotInterestingEvents(self,axis2):
        for i in range(len(self.GravitationalWaves.Time)): # Loop through GW
            t0 = self.GravitationalWaves.Time[i] # Absolute time of the GW
            NObs = 0
            for j in range(len(self.Neutrinos.Time)): # Loop through Neutrinos
                interesting_event = False
                x  = [self.Neutrinos.Time[j]-t0] # Set the Absolute time of the neutrino relative to the GW.
                c  = self.NeutrinoColors[j]
                acceptance_time_range = self.GravitationalWaves_TimeRange[i][j] +self.Constants.NeutrinoEmissionRange[1]
                
                if x[0] > self.Constants.NeutrinoEmissionRange[0] and  x[0] < acceptance_time_range:
                    # Found a neutrino in the time range.
                    interesting_event = True
                    NObs+=1
                    # Plot it
                    fig, ax1 = plt.subplots()
                    
                    xmin = self.Constants.NeutrinoEmissionRange[0]-200 # axis
                    xmax = acceptance_time_range+1000 # axis
                    ax1.axis([xmin,xmax,0,20]) # axis
                                        
                    window_start = self.Constants.NeutrinoEmissionRange[0] # [0] is the curently -500s
                    window_end   = acceptance_time_range
                    time_window = acceptance_time_range - window_start
                    
                    if axis2:
                        ax2 = ax1.twinx()
                        ymax = 1000
                        ymin = 0.0001
                        ax2.axis([xmin,xmax,ymin,ymax])
                        ax2.set_ylabel('Counts in time window')
                        ax2.set_yscale("log")
                        ax2.axhline(self.NeutrinoRateExpectation*time_window,color ='k',alpha =1,linewidth = 0.5)
                        NExp = str(np.round((self.NeutrinoRateExpectation)*time_window,3)) # The expected number of neutrinos

                        plt.text(xmax*0.98,self.NeutrinoRateExpectation*time_window*0.98,
                             'Exp. = '+NExp +' counts',rotation = 0,va = 'top',ha='right',size=8,color = 'k')

                        x_l = window_start
                        x_h = window_end
                        y_l = (self.NeutrinoRateExpectation-self.NeutrinoRateExpectationUncertainty)*time_window
                        y_h = (self.NeutrinoRateExpectation+self.NeutrinoRateExpectationUncertainty)*time_window
                        ax2.fill([x_l,x_h,x_h,x_l],[y_l,y_l,y_h,y_h],color = 'k',alpha = 0.2)

                        ax2.axhline(NObs,color ='k',alpha =1,linewidth = 0.5)
                        plt.text(xmax*0.98,NObs*0.98,
                             'Obs. = '+str(NObs) +' counts',rotation = 0,va = 'bottom',ha='right',size=8,color = 'k')
                        
                        prob = np.round(scipy.stats.distributions.poisson.pmf(NObs, float(NExp)),3)
                        plt.text(xmax*0.98,ymax*0.9,
                             'P-value = '+str(prob) +' ',rotation = 0,va = 'top',ha='right',size=10,color = 'k')


                    
                    y  = [self.Neutrinos.Energy[j]] # Energy of the neutrino
                    ym = [-self.Neutrinos.Energy_m[j]] # With uncertainties
                    yp = [self.Neutrinos.Energy_p[j]]
                    p = ax1.errorbar(x, y,yerr=[ym,yp], xerr = [0], fmt='o',ms=3, mew=1,color = c)
                    
                    ax1.text(x[0],y[0]+0.1,
                         'E = '+str(np.round(y[0],1))+'MeV',rotation = 0,va = 'bottom',ha='left',size=10,color = c)

                    ax1.vlines(window_end, ymin = 0,ymax = 100,linestyles='-',color = c)
                    ax1.vlines(window_start, ymin = 0,ymax = 100,linestyles='-',color = c)
                    plt.axvspan( window_start,
                                 window_end, facecolor=c,alpha=0.2)
                    plt.axvspan( self.Constants.NeutrinoEmissionRange[0],
                                 self.Constants.NeutrinoEmissionRange[1], facecolor='k', alpha=0.1)
                    
                if interesting_event:
                    plt.title(self.GravitationalWaves.Name[i]+' ('+str(int(self.GravitationalWaves.Distance[i]))+'Mpc)')
                    ax1.vlines(0,ymin = 0,ymax = 100,linestyles='-',color = 'k',alpha = 0.2)
                    #ax1.vlines(self.Constants.NeutrinoEmissionRange[1],ymin = 0,ymax = 100,linestyles=':',color = c,alpha = 0.3)
                    ax1.set_ylabel('Energy [MeV]')
                    plt.xlabel('Time [s]')
                    #plt.legend()
                    plt.savefig(self.DataPaths.pdfLocation+'/plotInterestingEvents_'+self.GravitationalWaves.Name[i]+'.pdf', format='pdf', dpi=200, bbox_inches='tight')
                    print(self.DataPaths.pdfLocation+'/plotInterestingEvents_'+self.GravitationalWaves.Name[i]+'.pdf')
                    plt.show()
            
        return 
    
    
    def calcFeldmanCousins(self,plot=True,confidenceLevel = 0.9):
        lower_limit = []
        upper_limit = []
        for i in range(len(self.GravitationalWaves.Time)):
            if plot:
                fig, ax1 = plt.subplots()
            t0 = self.GravitationalWaves.Time[i] # The absolute time of the gravitational wave
            NObs = 0
            for j in range(len(self.Neutrinos.Time)): # Loop through the neutrinos.
                x  = [self.Neutrinos.Time[j]-t0]
                c  = self.NeutrinoColors[j]
                time_window = [self.Constants.NeutrinoEmissionRange[0], self.GravitationalWaves_TimeRange[i][j] +self.Constants.NeutrinoEmissionRange[1]] # The time window [seconds prior, seconds after]
                time_window_range = time_window[1]-time_window[0]     # Total time window
                if x[0] > time_window[0] and  x[0] < time_window[-1]: # If you see a neutrion in the time window count it.
                    NObs+=1
                    
            Exp = self.NeutrinoRateExpectation*time_window_range # This is the expected number of neutrinos in the time window.
            
            if NObs>3:
                # u = mean number expected. n= number of observed.
                print('You should increase the signal array u, and the n values here.')
                
            b = Exp         # background, or expectation given no signal.
            n_low = []      # Lower limit on Number observed.
            n_high = []     # Upper limit on Number observed.
            
            u = np.arange(0, 6,0.1) # Signal mean. Hypotheses.
            n = np.arange(0,  12,1) # Number of observed events.
  
            for hyp in u: # loop through the different amounts of signal hypotheses. 
                Prob = []
                R    = []
                for n_obs in n: # loop through the different number of observations.
                    hyp_LLH = scipy.stats.distributions.poisson.pmf(n_obs, b+hyp) # Calc poisson probability. Expectation is signal + background = b+hyp. Observed = n_obs.
                    Prob.append(hyp_LLH)
                    LLH = []
                    for u_test in u:
                        LLH.append(scipy.stats.distributions.poisson.pmf(n_obs, b+u_test))
                    max_LLH  = max(LLH) # given n_obs, wat is the best u_test. It's the one with the max LLH.
                    R.append(hyp_LLH/max_LLH) # R factor from FC paper.
                R = np.asarray(R)
                sorted_R, sorted_n, sorted_P = (list(t) for t in zip(*sorted(zip(R, n, Prob),reverse=True))) # sort lists.
                
                sum_P = 0
                accepted_values = []
                for k in range(len(sorted_R)):
                    sum_P += sorted_P[k]
                    if sum_P < confidenceLevel:
                        accepted_values.append(sorted_n[k])
                    else:
                        accepted_values.append(sorted_n[k])
                        break
                n_low.append(min(accepted_values))
                n_high.append(max(accepted_values))
            
            n_low = np.asarray(n_low)
            n_high = np.asarray(n_high)
            
            u_allowed_values = []
            for k in range(len(u)):
                if np.logical_and(NObs >= n_low[k], n_high[k]> NObs):
                    #print(NObs,n_low[k],n_high[k])
                    u_allowed_values.append(u[k])
                    
            lower_limit.append(np.round(min(u_allowed_values),3))
            upper_limit.append(np.round(max(u_allowed_values),3))
            
            if plot:
                for k in range(len(n_low)):
                    plt.plot([n_low[k],n_high[k]],[u[k],u[k]],'k',linewidth = 2)

                plt.plot([NObs+0.5,NObs+0.5],[min(u_allowed_values),max(u_allowed_values)],color = 'r',linewidth = 5)
                
                plt.text(0.2,4.7,
                         r'90% CL: ['+
                         str(np.round(min(u_allowed_values),1))+','+
                         str(np.round(max(u_allowed_values),1))+']',
                         rotation = 0,va = 'top',ha='left',size=10,color ='r')
                
                plt.title(self.GravitationalWaves.Name[i]+' ('+str(int(self.GravitationalWaves.Distance[i]))+'Mpc)')
                plt.xlabel('N Observed')
                plt.ylabel('Mean Signal (u)')
                plt.grid(True)
                plt.axis([0,5,0,5])
                plt.savefig(self.DataPaths.pdfLocation+'/calcFeldmanCousins_'+self.GravitationalWaves.Name[i]+'.pdf', format='pdf', dpi=200, bbox_inches='tight')
                print(self.DataPaths.pdfLocation+'/calcFeldmanCousins_'+self.GravitationalWaves.Name[i]+'.pdf')
                plt.show()
                
                
        self.FCLowerLimits = np.asarray(lower_limit)
        self.FCUpperLimits = np.asarray(upper_limit)
        return 

            
            
    def plotTOF(self,Mn):
        def neutrinoTimeOfFlight(distance_Mpc,Energy_MeV):
            mn = Mn #self.Constants.neutrinoMass
            H0 = self.Constants.H0 * 1000/(3.08568e22)
            Ol = self.Constants.OmegaLambda
            Om =  self.Constants.OmegaMatter
            c = self.Constants.c
            v = H0 *distance_Mpc * 3.08568e22; 
            z = v/c
            E = Energy_MeV*1e6
            const = mn**2/(2*H0*E**2)
            def func(x):
                return const * 1./((1+x)**2 * np.sqrt(Ol+Om*(1+x)**3))
            return quad(func, 0., z)[0]

        fig, ax1 = plt.subplots()
        for distance in self.GravitationalWaves.Distance:
            x = np.linspace(1.8,20,100)
            y = []
            for i in range(len(x)):
                y.append(neutrinoTimeOfFlight(distance,x[i]))
            plt.plot(x,y,label = 'GW '+str(np.round(distance,1)) +'Mpc')
        
        plt.ylabel('Neutrino TOF delay [s]')
        plt.xlabel('Neutrino Energy [MeV]')
        
        xmax = 20
        ymax = 100000
        plt.text(20*0.98,100000*3, r'Time of Flight (M$_N$ = '+str(Mn)+' eV)',
                 rotation = 0,va = 'top',ha='right',size=10,color = 'k')
            
        plt.plot([0,100],[500,500],'k')
        plt.legend()
        ax1.set_yscale('log')
        plt.axis([1.8,xmax,0.01,ymax])
        plt.savefig(self.DataPaths.pdfLocation+'TOF.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'TOF.pdf')
        plt.show()
        return


    def plotNeutrinoEnergyPDF(self,log):
        fig, ax1 = plt.subplots()
        plt.plot(self.Constants.NeutrinoCenterEnergies,  self.NeutrinoEnergyPDF,label='Neutrino Energy PDF')
        plt.ylabel('Arbitrary')
        plt.xlabel('Neutrino Energy [MeV]')
        #plt.text(20*0.98,100000*3, r'Time of Flight (M$_N$ = '+str(self.Constants.neutrinoMass)+' eV)',
        #         rotation = 0,va = 'top',ha='right',size=10,color = 'k')
            
        #plt.plot([0,100],[500,500],'k')
        plt.legend()
        
        plt.axis([0,100,0., max(self.NeutrinoEnergyPDF)])
        if log==True:
            ax1.set_yscale('log')
            plt.axis([0,100,0.0000001, max(self.NeutrinoEnergyPDF)])
        plt.savefig(self.DataPaths.pdfLocation+'plotNeutrinoPDF.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'plotNeutrinoPDF.pdf')
        plt.show()
        return


                
    def calcFluence(self,plot=True):
        E      = self.Constants.NeutrinoCenterEnergies
        NT     = self.Constants.NTargetProtons
        xs     = self.Constants.CrossSection
        Deff   = self.Constants.DetectionEfficiency
        Lambda = self.Constants.NeutrinoSpectrumPDF

        if plot:
            fig, ax1 = plt.subplots()

        for i in range(len(self.GravitationalWaves.Name)):
            lower_CL = self.FCLowerLimits[i]
            upper_CL = self.FCUpperLimits[i]
            fluence_lower = []
            fluence_upper = []
            for j in range(len(E)):
                fluence_lower.append(lower_CL/(NT*xs[j]*Deff[j]*Lambda[j]))
                fluence_upper.append(upper_CL/(NT*xs[j]*Deff[j]*Lambda[j]))

            if plot:
                plt.plot(E,fluence_upper,label = self.GravitationalWaves.Name[i])
                #plt.fill_between(E,fluence_lower,fluence_upper)
            ax1.set_yscale("log")
            plt.xlabel('Energy [MeV]')
            plt.ylabel('Fluence [cm-2]')
            plt.legend()
            lower_limit = []
        plt.savefig(self.DataPaths.pdfLocation+'calcFluence.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'calcFluence.pdf')
        plt.show()
        return
            
            
            
            
            