# Compiler & Debugger
CC = clang

# Input, Output, Testing files
IN = main.c
OUT = passwd-man
CRYPTO_MOD_BASE = src/crypto_mod
# TEST = tbd.c

# Build arguments
# LIBRARIES = -lssl -lcrypto
LIBRARIES = 
NORMAL_BUILD = -O1
MEM_ANALYSIS_BUILD = -fsanitize=address -O1 -fno-omit-frame-pointer
STEPPER_BUILD = -g

# Targets
build: $(IN)
	$(CC) $(LIBRARIES) $(NORMAL_BUILD) $(IN) -o $(OUT)

build-sanitary: $(IN)
	$(CC) $(LIBRARIES) $(MEM_ANALYSIS_BUILD) $(IN) -o $(OUT)

build-stepper: $(IN)
	$(CC) $(LIBRARIES) $(STEPPER_BUILD) $(IN) -o $(OUT)

# test-io: $(TEST) $(OUT)
# 	cat $(TEST) | ./$(OUT)

stepper: $(DEBUGGER) $(OUT)
	lldb ./$(OUT)

clean-debug:
	rm $(OUT)
	rm -rf $(OUT).dSYM
