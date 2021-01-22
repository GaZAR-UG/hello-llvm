#/===----------------------------------------------------------------------===//
#/
#/ Philipp Schubert
#/
#/    Copyright (c) 2021
#/    GaZAR UG (haftungsbeschränkt)
#/    Bielefeld, Germany
#/    philipp@gazar.eu
#/
#/===----------------------------------------------------------------------===//

EXE := hello-llvm
MAIN_FILE := hello-llvm.cpp

CXX := clang++
CXX_FLAGS := -std=c++17
CXX_FLAGS += -Wall
CXX_FLAGS += -Wextra
CXX_FLAGS += -O0
CXX_FLAGS += -g

LLVM_FLAGS := `llvm-config --cppflags --ldflags`
LLVM_LIBS := `llvm-config --system-libs --libs all`

.PHONY: clean

all: target
	$(CXX) $(CXX_FLAGS) $(LLVM_FLAGS) $(MAIN_FILE) -o $(EXE) $(LLVM_LIBS) 

target:
	clang++ -std=c++17 -Wall -Wextra -emit-llvm -S -fno-discard-value-names target-program.cpp

clean:
	rm -f $(EXE)
	rm -f target-program.ll
