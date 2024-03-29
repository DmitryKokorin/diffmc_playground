# Makefile
TARGET=$(shell basename `pwd`)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)


#debug
CPPFLAGS +=-D__DEBUG__ -g

#profiling
CPPFLAGS += -g
LDFLAGS  += -g

#arch
#CPPFLAGS +=-march=native -O3 -ffast-math -pipe
CPPFLAGS += -march=native -O3

#warnings
CPPFLAGS +=-W -Wall -Weffc++ -Werror -pedantic

#CPPFLAGS    +=-DEXPERIMENTAL=1

#openmp
CPPFLAGS +=-fopenmp
LDFLAGS  +=-fopenmp


all: $(TARGET)

$(OBJECTS): $(SOURCES)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJECTS) $(LOADLIBES) $(LDLIBS)
  
clean:
	$(RM) $(OBJECTS) $(TARGET)
  
.PHONY: all clean
