# Compiler & Debugger
CC = clang
PIP = pip3

# Input, Output, Testing files
IN = main.c
OUT = passwd-man
CRYPTO_MOD_BASE = src/crypto_mod
PY_REQ_LST = requirements.txt

# Build arguments
LIBRARIES = 
NORMAL_BUILD = -O1
MEM_ANALYSIS_BUILD = -fsanitize=address -O1 -fno-omit-frame-pointer
STEPPER_BUILD = -g
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

# Targets
build: $(IN)
	$(CC) $(LIBRARIES) $(NORMAL_BUILD) $(IN) -o $(OUT)

setup: $(PY_REQ_LST)
	$(PIP) install -r $(PY_REQ_LST)
	alias passwd-man=$(ROOT_DIR)/passwd-man

build-sanitary: $(IN)
	$(CC) $(LIBRARIES) $(MEM_ANALYSIS_BUILD) $(IN) -o $(OUT)

build-stepper: $(IN)
	$(CC) $(LIBRARIES) $(STEPPER_BUILD) $(IN) -o $(OUT)

stepper: $(DEBUGGER) $(OUT)
	lldb ./$(OUT)

clean-debug:
	rm $(OUT)
	rm -rf $(OUT).dSYM
