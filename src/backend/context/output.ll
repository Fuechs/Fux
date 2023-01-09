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
  store i64 16, ptr %4, align 4
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

define common fastcc void @resizeStr(ptr %0, i64 %1) {
entry:
  %output = call ptr @malloc(i64 %1)
  %2 = getelementptr ptr, ptr %0, i64 0
  %buffer = load ptr, ptr %2, align 8
  %3 = getelementptr ptr, ptr %0, i64 1
  %len = load i64, ptr %3, align 4
  %4 = call ptr @memcpy(ptr %output, ptr %buffer, i64 %len)
  call void @free(ptr %buffer)
  store ptr %output, ptr %2, align 8
  %5 = getelementptr ptr, ptr %0, i64 2
  store i64 %1, ptr %5, align 4
  ret void
}

define common fastcc void @addChar(ptr %0, i8 %1) {
entry:
  %2 = getelementptr ptr, ptr %0, i64 1
  %len = load i64, ptr %2, align 4
  %3 = getelementptr ptr, ptr %0, i64 2
  %maxlen = load i64, ptr %3, align 4
  %4 = icmp eq i64 %len, %maxlen
  br i1 %4, label %grow_begin, label %grow_close

grow_begin:                                       ; preds = %entry
  %5 = getelementptr ptr, ptr %0, i64 3
  %factor = load i64, ptr %5, align 4
  %6 = add i64 %maxlen, %factor
  call void @resizeStr(ptr %0, i64 %6)
  br label %grow_close

grow_close:                                       ; preds = %grow_begin, %entry
  %7 = getelementptr ptr, ptr %0, i64 0
  %buffer = load ptr, ptr %7, align 8
  %8 = getelementptr ptr, ptr %buffer, i64 %len
  store i8 %1, ptr %8, align 1
  %9 = add i64 %len, 1
  store i64 %9, ptr %2, align 4
  ret void
}

declare common void @putch(i8)

declare common void @puts(%str)

declare common %str @read()

attributes #0 = { nounwind }