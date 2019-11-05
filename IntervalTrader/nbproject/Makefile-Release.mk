#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AppIntervalTrader.o \
	${OBJECTDIR}/Instance.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 `/usr/local/bin/wx-config --cxxflags` 
CXXFLAGS=-m64 `/usr/local/bin/wx-config --cxxflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib -Wl,-rpath,'/usr/local/lib' ../lib/TFBitsNPieces/dist/Release/GNU-Linux/libtfbitsnpieces.a ../lib/TFIQFeed/dist/Release/GNU-Linux/libtfiqfeed.a ../lib/TFInteractiveBrokers/dist/Release/GNU-Linux/libtfinteractivebrokers.a ../lib/TFOptions/dist/Release/GNU-Linux/libtfoptions.a ../lib/TFVuTrading/dist/Release/GNU-Linux/libtfvutrading.a ../lib/TFTrading/dist/Release/GNU-Linux/libtftrading.a ../lib/OUCharting/dist/Release/GNU-Linux/liboucharting.a ../lib/OUSqlite/dist/Release/GNU-Linux/libousqlite.a ../lib/OUSQL/dist/Release/GNU-Linux/libousql.a ../lib/OUCommon/dist/Release/GNU-Linux/liboucommon.a ../lib/TFTimeSeries/dist/Release/GNU-Linux/libtftimeseries.a ../lib/TFHDF5TimeSeries/dist/Release/GNU-Linux/libtfhdf5timeseries.a -lhdf5_cpp -lhdf5 -lsz -lpthread -ldl -lz -lchartdir -lboost_date_time-gcc8-mt-x64-1_69 -lboost_filesystem-gcc8-mt-x64-1_69 -lboost_regex-gcc8-mt-x64-1_69 -lboost_serialization-gcc8-mt-x64-1_69 -lboost_system-gcc8-mt-x64-1_69 -lboost_thread-gcc8-mt-x64-1_69

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFBitsNPieces/dist/Release/GNU-Linux/libtfbitsnpieces.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFIQFeed/dist/Release/GNU-Linux/libtfiqfeed.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFInteractiveBrokers/dist/Release/GNU-Linux/libtfinteractivebrokers.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFOptions/dist/Release/GNU-Linux/libtfoptions.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFVuTrading/dist/Release/GNU-Linux/libtfvutrading.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFTrading/dist/Release/GNU-Linux/libtftrading.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/OUCharting/dist/Release/GNU-Linux/liboucharting.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/OUSqlite/dist/Release/GNU-Linux/libousqlite.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/OUSQL/dist/Release/GNU-Linux/libousql.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/OUCommon/dist/Release/GNU-Linux/liboucommon.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFTimeSeries/dist/Release/GNU-Linux/libtftimeseries.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ../lib/TFHDF5TimeSeries/dist/Release/GNU-Linux/libtfhdf5timeseries.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/intervaltrader ${OBJECTFILES} ${LDLIBSOPTIONS} `/usr/local/bin/wx-config --libs`

${OBJECTDIR}/AppIntervalTrader.o: AppIntervalTrader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DwxUSE_GUI -I../lib -std=c++17 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AppIntervalTrader.o AppIntervalTrader.cpp

${OBJECTDIR}/Instance.o: Instance.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DwxUSE_GUI -I../lib -std=c++17 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Instance.o Instance.cpp

# Subprojects
.build-subprojects:
	cd ../lib/TFBitsNPieces && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFIQFeed && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFInteractiveBrokers && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFOptions && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFVuTrading && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFTrading && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/OUCharting && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/OUSqlite && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/OUSQL && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/OUCommon && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFTimeSeries && ${MAKE} -j2 -f Makefile CONF=Release
	cd ../lib/TFHDF5TimeSeries && ${MAKE} -j2 -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../lib/TFBitsNPieces && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFIQFeed && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFInteractiveBrokers && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFOptions && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFVuTrading && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFTrading && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/OUCharting && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/OUSqlite && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/OUSQL && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/OUCommon && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFTimeSeries && ${MAKE} -j2 -f Makefile CONF=Release clean
	cd ../lib/TFHDF5TimeSeries && ${MAKE} -j2 -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc