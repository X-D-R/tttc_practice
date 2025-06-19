; ModuleID = 'tmp/a.ll'
source_filename = "tmp/a.ll"

define i32 @main() {
  %result = call i32 @test(i32 5)
  ret i32 %result
}

define i32 @test(i32 %a) {
  %mul.shift = shl i32 %a, 2
  ret i32 %mul.shift
}
