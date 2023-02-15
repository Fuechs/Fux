; ModuleID = 'fux compiler'
source_filename = "fux compiler"

define i64 @main() {
entry:
  %a = alloca i64, align 8
  store i64 1, ptr %a, align 4
  %b = alloca i64, align 8
  store i64 2, ptr %b, align 4
  %c = alloca i64, align 8
  %0 = load i64, ptr %a, align 4
  %1 = load i64, ptr %b, align 4
  %addtmp = add i64 %0, %1
  store i64 %addtmp, ptr %c, align 4
  %d = alloca i64, align 8
  store i64 10, ptr %d, align 4
  %2 = load i64, ptr %c, align 4
  %3 = load i64, ptr %d, align 4
  store i64 %3, ptr %c, align 4
  store i64 %2, ptr %d, align 4
  %4 = load i64, ptr %c, align 4
  ret i64 %4
}
