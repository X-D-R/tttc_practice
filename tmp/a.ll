; RUN: opt -passes=mult-pass -S %s | FileCheck %s

define i32 @main() {
  %result = call i32 @test(i32 5)
  ret i32 %result
}

define i32 @test(i32 %a) {
  %mul = mul i32 4, %a
  ret i32 %mul
}
