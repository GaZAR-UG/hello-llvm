//===----------------------------------------------------------------------===//
//
// Philipp Schubert
//
//    Copyright (c) 2021
//    GaZAR UG (haftungsbeschränkt)
//    Bielefeld, Germany
//    philipp@gazar.eu
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <memory>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

int main(int Argc, char **Argv) {
  if (Argc != 2) {
    std::cout << "usage: <prog> <IR file>\n";
    return 1;
  }
  // Parse an LLVM IR file
  llvm::SMDiagnostic Diag;
  llvm::LLVMContext Ctx;
  std::unique_ptr<llvm::Module> M =
      llvm::parseIRFile(Argv[1], Diag, Ctx); // NOLINT
  // Check if the module is valid
  bool BrokenDbgInfo = false;
  if (llvm::verifyModule(*M, &llvm::errs(), &BrokenDbgInfo)) {
    std::cerr << "error: invalid module\n";
    return 1;
  }
  if (BrokenDbgInfo) {
    std::cerr << "caution: debug info is broken\n";
  }
  // Iterate the IR and analyze the functions, basic blocks or individual
  // instructions.
  for (const auto &F : *M) {
    llvm::outs() << "found function " << F.getName() << '\n';
    for (const auto &BB : F) {
      for (const auto &I : BB) {
        if (const auto *Alloca = llvm::dyn_cast<llvm::AllocaInst>(&I)) {
          llvm::outs() << "found alloca inst:\n";
          Alloca->print(llvm::outs());
          llvm::outs() << '\n';
        }
        if (const auto *CallSite = llvm::dyn_cast<llvm::CallBase>(&I)) {
          if (const auto *Callee = CallSite->getCalledFunction()) {
            llvm::outs() << "found callee: " << Callee->getName() << '\n';
          } else {
            llvm::outs() << "found indirect callsite: ";
            CallSite->print(llvm::outs());
            llvm::outs() << '\n';
          }
        }
      }
    }
  }
  return 0;
}
