
# |-----------|
# | VARIABLES |
# |-----------|

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
RULES_DIR = $(PROJ_DIR)/build-rules
DEB_BIN_DIR = $(PROJ_DIR)/bin-debug
REL_BIN_DIR = $(PROJ_DIR)/bin-release

LIBS =	-lgecodeflatzinc -lgecodedriver \
		-lgecodegist -lgecodesearch		\
		-lgecodeminimodel -lgecodeset	\
		-lgecodefloat -lgecodeint		\
		-lgecodekernel -lgecodesupport

INCLUDE = $(SRC_DIR)

BASE_FLAGS = -std=c++11 -Wextra -Wall

EXE_NAME = wrapping-boxes

# |--------------|
# | DEPENDENCIES |
# |--------------|

MAIN_DPS =

# |-------|
# | RULES |
# |-------|

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/$(EXE_NAME)
release: releasedirectory $(REL_BIN_DIR)/$(EXE_NAME)

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
