; ModuleID = 'fux compiler'
source_filename = "fux compiler"

define i64 @main() {
entry:
  %a = alloca i64, align 8
  store i64 1, ptr %a, align 4
  %b = alloca i64, align 8
  store i64 2, ptr %b, align 4
  %c = alloca i64, align 8
  %a_LOAD_ = load i64, ptr %a, align 4
  %b_LOAD_ = load i64, ptr %b, align 4
  %addtmp = add i64 %a_LOAD_, %b_LOAD_
  store i64 %addtmp, ptr %c, align 4
  %d = alloca i64, align 8
  store i64 10, ptr %d, align 4
  %c_LOAD_ = load i64, ptr %c, align 4
  %d_LOAD_ = load i64, ptr %d, align 4
  store i64 %d_LOAD_, ptr %c, align 4
  store i64 %c_LOAD_, ptr %d, align 4
  %c_LOAD_1 = load i64, ptr %c, align 4
  ret i64 %c_LOAD_1
}
