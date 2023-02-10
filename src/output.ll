; ModuleID = 'fux compiler'
source_filename = "fux compiler"

define i64 @main() {
entry:
  %x = alloca i64, align 8
  store i64 2, ptr %x, align 4
  %0 = zext ptr %x to i64
  ret i64 %0
}
