#---- Outputs ----#
COMPONENT_NAME = protos
#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Inputs ----#
PROJECT_HOME_DIR = .
ifeq "$(CPPUTEST_HOME)" ""
	CPPUTEST_HOME = ~/src/c/cpputest
endif
CPP_PLATFORM = Gcc
CPPUTEST_USE_EXTENSIONS = Y

SRC_DIRS = \
	src\

# to pick specific files (rather than directories) use this:
SRC_FILES =

TEST_SRC_DIRS = \
	tests \

MOCKS_SRC_DIRS = \
	mocks \

INCLUDE_DIRS =\
	./include/ \
	$(CPPUTEST_HOME)/include/ \
	$(CPPUTEST_HOME)/include/Platforms/Gcc\
	mocks

CPPUTEST_WARNINGFLAGS = -Wall -Werror -Wswitch-default
CPPUTEST_WARNINGFLAGS += -Wconversion -Wswitch-enum
CPPUTEST_CPPFLAGS += -DCPPUTESTING=1

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
