
# |-----------|
# | VARIABLES |
# |-----------|

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
UTILS_DIR = $(SRC_DIR)/utils
SOLVER_DIR = $(SRC_DIR)/utils

RULES_DIR = $(PROJ_DIR)/build-rules
DEB_BIN_DIR = $(PROJ_DIR)/build-debug
REL_BIN_DIR = $(PROJ_DIR)/build-release

LIBS =	-lgmp

INCLUDE = $(SRC_DIR)

BASE_FLAGS = -std=c++11 -Wextra -Wall

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
