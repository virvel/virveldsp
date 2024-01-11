# Project Name
TARGET = MPR121

# Sources
CPP_SOURCES = MPR121.cpp

# Library Locations
LIBDAISY_DIR = ./libDaisy/
DAISYSP_DIR = ./DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

CPPFLAGS += -I./virveldsp/src
