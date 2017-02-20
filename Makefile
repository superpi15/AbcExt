
CC   := gcc
CXX  := g++
LD   := $(CXX)

MSG_PREFIX ?=

$(info $(MSG_PREFIX)Using CC=$(CC))
$(info $(MSG_PREFIX)Using CXX=$(CXX))
$(info $(MSG_PREFIX)Using LD=$(LD))

PROG := abcExt

MODULES :=  src/base/abc src/base/io	src/base/ver src/aig/hop

all: lib$(PROG).a
default: $(PROG)

ARCHFLAGS_EXE ?= ./arch_flags

$(ARCHFLAGS_EXE) : arch_flags.c
	$(CC) arch_flags.c -o $(ARCHFLAGS_EXE)

INCLUDES += -I../abc/src -Isrc

ARCHFLAGS ?= $(shell $(CC) arch_flags.c -o $(ARCHFLAGS_EXE) && $(ARCHFLAGS_EXE))
ARCHFLAGS := $(ARCHFLAGS)

OPTFLAGS  ?= -g -O

CFLAGS    += -Wall -Wno-unused-function -Wno-write-strings -Wno-sign-compare $(ARCHFLAGS)
ifneq ($(findstring arm,$(shell uname -m)),)
	CFLAGS += -DABC_MEMALIGN=4
endif

# compile ABC using the C++ comipler and put everything in the namespace $(ABC_NAMESPACE)
ifdef ABC_USE_NAMESPACE
  CFLAGS += -DABC_NAMESPACE=$(ABC_USE_NAMESPACE) -fpermissive
  CC := $(CXX)
  $(info $(MSG_PREFIX)Compiling in namespace $(ABC_NAMESPACE))
endif

# compile CUDD with ABC
ifndef ABC_USE_NO_CUDD
  CFLAGS += -DABC_USE_CUDD=1
  #MODULES += src/bdd/cudd src/bdd/extrab src/bdd/dsd src/bdd/epd src/bdd/mtr src/bdd/reo src/bdd/cas src/bdd/bbr src/bdd/llb
  $(info $(MSG_PREFIX)Compiling with CUDD)
endif

ABC_READLINE_INCLUDES ?=
ABC_READLINE_LIBRARIES ?= -lreadline

# whether to use libreadline
ifndef ABC_USE_NO_READLINE
  CFLAGS += -DABC_USE_READLINE $(ABC_READLINE_INCLUDES)
  LIBS += $(ABC_READLINE_LIBRARIES)
  $(info $(MSG_PREFIX)Using libreadline)
endif

# whether to compile with thread support
ifndef ABC_USE_NO_PTHREADS
  CFLAGS += -DABC_USE_PTHREADS
  LIBS += -lpthread
  $(info $(MSG_PREFIX)Using pthreads)
endif

# whether to compile into position independent code
ifdef ABC_USE_PIC
  CFLAGS += -fPIC
  LIBS += -fPIC
  $(info $(MSG_PREFIX)Compiling position independent code)
endif

# whether to echo commands while building
ifdef ABC_MAKE_VERBOSE
  VERBOSE=
else
  VERBOSE=@
endif

# Set -Wno-unused-bug-set-variable for GCC 4.6.0 and greater only
ifneq ($(or $(findstring gcc,$(CC)),$(findstring g++,$(CC))),)
empty:=
space:=$(empty) $(empty)

GCC_VERSION=$(shell $(CC) -dumpversion)
GCC_MAJOR=$(word 1,$(subst .,$(space),$(GCC_VERSION)))
GCC_MINOR=$(word 2,$(subst .,$(space),$(GCC_VERSION)))

$(info $(MSG_PREFIX)Found GCC_VERSION $(GCC_VERSION))
ifeq ($(findstring $(GCC_MAJOR),0 1 2 3),)
$(info $(MSG_PREFIX)Found GCC_MAJOR>=4)
ifeq ($(findstring $(GCC_MINOR),0 1 2 3 4 5),)
$(info $(MSG_PREFIX)Found GCC_MINOR>=6)
CFLAGS += -Wno-unused-but-set-variable
endif
endif

endif

