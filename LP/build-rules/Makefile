
# |-----------|
# | VARIABLES |
# |-----------|

PROJ_DIR	= ..
SRC_DIR		= $(PROJ_DIR)/src
UTILS_DIR	= $(SRC_DIR)/utils
HEUR_DIR	= $(SRC_DIR)/heuristics
SOLV_DIR	= $(SRC_DIR)/solvers

RULES_DIR	= $(PROJ_DIR)/build-rules
DEB_BIN_DIR = $(PROJ_DIR)/build-debug
REL_BIN_DIR = $(PROJ_DIR)/build-release

include $(RULES_DIR)/Makefile.ibm

LIBS =	-L$(CPLEX_LIB_DIR) -lilocplex -lcplex \
		-L$(CONCERT_LIB_DIR) -lconcert \
		-lm -pthread

INCLUDE = $(SRC_DIR) -I$(CPLEX_INC_DIR) -I$(CONCERT_INC_DIR) 

BASE_FLAGS = $(IBM_FLAGS) -std=c++11 -Wextra -Wall

EXE_NAME = wrapping-boxes

# |--------------|
# | DEPENDENCIES |
# |--------------|

include $(RULES_DIR)/Makefile.deps

# |-------|
# | RULES |
# |-------|

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/$(EXE_NAME)
release: releasedirectory $(REL_BIN_DIR)/$(EXE_NAME)

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
