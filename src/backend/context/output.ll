; ModuleID = 'fux compiler'
source_filename = "fux compiler"

%str = type { ptr, i64, i64, i64 }

declare common ptr @malloc(i64)

declare common void @free(ptr)

declare common ptr @memcpy(ptr, ptr, i64)

; Function Attrs: nounwind
define common fastcc void @createDefaultStr(ptr %0) #0 {
entry:
  %1 = getelementptr ptr, ptr %0, i64 0
  store ptr null, ptr %1, align 8
  %2 = getelementptr ptr, ptr %0, i64 1
  store i64 0, ptr %2, align 4
  %3 = getelementptr ptr, ptr %0, i64 2
  store i64 0, ptr %3, align 4
  %4 = getelementptr ptr, ptr %0, i64 3
  store i64 0, ptr %4, align 4
  ret void
}

; Function Attrs: nounwind
define common fastcc void @deleteStr(ptr %0) #0 {
entry:
  %1 = getelementptr ptr, ptr %0, i64 0
  %2 = load ptr, ptr %1, align 8
  %3 = icmp ne ptr %2, null
  br i1 %3, label %free_begin, label %free_close

free_begin:                                       ; preds = %entry
  call void @free(ptr %2)
  br label %free_close

free_close:                                       ; preds = %free_begin, %entry
  ret void
}

declare common void @putch(i8)

declare common void @puts(%str)

declare common %str @read()

attributes #0 = { nounwind }