# LIBS := -ldl -lrt
LIBS += -ldl -lm
ifneq ($(findstring Darwin, $(shell uname)), Darwin)
   LIBS += -lrt
endif


$(info $(MSG_PREFIX)Using CFLAGS=$(CFLAGS))
CXXFLAGS += $(CFLAGS)

SRC  :=
GARBAGE := core core.* *.stackdump ./tags $(PROG) arch_flags

.PHONY: all default tags clean docs cmake_info

include $(patsubst %, %/module.make, $(MODULES))

OBJ := \
	$(patsubst %.cc, %.o, $(filter %.cc, $(SRC))) \
	$(patsubst %.cpp, %.o, $(filter %.cpp, $(SRC))) \
	$(patsubst %.c, %.o,  $(filter %.c, $(SRC)))  \
	$(patsubst %.y, %.o,  $(filter %.y, $(SRC)))

DEP := $(OBJ:.o=.d)

# implicit rules

%.o: %.c
	@echo "$(MSG_PREFIX)\`\` Compiling:" $(LOCAL_PATH)/$<
	$(VERBOSE)$(CC) -c $(OPTFLAGS) $(INCLUDES) $(CFLAGS) $< -o $@

%.o: %.cc
	@echo "$(MSG_PREFIX)\`\` Compiling:" $(LOCAL_PATH)/$<
	$(VERBOSE)$(CXX) -c $(OPTFLAGS) $(INCLUDES) $(CXXFLAGS) $< -o $@

%.o: %.cpp
	@echo "$(MSG_PREFIX)\`\` Compiling:" $(LOCAL_PATH)/$<
	$(VERBOSE)$(CXX) -c $(OPTFLAGS) $(INCLUDES) $(CXXFLAGS) $< -o $@

%.d: %.c
	@echo "$(MSG_PREFIX)\`\` Generating dependency:" $(LOCAL_PATH)/$<
	$(VERBOSE)./depends.sh $(CC) `dirname $*.c` $(OPTFLAGS) $(INCLUDES) $(CFLAGS) $*.c > $@

%.d: %.cc
	@echo "$(MSG_PREFIX)\`\` Generating dependency:" $(LOCAL_PATH)/$<
	$(VERBOSE)./depends.sh $(CXX) `dirname $*.cc` $(OPTFLAGS) $(INCLUDES) $(CXXFLAGS) $*.cc > $@

%.d: %.cpp
	@echo "$(MSG_PREFIX)\`\` Generating dependency:" $(LOCAL_PATH)/$<
	$(VERBOSE)./depends.sh $(CXX) `dirname $*.cpp` $(OPTFLAGS) $(INCLUDES) $(CXXFLAGS) $*.cpp > $@

ifndef ABC_MAKE_NO_DEPS
-include $(DEP)
endif

# Actual targets

depend: $(DEP)

clean:
	@echo "$(MSG_PREFIX)\`\` Cleaning up..."
	$(VERBOSE)rm -rvf $(PROG) lib$(PROG).a $(OBJ) $(GARBAGE) $(OBJ:.o=.d)

tags:
	etags `find . -type f -regex '.*\.\(c\|h\)'`

$(PROG): $(OBJ)
	@echo "$(MSG_PREFIX)\`\` Building binary:" $(notdir $@)
	$(VERBOSE)$(LD) -o $@ $^ $(LIBS)

lib$(PROG).a: $(OBJ)
	@echo "$(MSG_PREFIX)\`\` Linking:" $(notdir $@)
	$(VERBOSE)ar rv $@ $?
	$(VERBOSE)ranlib $@

docs:
	@echo "$(MSG_PREFIX)\`\` Building documentation." $(notdir $@)
	$(VERBOSE)doxygen doxygen.conf

cmake_info:
	@echo SEPARATOR_CFLAGS $(CFLAGS) SEPARATOR_CFLAGS
	@echo SEPARATOR_CXXFLAGS $(CXXFLAGS) SEPARATOR_CXXFLAGS
	@echo SEPARATOR_LIBS $(LIBS) SEPARATOR_LIBS
	@echo SEPARATOR_SRC $(SRC) SEPARATOR_SRC
