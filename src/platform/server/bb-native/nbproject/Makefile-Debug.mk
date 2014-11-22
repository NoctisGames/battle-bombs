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
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/GameListener.o \
	${OBJECTDIR}/src/ServerGameSession.o \
	${OBJECTDIR}/src/bb_room_adaptor.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -shared
CXXFLAGS=-m64 -shared

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk dist/libBbNative.dll

dist/libBbNative.dll: ${OBJECTFILES}
	${MKDIR} -p dist
	${LINK.cc} -o dist/libBbNative.dll ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/src/GameListener.o: src/GameListener.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/GameListener.o src/GameListener.cpp

${OBJECTDIR}/src/ServerGameSession.o: src/ServerGameSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerGameSession.o src/ServerGameSession.cpp

${OBJECTDIR}/src/bb_room_adaptor.o: src/bb_room_adaptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bb_room_adaptor.o src/bb_room_adaptor.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} dist/libBbNative.dll

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
