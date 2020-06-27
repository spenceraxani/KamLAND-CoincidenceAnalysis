# -- neutrino candidate
#TARGET         = example-DelayedCoincidence-and-Veto-Neutrino

#TARGET		= example-DelayedCoincidence-and-Veto-SolarAntiNeutrino
#TARGET		= example-DelayedCoincidence-and-Veto-SolarAntiNeutrino-temporaryfile

#TARGET		= example-DelayedCoincidence-and-Veto-Accidental-SolarAntiNeutrino
#TARGET  = hoge

#TARGET		= example-Read-DelayedCoincidenceEvent-LikelihoodRatio-HighEnergy-shimizu

#TARGET		= example-Read-DelayedCoincidenceEvent-LikelihoodRatio-HighEnergy-shimizu-ishidoshiro
TARGET		= example-Read-DelayedCoincidenceEvent-LikelihoodRatio-HighEnergy-shimizu-ishidoshiro-temporary
#TARGET		= test

#TARGET		= DelayedCoincidence-and-Veto-Accidental-Fast
#TARGET		= example-DelayedCoincidence-and-Muon-for-8He9Li
#TARGET		= KVFParticleEnergyKat

#TARGET		= example-Read-DelayedCoincidenceEvent-Muon-LikelihoodRatio
#TARGET		= example-Read-DelayedCoincidenceEvent-Muon-LikelihoodRatio-XePhase
#TARGET		= LiveTime
#TARGET		= root_DelayedCoincidence-atmospheric-test
#TARGET		= root_DelayedCoincidence-atmospheric
#TARGET		= root_DelayedCoincidence-all-check
#TARGET		= root_DelayedCoincidence-FastNeutron-scale
#TARGET		= root_DelayedCoincidence-SolarNu-scale



# 
# debug mode trigger
#  SETDEBUG=0 : Optimization (default)
#  SETDEBUG=1 : Debug mode 


SETDEBUG=0
#SETDEBUG=1



#
# check system
#

OSNAME		= $(shell uname -s)
MTNAME          = $(shell uname -m)


ifeq ($(OSNAME),AIX)
CXX=xlC
BITMODE=64
DEBUGFLAGS=-g -qdbxextra -qfullpath -q${BITMODE}
OPTIMIZEFLAGS=-O3 -qarch=pwr4 -qtune=pwr4 -qmaxmem=-1 -q${BITMODE}

CXXFLAGS=
LDFLAGS= -Wl,-bhalt:8
GLLIBS= -lXGLW -lX11 -lXm -lXmu
ARFLAGS=-X${BITMODE} 
ECHO=echo
else
CXX=g++
DEBUGFLAGS=-g -Wall -ggdb3
OPTIMIZEFLAGS=-O2
ifeq ($(MTNAME),x86_64)
BITMODE=64
else
BITMODE=32
endif

DEBUGFLAGS=-m${BITMODE} -g -Wall -ggdb3
OPTIMIZEFLAGS=-m${BITMODE} -O2

CXXFLAGS= -Wno-deprecated
LDFLAGS= 
ARFLAGS=
ECHO=echo -e
endif

#
# change flags 
#

ifeq ($(SETDEBUG),1)
CXXFLAGS+=$(DEBUGFLAGS)
LDEXTRA=$(THISSYSTEM)$(BITMODE)_Debug
EXTRASUFFIX=_Debug
else
CXXFLAGS+=$(OPTIMIZEFLAGS)
LDEXTRA=$(THISSYSTEM)$(BITMODE)
EXTRASUFFIX=
endif

SUFFIX=${OSNAME}${BITMODE}${EXTRASUFFIX}

#
# setup for libKVF
#
# for old system (teena, count, mojabo)
#KLDIR           = /home/k/kamland/tool

# for new system (abby)
KLDIR          = /data/kamland/tool/${BITMODE}/KamLAND

LIBNAME		= libKVF_${SUFFIX}.a

#CXXFLAGS        += -I${KLDIR}/include
#LDFLAGS       	+= -lm -L${KLDIR}/lib -lLiveTime_${SUFFIX} -lKSync_${SUFFIX} -lKMath_${SUFFIX}

CXXFLAGS        += -I../livetime/ -I${KLDIR}/include
LDFLAGS       	+= -lm -L../livetime/ -lLiveTime_${SUFFIX} -L${KLDIR}/lib -lKSync_${SUFFIX} -lKMath_${SUFFIX}


CXXFLAGS       += $(shell root-config --cflags)
LDFLAGS       	+= $(shell root-config --libs) 
LDFLAGS       	+= $(shell root-config --glibs) 

OBJLISTS	= KVFVirtualEvent.o KVFWriter.o KVFReader.o
OBJLISTS        += KVFVirtualHeader.o KVFHeader.o
OBJLISTS	+= KVFConstHandler.o
OBJLISTS	+= KVFEnergyCorrection.o
OBJLISTS	+= KVFGeneralEvent.o
OBJLISTS	+= KVFDelayedCoincidenceEvent.o
OBJLISTS	+= KVFDelayedCoincidenceEventMuon.o
OBJLISTS	+= KVFSingleEvent.o
OBJLISTS	+= KVFSpallationCut.o
OBJLISTS	+= KVFParticleEnergy.o
OBJLISTS	+= KVFDelayedEnergy.o
OBJLISTS	+= KVFRunInfo.o
#OBJLISTS	+= KVFPreTriggEffCorr.o
OBJLISTS        += KVFLikelihoodRatio.o
OBJLISTS        += KVFResidualCharge.o


###
#OBJLISTS        += KVFVertexCorrection.o
###


OBJS=$(patsubst %.o,%.${SUFFIX}.o,$(OBJLISTS))

EXAMPLE		= ${TARGET}.${SUFFIX}.o

all: examples

examples: ${OBJS} ${EXAMPLE}
	${CXX} ${CXXFLAGS}  -o ${TARGET}.${OSNAME} ${EXAMPLE} ${OBJS} ${LDFLAGS}

%.${SUFFIX}.o: %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	rm -rf ${OBJS} *~ core ${TARGET} ${LIBNAME} ${RTARGET} ${WTARGET}

lib: ${OBJS}
	rm -f ${LIBNAME}
	ar ${ARFLAGS} -rv ${LIBNAME} ${OBJS}

kamland: lib
	cp -f *.hh ${KLDIR}/include/
	rm -f *.o
