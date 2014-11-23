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
	${OBJECTDIR}/_ext/361124323/Circle.o \
	${OBJECTDIR}/_ext/361124323/DynamicGameObject.o \
	${OBJECTDIR}/_ext/361124323/DynamicGridGameObject.o \
	${OBJECTDIR}/_ext/361124323/GameObject.o \
	${OBJECTDIR}/_ext/361124323/GridGameObject.o \
	${OBJECTDIR}/_ext/361124323/Line.o \
	${OBJECTDIR}/_ext/361124323/OverlapTester.o \
	${OBJECTDIR}/_ext/361124323/Rectangle.o \
	${OBJECTDIR}/_ext/361124323/Triangle.o \
	${OBJECTDIR}/_ext/361124323/Vector2D.o \
	${OBJECTDIR}/_ext/1528379507/BombGameObject.o \
	${OBJECTDIR}/_ext/1528379507/BotPlayerDynamicGameObject.o \
	${OBJECTDIR}/_ext/1528379507/BreakableBlock.o \
	${OBJECTDIR}/_ext/1528379507/Crater.o \
	${OBJECTDIR}/_ext/1528379507/Explosion.o \
	${OBJECTDIR}/_ext/1528379507/FallingObjectShadow.o \
	${OBJECTDIR}/_ext/1528379507/Fire.o \
	${OBJECTDIR}/_ext/1528379507/FireBall.o \
	${OBJECTDIR}/_ext/1528379507/GameSession.o \
	${OBJECTDIR}/_ext/1528379507/IceBall.o \
	${OBJECTDIR}/_ext/1528379507/IcePatch.o \
	${OBJECTDIR}/_ext/1528379507/InsideBlock.o \
	${OBJECTDIR}/_ext/1528379507/MapBorder.o \
	${OBJECTDIR}/_ext/1528379507/MapSearchNode.o \
	${OBJECTDIR}/_ext/1528379507/PathFinder.o \
	${OBJECTDIR}/_ext/1528379507/PlayerDynamicGameObject.o \
	${OBJECTDIR}/_ext/1528379507/PowerUp.o \
	${OBJECTDIR}/_ext/1528379507/SpaceTile.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk dist/libBpNative.dll

dist/libBpNative.dll: ${OBJECTFILES}
	${MKDIR} -p dist
	${LINK.cc} -o dist/libBpNative.dll ${OBJECTFILES} ${LDLIBSOPTIONS} -shared

${OBJECTDIR}/_ext/361124323/Circle.o: ../../../core/framework/Circle.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/Circle.o ../../../core/framework/Circle.cpp

${OBJECTDIR}/_ext/361124323/DynamicGameObject.o: ../../../core/framework/DynamicGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/DynamicGameObject.o ../../../core/framework/DynamicGameObject.cpp

${OBJECTDIR}/_ext/361124323/DynamicGridGameObject.o: ../../../core/framework/DynamicGridGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/DynamicGridGameObject.o ../../../core/framework/DynamicGridGameObject.cpp

${OBJECTDIR}/_ext/361124323/GameObject.o: ../../../core/framework/GameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/GameObject.o ../../../core/framework/GameObject.cpp

${OBJECTDIR}/_ext/361124323/GridGameObject.o: ../../../core/framework/GridGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/GridGameObject.o ../../../core/framework/GridGameObject.cpp

${OBJECTDIR}/_ext/361124323/Line.o: ../../../core/framework/Line.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/Line.o ../../../core/framework/Line.cpp

${OBJECTDIR}/_ext/361124323/OverlapTester.o: ../../../core/framework/OverlapTester.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/OverlapTester.o ../../../core/framework/OverlapTester.cpp

${OBJECTDIR}/_ext/361124323/Rectangle.o: ../../../core/framework/Rectangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/Rectangle.o ../../../core/framework/Rectangle.cpp

${OBJECTDIR}/_ext/361124323/Triangle.o: ../../../core/framework/Triangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/Triangle.o ../../../core/framework/Triangle.cpp

