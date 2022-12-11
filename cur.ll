; ModuleID = 'fux compiler'
source_filename = "fux compiler"

define i32 @main(i32 %x, i32 %y) {
entry:
  %addtmp = add i32 %x, %y
  ret i32 %addtmp
}