import numpy as np

dat= np.loadtxt('../vector-file-Kat/analysis_list/dcvf-Kat.list-GRB',dtype={'names':('run','fname'),'formats':('f8','S8')})

runList = dat['run']

dat = np.loadtxt('/data/kamland/const/live-time/livetime-6.0m.dat')
liverun = dat[:,0]
livetime = dat[:,3]

total_livetime1 = 0
total_livetime2 = 0
total_livetime3 = 0
for line in range(len(liverun)):
    if len(liverun[np.where(liverun[line]==runList)]) > 0:
        
        if liverun[line] > 1313 and liverun[line] <= 6820:
            total_livetime1 = total_livetime1 + livetime[line]
        elif liverun[line] > 6890 and liverun[line] <=10675:
            total_livetime2 = total_livetime2 + livetime[line]
        elif liverun[line] > 11000:
            total_livetime3 = total_livetime3 + livetime[line]
            pass
        pass
    pass

total_period1 = total_livetime1/60/60/24/365 + total_livetime2/60/60/24/365
total_period2 = total_livetime3/60/60/24/365
print 'Live time (Period 1): %s' %total_period1
#print 'Live time (Period 2): %s' %(total_livetime2/60/60/24/365)
print 'Live time (Period 2): %s' %total_period2

##Observed-dcvf-Kat_0.9MeV.dat                                                                             
edat0 = np.loadtxt('../kam_data/Observed-dcvf-Kat.dat')
edat1 = np.loadtxt('../kam_data/Observed-dcvf-Kat_0.9MeVwithLikelihood.dat')
#edat1 = np.loadtxt('../kam_data/Observed-dcvf-Kat_0.9MeV.dat')

eventrunList0 = edat0[:,0]
eventrunList1 = edat1[:,0]

total_event1 = 0
total_event2 = 0


eventrunList0 = eventrunList0[np.where( ( (eventrunList0 >= 1313) & (eventrunList0<6820)) |((eventrunList0>=6980) & (eventrunList0<10675)) )]
eventrunList0 = eventrunList0[np.where( eventrunList0 != 5380 )]
total_event1 = len(eventrunList0)


eventrunList1 = eventrunList1[np.where( eventrunList1>11000  )]
total_event3 = len(eventrunList1)
    
print 'Num of DC event (Period 1) (Ep > 7.5MeV): %s' %total_event1
print 'Num of DC event (Period 2) (Ep > 0.9MeV): %s' %total_event3

BG1 = (total_event1)/(total_livetime1/60/60 + total_livetime2/60/60)
BG2 = total_event3/(total_livetime3/60/60)

print 'Backgroud rate (Period 1): %s' %BG1
print 'Backgroud rate (Period 2): %s' %BG2


