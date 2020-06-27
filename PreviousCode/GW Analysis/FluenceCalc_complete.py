## Calculation of fluence corresponding to LIGO GW event analysis paper
## Emmett Krupczak
## 4 May 2016

import numpy as np
import math
import scipy.integrate as integrate


background_livetime = 2.41e-4 #background per second of livetime 
liverun = 0.8967 #livetime to runtime ratio
eff = 0.931 #selection efficiency
NFC = 2.24 #Feldman cousins upper limit 
N_target = 5.98e31 #number of protons in kamland using 6 m cut

def read_file(path):
	with open(path) as f:
		rows = [map(float, line.strip().split(" ")) for line in f.readlines()]
	return zip(*rows)

cstable = read_file("cross_section_table_higherEnergy.dat")
energy, cross_sec = range(2)

#print np.interp(1.9,cstable[energy],cstable[cross_sec])

### Fluence integration ###

#Limits of integration 
ll=1.8 #MeV
ul=111 #MeV

integrand = lambda E: np.interp(E, cstable[energy], cstable[cross_sec])*(E**-2)*eff*N_target*liverun

print "integrand",  integrate.quad(integrand, ll, ul)

fluence = NFC / integrate.quad(integrand, ll, ul)[0] 

print "fluence", "{:.6e}".format(fluence)

flux = lambda E: fluence*E**-2
energy_integrand = lambda E: fluence* E**-1 #flux*E

energy = integrate.quad(energy_integrand, ll, ul)
print "energy", "{:.6e}".format(energy[0]), "error", energy[1]


### Luminosity calculation ###

#distance
d = 410 #Megaparsec
d_cm = 1.26526e27 #cm

luminosity = 4*np.pi*(d_cm**2)*energy[0]

MeV_ergs = 0.0000016021773 #ergs per MeV
luminosity_ergs = luminosity* MeV_ergs

print "luminosity", luminosity_ergs

