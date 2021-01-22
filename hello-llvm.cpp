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
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: <prog> <IR file>\n";
    return 1;
  }
  // Parse an LLVM IR file
  llvm::SMDiagnostic Diag;
  std::unique_ptr<llvm::LLVMContext> Ctx(new llvm::LLVMContext);
  std::unique_ptr<llvm::Module> M = llvm::parseIRFile(argv[1], Diag, *Ctx);
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
          llvm::outs() << "found alloca instruction, allocating: ";
          Alloca->getAllocatedType()->print(llvm::outs());
          llvm::outs() << '\n';
        }
        if (const auto *Load = llvm::dyn_cast<llvm::LoadInst>(&I)) {
          llvm::outs() << "found load instruction, loading from: ";
          Load->getPointerOperand()->print(llvm::outs());
          llvm::outs() << '\n';
        }
        if (llvm::isa<llvm::CallInst>(&I) || llvm::isa<llvm::InvokeInst>(&I)) {
          llvm::ImmutableCallSite CS(&I);
          if (!CS.isIndirectCall()) {
            const auto *Callee = CS.getCalledFunction();
            if (Callee) {
              llvm::outs() << "found direct call to " << Callee->getName()
                           << '\n';
            }
          } else {
            llvm::outs() << "found indirect call to ";
            CS.getCalledValue()->print(llvm::outs());
            llvm::outs() << '\n';
          }
        }
      }
    }
  }
  return 0;
}
