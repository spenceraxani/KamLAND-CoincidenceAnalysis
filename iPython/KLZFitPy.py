
import numpy as np
import os
from scipy.integrate import quad
import scipy
import matplotlib.pyplot as plt

class DataPaths:
    def __init__(self, location):
        self.location           = location
        self.resourcesLocation  = location+'/Resources/'
        self.pdfLocation        = location+'/Figures/'
        return
    
    def RealoadPaths(self,location):
        self.location = location
        self.xsLocation = location+'/Resources/'
        return
    

class Constants:
    def __init__(self):
        self.H0 = 67.66                # Hubble constant (km/s/Mpc). From Planck (2018) 
        self.OmegaLambda = 0.6889      # Cosmilogical constant
        self.OmegaMatter = 0.315       # Omega matter density 
        self.c = 299792458             # Speed of light
        self.NTargetProtons = 5.98e31  # Number of target protons. Update from Joe.
        self.neutrinoMass = 0.087        # Average neutrino mass. ?? Should it be the heaviest? 1.0, 0.087
        self.NeutrinoEmissionRange = [-500,500] # Range of allowed neutrino emission time
        self.EventTimeSpan = 0
        self.maxNeutrinoEnergy_MeV = 100
        self.minNeutrinoEnergy_MeV = 1.8
        
        self.LiveTime = float(3600*24*365*0.9) # This is the livetime of the data.
        #self.RealTime = float(3600*24*365)     # The realtime of the data. 
        #self.LiveTimeEfficiency = self.LiveTime/self.RealTime 
        #self.NeutrinoCenterEnergies = np.linspace(1.8,100,983)
        
        self.NeutrinoCenterEnergies = np.linspace(0,100,1001)
        #self.NeutrinoCenterEnergies_True = self.NeutrinoCenterEnergies+0.78
        
        #self.DetectionEfficiency = np.asarray([(0.5*i)/(100)+0.5 for i in self.NeutrinoCenterEnergies])
        #em2_spectrum = np.asarray([i**-2 for i in self.NeutrinoCenterEnergies])
        #em2_spectrum_pdf = em2_spectrum/sum(em2_spectrum)
        #flat_spectrum = np.asarray([1. for i in self.NeutrinoCenterEnergies])
        #flat_spectrum_pdf = flat_spectrum/sum(flat_spectrum)

        return
    
    def setSeed(self,seed):
        np.random.seed(seed)
        return
        
    def setCrossSection(self, cross_section_file):
        # lets have the cross_section be a file:
        energy, cs = np.loadtxt(cross_section_file,unpack=True)
        # linear interp to the desired energy values
        cross_section = np.interp(self.NeutrinoCenterEnergies, energy, cs)
        self.CrossSection = cross_section
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
        self.err_neg     = np.asarray(GWArray[:,3]).astype(float)
        self.err_pos     = np.asarray(GWArray[:,4]).astype(float)
        return
    
class NeutrinoClass:
    def __init__(self,NeutrinoArray):
        self.Time         = np.asarray(NeutrinoArray[:,0]).astype(float) 
        self.Energy       = np.asarray(NeutrinoArray[:,1]).astype(float)
        self.Energy_m     = np.asarray(NeutrinoArray[:,2]).astype(float)
        self.Energy_p     = np.asarray(NeutrinoArray[:,3]).astype(float)
        return
    
