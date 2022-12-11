; ModuleID = 'fux compiler'
source_filename = "fux compiler"

define double @main(double %x, double %y) {
entry:
  %addtmp = fadd double %x, %y
  ret double %addtmp
}