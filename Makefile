# Project name
EXEC=TryEngine

# Compiler
LIBPATH=../../_Libraries
IDIR=include include/TryEngine $(LIBPATH)/SFML/include $(LIBPATH)/Box2D/include $(LIBPATH)/DiscordRPC/include $(LIBPATH)/JSONcpp/include
IDIRFLAG=$(foreach idir, $(IDIR), -I$(idir))
CXXFLAGS= -std=c++0x -std=c++11 -std=c++14 -std=c++17 $(IDIRFLAG)
# Linker
LFLAGS=$(IDIRFLAG) -L$(LIBPATH)/SFML/lib/ -L$(LIBPATH)/Box2D/lib/ -L$(LIBPATH)/DiscordRPC/lib/ -L$(LIBPATH)/JSONcpp/lib/
LIBS= -lopengl32 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -lbox2d -ldiscord-rpc -ljsoncpp
# Directories
SRCTRYENGINEMODULES= CoreObject System Window 
SRCTRYENGINE= $(foreach sdir, $(SRCTRYENGINEMODULES), TryEngine/$(sdir))
SRCMODULES= $(SRCTRYENGINE) Modules Components Systems
SRCDIR= src $(foreach sdir, $(SRCMODULES), src/$(sdir)) 
#src src/Utilities src/TryEngine/CoreObject
OBJDIR=obj
BINDIR=bin
RELEASEDIR=Release
DEBUGDIR=Debug

# Files
SOURCES=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp))
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

# For rm
SOURCESTILDE=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp~))
INCLUDESTILDE=$(foreach idir, $(IDIR), $(wildcard $(idir)/*.hpp~))

vpath %.cpp $(SRCDIR)

# Reminder, 'cause it is easy to forget makefile's fucked-up syntax...
# $@ is what triggered the rule, ie the target before :
# $^ is the whole dependencies list, ie everything after :
# $< is the first item in the dependencies list

# Rules
gcc: $(info $(SRCDIR))
gcc: clean
gcc: CXX=g++
gcc: LINKER=g++ -o
gcc: CXXFLAGS += -DNDEBUG
gcc: $(BINDIR)/$(RELEASEDIR)/$(EXEC)

gcc-debug: clean-debug
gcc-debug: CXX=g++
gcc-debug: LINKER=g++ -o
gcc-debug: CXXFLAGS += -g
gcc-debug: $(BINDIR)/$(DEBUGDIR)/$(EXEC)

clang: clean
clang: CXX=clang++
clang: LINKER=clang++ -o
clang: CXXFLAGS += -DNDEBUG -stdlib=libc++
clang: $(BINDIR)/$(RELEASEDIR)/$(EXEC)

clang-debug: clean-debug
clang-debug: CXX=clang++
clang-debug: LINKER=clang++ -o
clang-debug: CXXFLAGS += -g -stdlib=libc++
clang-debug: $(BINDIR)/$(DEBUGDIR)/$(EXEC)

$(BINDIR)/$(RELEASEDIR)/$(EXEC): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $^ $(LIBS)

$(BINDIR)/$(DEBUGDIR)/$(EXEC): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $^ $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: gcc gcc-debug clang clang-debug clean 

clean-all:
	rm -fr core *~ $(OBJECTS) $(BINDIR)/$(DEBUGDIR)/$(EXEC) $(BINDIR)/$(RELEASEDIR)/$(EXEC) $(SOURCESTILDE) $(INCLUDESTILDE)

clean:
	rm -fr core *~ $(OBJECTS) $(BINDIR)/$(RELEASEDIR)/$(EXEC) $(SOURCESTILDE) $(INCLUDESTILDE)

clean-debug:
	rm -fr core *~ $(OBJECTS) $(BINDIR)/$(DEBUGDIR)/$(EXEC) $(SOURCESTILDE) $(INCLUDESTILDE)