class KLZFit:
    def __init__(self, DataPaths, Constants,FitParameters):
        self.DataPaths        = DataPaths
        self.Constants        = Constants
        self.FitParameters    = FitParameters
        self.Neutrinos        = 'None'
        self.GravitationalWaves = 'None'
    
    def setDetectionEfficiency(self, detection_efficiency_file):
        # We want detection efficiency as a function of true neutrino energy
        with open(detection_efficiency_file) as f:
            content = f.readlines()
        delim = ' '
        for line in content:
            if ',' in line:
                delim =','
                
        # Upload detection efficiency as a function of measured neutrino energy , from thesis.   
        energy, efficiency = np.loadtxt(detection_efficiency_file,unpack=True,delimiter=delim) 
        
        # Convert measured energy to true energy:
        energy = energy + 0.78
        
        # linear interp to the energy values that we are using in the analysis
        efficiency = np.interp(self.Constants.NeutrinoCenterEnergies, energy, efficiency)
        self.DetectionEfficiency = efficiency
        #self.DetectionEfficiency_ = efficiency
        return 
   

    def setNeutrinoEnergyPDF(self, neutrino_energy_file):
        # This function takes in the measured energy distribution from a apaper or thesis, 
        # and converts it to a true neutrino energy PDF. You need to also have the 
        # detection efficiency of the measured energy distribution.
        
        # neutrino_energy_file should be 2d array, [energy[MeV], number of neutrinos]
        # The file should be the measured energy distribution. Check to see if the file has a comma in it.
        with open(neutrino_energy_file) as f:
              content = f.readlines()
        delimiter = ' '
        for line in content:
            if ',' in line:
                delimiter =','
        
        # Now, convert the Measured energy distribution to a true neutrino energy distribution.
        
        # Upload the measured energy distribution from a thesis.
        energy, relative_rate = np.loadtxt(neutrino_energy_file, unpack = True, delimiter = delimiter)
        
        # Convert KamLAND measured energy to true energy, just add 0.78MeV.
        true_energy = energy + 0.78 # See https://arxiv.org/pdf/1503.02137.pdf
        
        # make sure that the true energy pdf is 0 below 1.8MeV
        for i in range(len(true_energy)):
            if true_energy[i] <1.8:
                relative_rate[i] = 0
        
        # Interpolate so we have data at our desired energy values, NeutrinoCenterEnergies:
        relative_rate               = np.interp(self.Constants.NeutrinoCenterEnergies, true_energy, relative_rate)        
        
        
        
        # Account for the detection efficiency:
        relative_rate = relative_rate/self.DetectionEfficiency
        
        # Normalize so that it is a PDF
        self.NeutrinoEnergyPDF        = relative_rate/sum(relative_rate)
        
        # Get the true average energy, for fun
        self.NeutrinoEnergyAverage    = sum(self.NeutrinoEnergyPDF *
                                            self.Constants.NeutrinoCenterEnergies)
        
        print('Average True Neutrino Energy in sample: '+str(np.round(self.NeutrinoEnergyAverage,2))+'MeV')
        return
    
    
    def write_to_file(self,array,outfile_name,delimiter = ','):
        f = open(outfile_name, "w")
        for i in range(len(array)):
            outstring = ''
            for j in range(len(array[i])):
                outstring+= str(array[i][j])
                if j < len(array[i])-1:
                    outstring+=', '
            f.write(outstring+'\n')
        f.close()
        return
    
    
    def setNeutrinos(self,realization_file):
        # set the neutrinos for the analysis from a file containing the neutrino information.
        # File should be array of Time, Measured energy (MeV), Energy uncertainty - (MeV), Energy uncertainty + (MeV), 
        with open(realization_file) as f:
              content = f.readlines()
        delim = ' '
        for line in content:
            if ',' in line:
                delim =','
                
        data = np.loadtxt(realization_file, unpack=True, delimiter=delim)
        data = np.transpose(data)
        neutrino_array = []
        for i in range(len(data)):
            neutrino_array.append(np.ndarray.tolist(data[i]))
        
        Message = 'Found '+str(len(neutrino_array)) +'Number of neutrinos'
        os.write(1, Message.encode())
        
        neutrino_array            = np.asarray(neutrino_array) 
        NeutrinoEvents            = NeutrinoClass(neutrino_array)
        self.Neutrinos            = NeutrinoEvents
        self.NeutrinoColors       = np.asarray(['k']*len(neutrino_array))
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
            Om = self.Constants.OmegaMatter
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
            GWDist    = self.GravitationalWaves.Distance[i]
            '''
            if FitParameters.GWDistance < 0:
                GWDist    = self.GravitationalWaves.Distance[i] +\
                            FitParameters.GWDistance * self.GravitationalWaves.Distance_m[i]
            else:
                GWDist    = self.GravitationalWaves.Distance[i] +\
                            FitParameters.GWDistance * self.GravitationalWaves.Distance_p[i]
            '''
            max_tof   = neutrinoTimeOfFlight(GWDist, 1.8)
            neutrino_energies = []
            for j in range(len(self.Neutrinos.Time)):
                NTime = self.Neutrinos.Time[j]
                neutrino_energies.append(self.Neutrinos.Energy[j])
                '''
                if FitParameters.NeutrinoEnergy < 0:
                    neutrino_energies.append(self.Neutrinos.Energy[j] +\
                                             self.FitParameters.NeutrinoEnergy*self.Neutrinos.Energy_m[j])
                else:
                    neutrino_energies.append(self.Neutrinos.Energy[j] +\
                                             self.FitParameters.NeutrinoEnergy*self.Neutrinos.Energy_p[j])
                '''
            TOF_neutrinos   = np.asarray([neutrinoTimeOfFlight(GWDist, j) for j in neutrino_energies])
            GWNeutrinoTOF.append(TOF_neutrinos)
        self.GravitationalWaves_TimeRange    = np.asarray(GWNeutrinoTOF)
        return 
    
    
    def getNeutrinoRealization(self,Rate):
        # This function returns a list of fake neutrinos.
        # Each element in the list is one neutrino.
        # Each neutrino is defined by its interaction time, and measured Energy with uncertainty.
        
        # This makes some fake neutrinos from the Neutrino Energy PDF.
        # Rate = neutrino rate that you want.
        try:
            NeutrinoEnergyPDF = self.NeutrinoEnergyPDF
        except:
            print('You need to define the self.NeutrinoEnergyPDF before getting a realization.')
        
        # Randomly Generate neutrinos from the NeutrinoEnergyPDF
        cdf             = np.cumsum(NeutrinoEnergyPDF)
        cdf             = cdf / cdf[-1]
        random_values   = np.random.rand(int(self.Constants.LiveTime*Rate)) 
        value_bins      = np.searchsorted(cdf, random_values) 
        random_true_neutrino_energies = self.Constants.NeutrinoCenterEnergies[value_bins]
        NeutrinoEvents  = []
        
        # Loop through the number of desired neutrinos
        for i in range(len(random_true_neutrino_energies)):
            # The true energy has to be above 1.8MeV to interact through IBD:
            if random_true_neutrino_energies[i] > 1.8:
                # Convert from true neutrino energy to a measured (prompt) energy
                prompt_energy = random_true_neutrino_energies[i] - 0.78
                
                # Energy uncertainty for KamLAND is 6.4%/sqrt(MeV). 
                energy_uncertainty_percentage = 0.064/np.sqrt(prompt_energy) # This is a percentage
                
                # Smear the measured energy by the energy uncertainty
                smeared_energy = np.random.normal(prompt_energy, prompt_energy*energy_uncertainty_percentage)
                
                # Calcualte the +/- uncertainties in MeV units for the energy
                smeared_energy_uncertainty_low = -smeared_energy*energy_uncertainty_percentage
                smeared_energy_uncertainty_high = smeared_energy*energy_uncertainty_percentage

                # Assign the neutrion a random interaction time within the livetime of the analysis:
                Neutrino_interaction_time = np.random.uniform(low=0,high=self.Constants.LiveTime,size=(1,))[0]
                
                # Make a list of our neutrinos.
                NeutrinoEvents.append([Neutrino_interaction_time, # When did it interact?
                                       smeared_energy, # What energy did KamLAND measure
                                       smeared_energy_uncertainty_low, # What is the assigned uncertainty
                                       smeared_energy_uncertainty_high]) # What is the assigned uncertainty

        self.NeutrinoRateExpectation = len(random_true_neutrino_energies)/self.Constants.LiveTime
        self.NeutrinoRateExpectationUncertainty = np.sqrt(len(random_true_neutrino_energies))/self.Constants.LiveTime
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
    
    def plotAllEvents(self,plot_GW):
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

        #NExp_accidental_events= 0
        
        for i in range(len(self.GravitationalWaves.Time)):
            GW_distance = self.GravitationalWaves.Distance[i]
            average_time_window = self.Constants.NeutrinoEmissionRange[1]-\
                                    self.Constants.NeutrinoEmissionRange[0]+\
                                    neutrinoTimeOfFlight(GW_distance,self.NeutrinoEnergyAverage)+\
                                    self.Constants.EventTimeSpan
                        
            GW_time = self.GravitationalWaves.Time[i]
            
            # Accidental rate = 2 * Neutrino Rate * GW Rate * Time window
            
            NExp_accidental_events_low = 0
            NExp_accidental_events_high = 0
            window_range = (self.Constants.NeutrinoEmissionRange[1] - self.Constants.NeutrinoEmissionRange[0] + self.Constants.EventTimeSpan)
            print()
            
            if plot_GW:
                plt.vlines(GW_time, ymin = 0,ymax = 100,linestyles='--')
                plt.text(GW_time + self.Constants.LiveTime/100.,0.98*ymax,
                         str(int(self.GravitationalWaves.Distance[i]))+'Mpc',
                         rotation = 90,va = 'top',size=8)

                plt.axvspan(self.Constants.NeutrinoEmissionRange[0]+\
                            GW_time,
                            self.Constants.NeutrinoEmissionRange[1]+\
                            GW_time+\
                            self.Constants.EventTimeSpan, 
                            facecolor='k',alpha=0.2)
            
                max_TOF = neutrinoTimeOfFlight(self.GravitationalWaves.Distance[i],1.8)
            
                NExp_accidental_events_low += 2 *\
                                self.NeutrinoRateExpectation *\
                                1. *\
                                window_range
        
                NExp_accidental_events_high += 2 *\
                                self.NeutrinoRateExpectation *\
                                1. *\
                                (window_range+max_TOF)
            
        neutrino_colors = []
        
        for j in range(len(self.Neutrinos.Time)):
                x  = [self.Neutrinos.Time[j]]
                y  = [self.Neutrinos.Energy[j]]
                ym = [-self.Neutrinos.Energy_m[j]]
                yp = [self.Neutrinos.Energy_p[j]]
                p = plt.errorbar(x, y,yerr=[ym,yp], xerr = [0], fmt='o',ms=1, mew=1)
                neutrino_colors.append(p[0].get_color())
        
        plt.text(xmax*0.98,ymax*1.05, 'Total Accidental Exp. = '+str(np.round(NExp_accidental_events_low,2)) +\
                         '-'+str(np.round(NExp_accidental_events_high,2))+' Events',
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
    
    def plotMaxRangeEachEvents(self,plot_GW):
        # The absolute maximum time window, assuming lowest energy (1.8MeV), and highest mass (1eV).
        import matplotlib.pyplot as plt

        def neutrinoTimeOfFlight(distance_Mpc, Energy_MeV):
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
        
        for i in range(len(self.GravitationalWaves.Time)):
            fig, ax1 = plt.subplots()
            GW_distance = self.GravitationalWaves.Distance[i]
            GW_time = self.GravitationalWaves.Time[i]
            ymax = max(self.Neutrinos.Energy)*1.2
            
            if plot_GW:
                plt.vlines(0, ymin = 0,ymax = 100,linestyles='--')
                plt.text(0,0.98*ymax,
                         str(int(self.GravitationalWaves.Distance[i]))+'Mpc',rotation = 90,va = 'top',size=8)

                plt.axvspan(self.Constants.NeutrinoEmissionRange[0],
                            self.Constants.NeutrinoEmissionRange[1]+
                            neutrinoTimeOfFlight(GW_distance,1.8)+
                            self.Constants.EventTimeSpan, 
                            facecolor='k',alpha=0.1)
                
                plt.axvspan(self.Constants.NeutrinoEmissionRange[0],
                            self.Constants.NeutrinoEmissionRange[1], 
                            facecolor='k',alpha=0.1)
                
                plt.axvspan(self.Constants.NeutrinoEmissionRange[0],
                            self.Constants.NeutrinoEmissionRange[1]+self.Constants.EventTimeSpan, 
                            facecolor='k',alpha=0.1)
                
            for j in range(len(self.Neutrinos.Time)):
                    x  = [self.Neutrinos.Time[j]-GW_time]
                    y  = [self.Neutrinos.Energy[j]]
                    ym = [-self.Neutrinos.Energy_m[j]]
                    yp = [self.Neutrinos.Energy_p[j]]
                    p = plt.errorbar(x, y,yerr=[ym,yp], xerr = [0], fmt='o', color = self.NeutrinoColors[j], ms=3, mew=1)

                    
            tof_1p8MeV = neutrinoTimeOfFlight(GW_distance,1.8)
            plot_extent = 0.1*(self.Constants.NeutrinoEmissionRange[1]+tof_1p8MeV+self.Constants.EventTimeSpan)
            
            plt.axis([self.Constants.NeutrinoEmissionRange[0]-plot_extent,
                      self.Constants.NeutrinoEmissionRange[1]+tof_1p8MeV+self.Constants.EventTimeSpan + plot_extent,
                      0,ymax])
            
            plt.vlines(self.Constants.NeutrinoEmissionRange[0], 
                       ymin = 0,ymax = 100,linestyles='-')
            
            plt.vlines(self.Constants.NeutrinoEmissionRange[1]+
                       tof_1p8MeV+
                       self.Constants.EventTimeSpan, 
                       ymin = 0,ymax = 100,linestyles='-')
            
            plt.vlines(self.Constants.NeutrinoEmissionRange[1],
                       ymin = 0,ymax = 100,linestyles=':')
            
            plt.vlines(self.Constants.NeutrinoEmissionRange[1]+
                       self.Constants.EventTimeSpan, 
                       ymin = 0,ymax = 100,linestyles=':')
            
            plt.title(self.GravitationalWaves.Name[i]+' ('+str(int(self.GravitationalWaves.Distance[i]))+'Mpc)')

            plt.xlabel('Time [s]')
            plt.ylabel('Energy [MeV]')
            plt.savefig(self.DataPaths.pdfLocation+'/plotEachEvents'+str(i)+'.pdf', format='pdf', dpi=200, bbox_inches='tight')
            print(self.DataPaths.pdfLocation+'/plotAllEvents'+str(i)+'.pdf')
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
                tn  = [self.Neutrinos.Time[j]-t0] # Set the Absolute time of the neutrino relative to the GW.
                c   = self.NeutrinoColors[j] # Just color of the neutrino
                
                window_start = self.Constants.NeutrinoEmissionRange[0] # [0] is the curently -500s
                window_end   = self.GravitationalWaves_TimeRange[i][j]+\
                               self.Constants.NeutrinoEmissionRange[1]+\
                               self.Constants.EventTimeSpan
                time_window  = window_end - window_start
                
                if tn[0] > window_start and  tn[0] < window_end:
                    # Found a neutrino in the time range.
                    interesting_event = True
                    NObs+=1
                    
                    # Plot it
                    fig, ax1 = plt.subplots()
                    xmin = -0.1 * window_end + window_start # axis
                    xmax = 1.1 * window_end  # +1000 # axis
                    ax1.axis([xmin,xmax,0,20]) # axis

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
                    p = ax1.errorbar(tn, y,yerr=[ym,yp], xerr = [0], fmt='o',ms=3, mew=1,color = c)
                    
                    ax1.text(tn[0],y[0]+0.1,
                         'E = '+str(np.round(y[0],1))+'MeV',rotation = 0,va = 'bottom',ha='left',size=10,color = c)
                    
                    ax1.vlines(window_end, ymin = 0,ymax = 100,linestyles='-',color = c)
                    ax1.vlines(window_start, ymin = 0,ymax = 100,linestyles='-',color = c)
                    plt.axvspan( window_start,
                                 window_end, facecolor=c,alpha=0.2)
                    plt.axvspan( self.Constants.NeutrinoEmissionRange[0],
                                 self.Constants.NeutrinoEmissionRange[1], facecolor='k', alpha=0.1)
                    
                if interesting_event:
                    plt.title(self.GravitationalWaves.Name[i]+' ('+str(int(self.GravitationalWaves.Distance[i]))+'Mpc)')
                    ax1.vlines(0,ymin = 0,ymax = 100,linestyles='--',color = 'k',alpha = 1)
                    #ax1.vlines(self.Constants.NeutrinoEmissionRange[1],ymin = 0,ymax = 100,linestyles=':',color = c,alpha = 0.3)
                    ax1.set_ylabel('Energy [MeV]')
                    plt.xlabel('Time [s]')
                    #plt.legend()
                    plt.savefig(self.DataPaths.pdfLocation+'/plotInterestingEvents_'+self.GravitationalWaves.Name[i]+'.pdf', format='pdf', dpi=200, bbox_inches='tight')
                    print(self.DataPaths.pdfLocation+'/plotInterestingEvents_'+self.GravitationalWaves.Name[i]+'.pdf')
                    plt.show()
            
        return 
    
    
                
    def calcFluence(self,plot=True):
        E      = self.Constants.NeutrinoCenterEnergies
        NT     = self.Constants.NTargetProtons
        xs     = self.Constants.CrossSection
        Deff   = self.DetectionEfficiency
        Lambda = self.NeutrinoEnergyPDF

        if plot:
            fig, ax1 = plt.subplots()

        for i in range(len(self.GravitationalWaves.Name)):
            lower_CL = self.FCLowerLimits[i]
            upper_CL = self.FCUpperLimits[i]
            fluence_lower = []
            fluence_upper = []
            for j in range(len(E)):
                fluence_lower.append(lower_CL/(NT*xs[j]*Deff[j]))#*Lambda[j]))
                fluence_upper.append(upper_CL/(NT*xs[j]*Deff[j]))#*Lambda[j]))

            if plot:
                plt.plot(E,fluence_upper,label = self.GravitationalWaves.Name[i])
                #plt.fill_between(E,fluence_lower,fluence_upper)
            ax1.set_yscale("log")
            plt.xlabel('Energy [MeV]')
            plt.ylabel('Fluence [cm-2]')
            plt.legend()
            lower_limit = []
            
        plt.axvline(x = 1.8,linestyle=':',color = 'k')
        plt.xlim([0,100])
        plt.savefig(self.DataPaths.pdfLocation+'calcFluence.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'calcFluence.pdf')
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
                time_window = [self.Constants.NeutrinoEmissionRange[0], 
                               self.GravitationalWaves_TimeRange[i][j]+\
                               self.Constants.EventTimeSpan+\
                               self.Constants.NeutrinoEmissionRange[1]] # The time window [seconds prior, seconds after]
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
                if len(accepted_values) == 1:
                    n_high.append(1)
                else:
                    n_high.append(max(accepted_values))

                #n_low.append(min(accepted_values))
                #n_high.append(max(accepted_values))
                #
                
            
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

            
            
    def plotNeutrinoTOF(self,Mn):
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

    def plotNeutrinoEnergyPDF(self,log,xmin = 0,xmax = 20,plot_realization=True, prompt= True):
        fig, ax1 = plt.subplots()
        # If prompt == True, then plot the measured energy distribution, with the measured energy PDF.
        if prompt: 
            plt.plot(self.Constants.NeutrinoCenterEnergies-0.78,self.NeutrinoEnergyPDF, label='Prompt Neutrino Energy PDF')
            plt.xlabel('Prompt Neutrino Energy [MeV]')
            # if plot_realization==True, plot the fake neutrinos as well as a histogram.
            if plot_realization:
                # The neutrino events energy is a prompt energy
                # print(min(self.Neutrinos.Energy))
                
                counts = plt.hist(self.Neutrinos.Energy, bins = self.Constants.NeutrinoCenterEnergies,
                         weights = np.ones(len(self.Neutrinos.Energy))/len(self.Neutrinos.Energy),
                         label   = 'Realization (N='+str(len(self.Neutrinos.Energy))+')')

        else:
            # if not prompt, plot the true energy
            plt.plot(self.Constants.NeutrinoCenterEnergies,self.NeutrinoEnergyPDF, label='Neutrino Energy PDF')
            plt.xlabel('True Neutrino Energy [MeV]')
            plt.axvline(x = 1.8,linestyle=':',color = 'k')
            if plot_realization:
                counts = plt.hist(self.Neutrinos.Energy+0.78, bins = self.Constants.NeutrinoCenterEnergies,
                         weights = np.ones(len(self.Neutrinos.Energy))/len(self.Neutrinos.Energy),
                         label   = 'Realization (N='+str(len(self.Neutrinos.Energy))+')')
            
        plt.ylabel('Arbitrary')

        plt.axis([xmin,xmax,0., 1.2*max(self.NeutrinoEnergyPDF)])
        plt.legend()
        plt.savefig(self.DataPaths.pdfLocation+'plotNeutrinoPDF.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'plotNeutrinoPDF.pdf')
        plt.show()
        return

    def plotDetectionEfficiency(self,xmax = 0,xmin=100):
        import matplotlib.pyplot as plt
        fig, ax1 = plt.subplots()

        plt.plot(self.Constants.NeutrinoCenterEnergies,
                 self.DetectionEfficiency,
                 label = 'Detection Efficiency',linewidth = 2)
        
        plt.axvline(x = 1.8,linestyle=':',color = 'k')
        
        ymax = 1.0
        ymin = 0
        plt.axis([xmin,xmax,ymin,ymax])
        plt.xlabel('True Neutrino Energy [MeV]')
        plt.ylabel(r'Detection Efficiency')
        plt.legend()
        
        plt.savefig(self.DataPaths.pdfLocation+'/plotDetectionEfficiency.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'/plotDetectionEfficiency.pdf')
        plt.show()
            
        return 
    
    def plotCrossSection(self,*arg):
        import matplotlib.pyplot as plt
        fig, ax1 = plt.subplots()
        
        for i in range(len(arg)):
            energy, cs = np.loadtxt(arg[i],unpack=True)
            plt.plot(energy,cs,label = 'XSFile '+str(i),linewidth = len(arg)*2-i*2)
        
        plt.plot(self.Constants.NeutrinoCenterEnergies,self.Constants.CrossSection,label = 'Current KLZFit XS',linewidth = 1)
        
        plt.axvline(x=1.8,linestyle=':',color = 'k')
        
        ymax = max(self.Constants.CrossSection)
        ymin = sorted(list(set(self.Constants.CrossSection)))[2]
        xmax = max(self.Constants.NeutrinoCenterEnergies)
        xmin = min(self.Constants.NeutrinoCenterEnergies)
        
        plt.axis([0,xmax,ymin,ymax])
        ax1.set_yscale('log')
        plt.xlabel('Neutrino Energy [MeV]')
        plt.ylabel(r'IBD Cross Section (cm$^2$)')
        plt.legend()
        plt.savefig(self.DataPaths.pdfLocation+'/plotCrossSection.pdf', format='pdf', dpi=200, bbox_inches='tight')
        print(self.DataPaths.pdfLocation+'/plotCrossSection.pdf')
        plt.show()
            
        return             
            