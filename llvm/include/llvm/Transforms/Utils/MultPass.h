#ifndef LLVM_TRANSFORMS_UTILS_MULTPASS_H
#define LLVM_TRANSFORMS_UTILS_MULTPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {
    class MultPass : public PassInfoMixin<MultPass> {
        public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    };
}

#endif