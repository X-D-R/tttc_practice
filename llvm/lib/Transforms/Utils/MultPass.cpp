#include "llvm/Transforms/Utils/MultPass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PatternMatch.h"

using namespace llvm;
using namespace PatternMatch;

PreservedAnalyses MultPass::run(Function &F, FunctionAnalysisManager &AM) {
    bool Changed = false;
    SmallVector<BinaryOperator*, 8> ToReplace;
    for (auto &BB : F) {
        for (auto &I : BB) {
            auto *BinOp = dyn_cast<BinaryOperator>(&I);
            if (!BinOp || BinOp->getOpcode() != Instruction::Mul)
                continue;
            
            Value *LHS = BinOp->getOperand(0);
            Value *RHS = BinOp->getOperand(1);
            ConstantInt *CI = nullptr;
            Value *OtherOp = nullptr;
            // Проверяем левый операнд
            if (auto *LHSC = dyn_cast<ConstantInt>(LHS)) {
                if (LHSC->getValue().isPowerOf2()) {
                    CI = LHSC;
                    OtherOp = RHS;
                }
            }
            
            // Проверяем правый операнд, если левый не подошел
            if (!CI) {
                if (auto *RHSC = dyn_cast<ConstantInt>(RHS)) {
                    if (RHSC->getValue().isPowerOf2()) {
                        CI = RHSC;
                        OtherOp = LHS;
                    }
                }
            }
            
            if (!CI || !OtherOp)
                continue;
                
            // Сохраняем для последующей замены
            ToReplace.push_back(BinOp);
        }
    }
    
    // Выполняем замену после обхода
    for (auto *BinOp : ToReplace) {
        Value *LHS = BinOp->getOperand(0);
        Value *RHS = BinOp->getOperand(1);
        ConstantInt *CI = cast<ConstantInt>(isa<ConstantInt>(LHS) ? LHS : RHS);
        Value *OtherOp = isa<ConstantInt>(LHS) ? RHS : LHS;
        
        unsigned ShiftAmount = CI->getValue().logBase2();
        IRBuilder<> Builder(BinOp);
        Value *Shl = Builder.CreateShl(
            OtherOp,
            ConstantInt::get(BinOp->getType(), ShiftAmount),
            BinOp->getName() + ".shift");
            
        // Заменяем все использования перед удалением
        BinOp->replaceAllUsesWith(Shl);
        BinOp->eraseFromParent();
        Changed = true;
    }
    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}