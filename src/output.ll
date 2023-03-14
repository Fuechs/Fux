; ModuleID = 'fux compiler'
source_filename = "fux compiler"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-darwin22.3.0"

@literal = private unnamed_addr constant [14 x i8] c"Hello World!\0A\00", align 1

define i64 @main() {
entry:
  %0 = call i64 @puts(ptr @literal)
  ret i64 0
}

declare i64 @puts(ptr nocapture)
