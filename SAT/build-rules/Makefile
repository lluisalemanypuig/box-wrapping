
# |-----------|
# | VARIABLES |
# |-----------|

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
UTILS_DIR = $(SRC_DIR)/utils
SOLVER_DIR = $(SRC_DIR)/encoder

RULES_DIR = $(PROJ_DIR)/build-rules
DEB_BIN_DIR = $(PROJ_DIR)/build-debug
REL_BIN_DIR = $(PROJ_DIR)/build-release

CLS_GEN_LIBS =
SOL_GEN_LIBS =

INCLUDE = $(SRC_DIR)

BASE_FLAGS = -std=c++11 -Wextra -Wall

CLAUSE_GEN_EXE	= clause-generator
SOL_GEN_EXE		= solution-generator

# |--------------|
# | DEPENDENCIES |
# |--------------|

include $(RULES_DIR)/Makefile.deps

# |-------|
# | RULES |
# |-------|

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/$(CLAUSE_GEN_EXE) $(DEB_BIN_DIR)/$(SOL_GEN_EXE)
release: releasedirectory $(REL_BIN_DIR)/$(CLAUSE_GEN_EXE) $(REL_BIN_DIR)/$(SOL_GEN_EXE)

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
