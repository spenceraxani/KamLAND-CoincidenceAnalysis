import numpy as np
import mydef,ROOT
dat0  = np.loadtxt("../kam_data/cross_section_table2.data")
EnuList = dat0[:,0]
EpList = EnuList - 0.78

[EvList,EffList]  = np.loadtxt("/data/work/koji/nearby_supernova/old/selection_eff/SelectionEfficiencyDS3.dat",unpack=True)

NewEffList =[]
for line in range(len(EpList)):
    for line2 in range(1,len(EvList)):
        if (EpList[line] > EvList[line2-1] ) and (EpList[line] <= EvList[line2]):

            a = (EffList[line2] - EffList[line2-1])/(EvList[line2]-EvList[line2-1])
            b = EffList[line2-1] - a * EvList[line2-1]
            NewEffList.append(a * EpList[line] + b)
            pass
        pass
    if EpList[line] > EvList[-1]:
        NewEffList.append(EffList[-2])

        pass
        
    pass

NewEffList = np.array(NewEffList)

g1=mydef.make_graph( xList=EvList, yList=EffList, name='Neutrino energy',
                     title='Presupernova neutrino (nu_e)', xtitle='Energy [MeV]', ytitle='Flux', iColor=2, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=None, xmax=None, ymin=None, ymax=None)

g2=mydef.make_graph( xList=EpList, yList=NewEffList, name='Neutrino energy',
                     title='Presupernova neutrino (nu_x)', xtitle='Energy [MeV]', ytitle='Flux', iColor=4, iSize=2, iStyle=2,
                     errPlot=False,errxList=None, erryList=None,
                     xmin=None, xmax=None, ymin=None, ymax=None)



c0 = ROOT.TCanvas('plot', 'plot', 600, 600)
c0.Divide(1,1)
c0.Draw()
c0.cd(1)

g1.Draw('ALP')
g2.Draw('LP')



c0.Update()


f = open("SelectionEfficiencyDS3withCompleteness.dat", 'w')

for line in range(len(EpList)):
    f.writelines( '%s %s' %(EpList[line],NewEffList[line]) + str('\n'))   
    pass
f.close()
