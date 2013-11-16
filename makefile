CPP = g++
#SRC = `lib/*.c *.cpp AI/*.cpp lib/*.cpp`
SRC = CMap.cpp main.cpp Menu.cpp Minimap.cpp Objects.cpp Scene.cpp SceneIO.cpp AI/AI_ants.cpp AI/AI_enemies.cpp AI/AI_environment.cpp AI/AI_graphic_objects.cpp AI/AI_nest.cpp AI/AI_objects.cpp AI/AI_renders.cpp AI/AI_renders_in_menu.cpp AI/AI_ring.cpp AI/AI_shared.cpp AI/AI_technical.cpp AI/AI_world.cpp lib/3dsloader.cpp lib/C3DObject.cpp lib/C3DObjEditor.cpp lib/CBubbleText.cpp lib/CCamera.cpp lib/CMotor.cpp lib/CSDLChrono.cpp lib/CSoundManager.cpp lib/font.cpp lib/Particles.cpp lib/TVector.cpp lib/CustomAssert.c lib/tga.c

OBJS =	$(SRC:.cpp=.o)

EXE =	Debug/fourmi_simul

CPPFLAGS = -g -Wall

LDFLAGS =  -LG:/Programmation/Dev-Cpp/lib -lSDL -lGLU -lGL -lSDL_mixer

RM=del

all: $(EXE)

$(EXE): $(OBJS)
	$(CPP) -o $(EXE) $(OBJS) $(LDFLAGS)

$(OBJS): makefile

.PHONY: clean
clean:
	$(RM) $(OBJS) *~ $(EXE)
