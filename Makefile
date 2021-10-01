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

LLVM_FLAGS := `llvm-config --cppflags --ldflags`
LLVM_LIBS := `llvm-config --system-libs --libs all`

.PHONY: clean

all: target
	clang++ -std=c++17 -Wall -Wextra -O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer $(LLVM_FLAGS) hello-llvm.cpp $(LLVM_LIBS) -o hello-llvm

target:
	clang++ -std=c++17 -Wall -Wextra -emit-llvm -S -fno-discard-value-names target-program.cpp

clean:
	rm -f hello-llvm
	rm -f target-program.ll
	rm -f target-program.bc
