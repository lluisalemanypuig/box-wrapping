
# |-----------|
# | VARIABLES |
# |-----------|

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
UTILS_DIR = $(SRC_DIR)/utils
HEUR_DIR = $(SRC_DIR)/heuristics
SOLV_DIR = $(SRC_DIR)/solvers

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

PARSE_DPS		= $(UTILS_DIR)/parse_string.hpp
LOGGER_DPS		= $(UTILS_DIR)/logger.hpp
TIME_DPS		= $(UTILS_DIR)/time.hpp
DEFINITIONS_DPS	= $(UTILS_DIR)/definitions.hpp
WRAPPED_BOXES	= $(UTILS_DIR)/wrapped_boxes.hpp
INPUT_DATA_DPS	= $(UTILS_DIR)/input_data.hpp $(DEFINITIONS_DPS)

BOX_WRAPPER_SIMPLE_DPS = $(SOLV_DIR)/box_wrapper_simple.hpp \
	$(INPUT_DATA_DPS) $(DEFINITIONS_DPS) $(LOGGER_DPS) $(WRAPPED_BOXES)

BOX_WRAPPER_ROTATE_DPS = $(SOLV_DIR)/box_wrapper_rotate.hpp \
	$(INPUT_DATA_DPS) $(DEFINITIONS_DPS) $(LOGGER_DPS) $(WRAPPED_BOXES)

BOX_WRAPPER_OPTIM_DPS = $(SOLV_DIR)/box_wrapper_optim.hpp \
	$(BOX_WRAPPER_ROTATE_DPS) $(INPUT_DATA_DPS) $(DEFINITIONS_DPS) \
	$(LOGGER_DPS) $(WRAPPED_BOXES)

HEURISTICS_DPS = $(HEUR_DIR)/heuristics.hpp $(INPUT_DATA_DPS) \
	$(WRAPPED_BOXES) $(BOX_WRAPPER_OPTIM_DPS)

MAIN_DPS = $(INPUT_DATA_DPS) $(WRAPPED_BOXES) $(PARSE_DPS) $(TIME_DPS) \
	$(BOX_WRAPPER_SIMPLE_DPS) \
	$(BOX_WRAPPER_ROTATE_DPS) \
	$(BOX_WRAPPER_OPTIM_DPS) \
	$(HEURISTICS_DPS)

# |-------|
# | RULES |
# |-------|

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/$(EXE_NAME)
release: releasedirectory $(REL_BIN_DIR)/$(EXE_NAME)

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