${OBJECTDIR}/_ext/361124323/Vector2D.o: ../../../core/framework/Vector2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/361124323
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/361124323/Vector2D.o ../../../core/framework/Vector2D.cpp

${OBJECTDIR}/_ext/1528379507/BombGameObject.o: ../../../core/game/BombGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/BombGameObject.o ../../../core/game/BombGameObject.cpp

${OBJECTDIR}/_ext/1528379507/BotPlayerDynamicGameObject.o: ../../../core/game/BotPlayerDynamicGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/BotPlayerDynamicGameObject.o ../../../core/game/BotPlayerDynamicGameObject.cpp

${OBJECTDIR}/_ext/1528379507/BreakableBlock.o: ../../../core/game/BreakableBlock.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/BreakableBlock.o ../../../core/game/BreakableBlock.cpp

${OBJECTDIR}/_ext/1528379507/Crater.o: ../../../core/game/Crater.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/Crater.o ../../../core/game/Crater.cpp

${OBJECTDIR}/_ext/1528379507/Explosion.o: ../../../core/game/Explosion.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/Explosion.o ../../../core/game/Explosion.cpp

${OBJECTDIR}/_ext/1528379507/FallingObjectShadow.o: ../../../core/game/FallingObjectShadow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/FallingObjectShadow.o ../../../core/game/FallingObjectShadow.cpp

${OBJECTDIR}/_ext/1528379507/Fire.o: ../../../core/game/Fire.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/Fire.o ../../../core/game/Fire.cpp

${OBJECTDIR}/_ext/1528379507/FireBall.o: ../../../core/game/FireBall.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/FireBall.o ../../../core/game/FireBall.cpp

${OBJECTDIR}/_ext/1528379507/GameSession.o: ../../../core/game/GameSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/GameSession.o ../../../core/game/GameSession.cpp

${OBJECTDIR}/_ext/1528379507/IceBall.o: ../../../core/game/IceBall.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/IceBall.o ../../../core/game/IceBall.cpp

${OBJECTDIR}/_ext/1528379507/IcePatch.o: ../../../core/game/IcePatch.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/IcePatch.o ../../../core/game/IcePatch.cpp

${OBJECTDIR}/_ext/1528379507/InsideBlock.o: ../../../core/game/InsideBlock.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/InsideBlock.o ../../../core/game/InsideBlock.cpp

${OBJECTDIR}/_ext/1528379507/MapBorder.o: ../../../core/game/MapBorder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/MapBorder.o ../../../core/game/MapBorder.cpp

${OBJECTDIR}/_ext/1528379507/MapSearchNode.o: ../../../core/game/MapSearchNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/MapSearchNode.o ../../../core/game/MapSearchNode.cpp

${OBJECTDIR}/_ext/1528379507/PathFinder.o: ../../../core/game/PathFinder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/PathFinder.o ../../../core/game/PathFinder.cpp

${OBJECTDIR}/_ext/1528379507/PlayerDynamicGameObject.o: ../../../core/game/PlayerDynamicGameObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/PlayerDynamicGameObject.o ../../../core/game/PlayerDynamicGameObject.cpp

${OBJECTDIR}/_ext/1528379507/PowerUp.o: ../../../core/game/PowerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/PowerUp.o ../../../core/game/PowerUp.cpp

${OBJECTDIR}/_ext/1528379507/SpaceTile.o: ../../../core/game/SpaceTile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1528379507
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1528379507/SpaceTile.o ../../../core/game/SpaceTile.cpp

${OBJECTDIR}/src/GameListener.o: src/GameListener.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/GameListener.o src/GameListener.cpp

${OBJECTDIR}/src/ServerGameSession.o: src/ServerGameSession.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ServerGameSession.o src/ServerGameSession.cpp

${OBJECTDIR}/src/bb_room_adaptor.o: src/bb_room_adaptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DTECHNE_GAMES_SERVER_WINDOWS -Ijdkinclude/ -Ijdkinclude/win32/ -I../../../core/framework/ -I../../../core/game/ -I../../../3rdparty -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bb_room_adaptor.o src/bb_room_adaptor.cpp

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
