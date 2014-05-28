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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/32229614/BombGameObject.o \
	${OBJECTDIR}/_ext/32229614/BreakableBlock.o \
	${OBJECTDIR}/_ext/32229614/DynamicGameObject.o \
	${OBJECTDIR}/_ext/32229614/DynamicGridGameObject.o \
	${OBJECTDIR}/_ext/32229614/Explosion.o \
	${OBJECTDIR}/_ext/32229614/GameObject.o \
	${OBJECTDIR}/_ext/32229614/GameSession.o \
	${OBJECTDIR}/_ext/32229614/GridGameObject.o \
	${OBJECTDIR}/_ext/32229614/InsideBlock.o \
	${OBJECTDIR}/_ext/32229614/Line.o \
	${OBJECTDIR}/_ext/32229614/OverlapTester.o \
	${OBJECTDIR}/_ext/32229614/PlayerDynamicGameObject.o \
	${OBJECTDIR}/_ext/32229614/PowerUp.o \
	${OBJECTDIR}/_ext/32229614/Rectangle.o \
	${OBJECTDIR}/_ext/32229614/Triangle.o \
	${OBJECTDIR}/_ext/32229614/Vector2D.o \
	${OBJECTDIR}/src/BotPlayerDynamicGameObject.o \
	${OBJECTDIR}/src/ServerGameSession.o \
	${OBJECTDIR}/src/SoundListener.o \
	${OBJECTDIR}/src/bp_room_adaptor.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk dist/libBpNative.dll

dist/libBpNative.dll: ${OBJECTFILES}
	${MKDIR} -p dist
	${LINK.cc} -o dist/libBpNative.dll ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/_ext/32229614/BombGameObject.o: ../../../core/BombGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/BombGameObject.o ../../../core/BombGameObject.cpp

${OBJECTDIR}/_ext/32229614/BreakableBlock.o: ../../../core/BreakableBlock.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/BreakableBlock.o ../../../core/BreakableBlock.cpp

${OBJECTDIR}/_ext/32229614/DynamicGameObject.o: ../../../core/DynamicGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/DynamicGameObject.o ../../../core/DynamicGameObject.cpp

${OBJECTDIR}/_ext/32229614/DynamicGridGameObject.o: ../../../core/DynamicGridGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/DynamicGridGameObject.o ../../../core/DynamicGridGameObject.cpp

${OBJECTDIR}/_ext/32229614/Explosion.o: ../../../core/Explosion.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/Explosion.o ../../../core/Explosion.cpp

${OBJECTDIR}/_ext/32229614/GameObject.o: ../../../core/GameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/GameObject.o ../../../core/GameObject.cpp

${OBJECTDIR}/_ext/32229614/GameSession.o: ../../../core/GameSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/GameSession.o ../../../core/GameSession.cpp

${OBJECTDIR}/_ext/32229614/GridGameObject.o: ../../../core/GridGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/GridGameObject.o ../../../core/GridGameObject.cpp

${OBJECTDIR}/_ext/32229614/InsideBlock.o: ../../../core/InsideBlock.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/InsideBlock.o ../../../core/InsideBlock.cpp

${OBJECTDIR}/_ext/32229614/Line.o: ../../../core/Line.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/Line.o ../../../core/Line.cpp

${OBJECTDIR}/_ext/32229614/OverlapTester.o: ../../../core/OverlapTester.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/OverlapTester.o ../../../core/OverlapTester.cpp

${OBJECTDIR}/_ext/32229614/PlayerDynamicGameObject.o: ../../../core/PlayerDynamicGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/PlayerDynamicGameObject.o ../../../core/PlayerDynamicGameObject.cpp

${OBJECTDIR}/_ext/32229614/PowerUp.o: ../../../core/PowerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/PowerUp.o ../../../core/PowerUp.cpp

${OBJECTDIR}/_ext/32229614/Rectangle.o: ../../../core/Rectangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/Rectangle.o ../../../core/Rectangle.cpp

${OBJECTDIR}/_ext/32229614/Triangle.o: ../../../core/Triangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/Triangle.o ../../../core/Triangle.cpp

${OBJECTDIR}/_ext/32229614/Vector2D.o: ../../../core/Vector2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/32229614/Vector2D.o ../../../core/Vector2D.cpp

${OBJECTDIR}/src/BotPlayerDynamicGameObject.o: src/BotPlayerDynamicGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/BotPlayerDynamicGameObject.o src/BotPlayerDynamicGameObject.cpp

${OBJECTDIR}/src/ServerGameSession.o: src/ServerGameSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerGameSession.o src/ServerGameSession.cpp

${OBJECTDIR}/src/SoundListener.o: src/SoundListener.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SoundListener.o src/SoundListener.cpp

${OBJECTDIR}/src/bp_room_adaptor.o: src/bp_room_adaptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include -I../../../../../../../../../Program\ Files/Java/jdk1.7.0_51/include/win32 -I../../../core -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bp_room_adaptor.o src/bp_room_adaptor.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} dist/libBpNative.dll

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
