#!/usr/bin/make

##############################################
# A simple but flexible Makefile for small-to-
#  medium C++ projects.

##############################################
# Note: Header dependencies lag compilation.
#  If you've edited a source file to add a new
#  "include" and suddenly get compiler errors,
#  try compiling again.

##############################################
# Set up the project-specific options
EXEC=convolve
# C++ version ex: -ansi, -std=c++11
CV=
# Libraries
LIBS=
# Special compile flags
SCFLAGS=-pedantic-errors -Wall -Wconversion -MD

##############################################
# Tweak the compiler/debugger/profiler options
# ex: CC=g++
# alternatives: nvcc, clang++
CC=g++

# ------Debug commands------
# ex: DEBUG=-O2		# (make execution faster)
# ex: DEBUG=-ggdb -Og	# (make debugging easier)
DEBUG=-pg
DEBUG_CMDLINE=

# ------Profiling commands------
# ex: PROFILE=-pg -fprofile-arcs -ftest-coverage
PROFILE=
#any needed profiling parameters
PROFILE_CMDLINE=

#############################################
# Set up the source directories 
INCDIR=include
SRCDIR=src
OBJDIR=obj
BINDIR=bin

#############################################
# The rest should only need to be modified if
#  you're not using g++
INCLUDES=-I$(INCDIR)
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
DEPS=$(OBJECTS:.o=.d)
PROFS=$(OBJECTS:.o=.gcda) $(OBJECTS:.o=.gcno)

LDFLAGS=$(DEBUG) $(PROFILE) $(LIBS) $(LIBDIRS)
CFLAGS=$(DEBUG) $(PROFILE) $(CV) -c $(LIBDIRS) $(INCLUDES) $(SCFLAGS)
EXECUTABLE=$(BINDIR)/$(EXEC)

all:	$(EXECUTABLE)
clean:
	rm -f $(OBJECTS) $(DEPS) $(PROFS) $(EXECUTABLE) $(OBJDIR)/gmon.out
run: $(EXECUTABLE)
	$(EXECUTABLE) $(DEBUG_CMDLINE)
debug: $(EXECUTABLE)
	gdb --args $(EXECUTABLE) $(DEBUG_CMDLINE)
profile: $(OBJDIR)/gmon.out
	gprof $(EXECUTABLE) $(OBJDIR)/gmon.out
$(OBJDIR)/gmon.out: $(EXECUTABLE)
	$(EXECUTABLE) $(PROFILE_CMDLINE)
	mv gmon.out $(OBJDIR)

-include $(DEPS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

