; ModuleID = 'fux compiler'
source_filename = "fux compiler"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-darwin22.3.0"

@literal = private unnamed_addr constant [14 x i8] c"Hello World!\0A\00", align 1

define void @say() {
entry:
  %0 = call i64 @puts(ptr @literal)
  ret void
}

declare i64 @puts(ptr nocapture)

define i64 @main(i64 %argc, ptr %argv) {
locals:
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  %c = alloca i64, align 8
  store i64 1, ptr %a, align 8
  %a_lhs_load = load i64, ptr %a, align 8
  store i64 2, ptr %b, align 8
  %b_rhs_load = load i64, ptr %b, align 8
  %addtmp = add i64 %a_lhs_load, %b_rhs_load
  %d = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %locals
  store i64 %addtmp, ptr %c, align 8
  %c_load = load i64, ptr %c, align 8
  store i64 10, ptr %d, align 8
  %d_load = load i64, ptr %d, align 8
  store i64 %d_load, ptr %c, align 8
  store i64 %c_load, ptr %d, align 8
  call void @say()
  %c_lhs_load = load i64, ptr %c, align 8
  %addtmp1 = add i64 %c_lhs_load, 1
  ret i64 %addtmp1
}
