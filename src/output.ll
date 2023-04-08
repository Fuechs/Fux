; ModuleID = 'fux compiler'
source_filename = "fux compiler"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-darwin22.3.0"

define i64 @main(i64 %argc, ptr %argv) {
locals:
  %x = alloca double, align 8
  br label %entry

entry:                                            ; preds = %locals
  store double 8.000000e+00, ptr %x, align 8
  %x_load = load double, ptr %x, align 8
  %powasg = call double @pow(double %x_load, double 0.000000e+00)
  store double %powasg, ptr %x, align 8
  %x_load1 = load double, ptr %x, align 8
  %typecast = fptosi double %x_load1 to i64
  ret i64 %typecast
}

; Function Attrs: nounwind readnone
declare double @pow(double, double) #0

attributes #0 = { nounwind readnone }
