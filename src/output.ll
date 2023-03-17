; ModuleID = 'fux compiler'
source_filename = "fux compiler"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-darwin22.3.0"

define i64 @main(i64 %argc, ptr %argv) {
entry:
  ret i64 12
}
