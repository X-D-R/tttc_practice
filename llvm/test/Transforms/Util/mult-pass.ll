; RUN: opt -passes=mult-pass -S %s | FileCheck %s

define i32 @test(i32 %a) {
; CHECK-LABEL: @test
  ; CHECK: shl i32 %a, 3
  %mul1 = mul i32 %a, 8
  
  ; CHECK: shl i32 %a, 2
  %mul2 = mul i32 4, %a
  
  ; CHECK: mul i32 %a, 3
  %mul3 = mul i32 %a, 3
  
  ; CHECK: shl i32 %a, 0
  %mul4 = mul i32 %a, 1
  
  ; CHECK: mul i32 %a, 0
  %mul5 = mul i32 %a, 0
  
  ret i32 0
}