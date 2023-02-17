; ModuleID = 'fux compiler'
source_filename = "fux compiler"

@literal_ = private unnamed_addr constant [14 x i8] c"Hello World!\0A\00", align 1

declare i64 @puts(ptr nocapture)

define i64 @main() {
entry:
  %0 = call i64 @puts(ptr @literal_)
  ret i64 0
}
