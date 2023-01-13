; ModuleID = 'fux compiler'
source_filename = "fux compiler"

%str = type { ptr, i64, i64, i64 }

declare ptr @malloc(i64)

declare void @free(ptr)

declare ptr @memcpy(ptr, ptr, i64)

; Function Attrs: nounwind
define fastcc void @Fux_str_create_default(ptr %0) #0 {
entry:
  %1 = getelementptr %str, ptr %0, i64 0, i32 0
  store ptr null, ptr %1, align 8
  %2 = getelementptr %str, ptr %0, i64 0, i32 1
  store i64 0, ptr %2, align 4
  %3 = getelementptr %str, ptr %0, i64 0, i32 2
  store i64 0, ptr %3, align 4
  %4 = getelementptr %str, ptr %0, i64 0, i32 3
  store i64 16, ptr %4, align 4
  ret void
}

; Function Attrs: nounwind
define fastcc void @Fux_str_delete(ptr %0) #0 {
entry:
  %1 = getelementptr %str, ptr %0, i64 0, i32 0
  %2 = load ptr, ptr %1, align 8
  %3 = icmp ne ptr %2, null
  br i1 %3, label %free_begin, label %free_close

free_begin:                                       ; preds = %entry
  call void @free(ptr %2)
  br label %free_close

free_close:                                       ; preds = %free_begin, %entry
  ret void
}

define fastcc void @Fux_str_resize(ptr %0, i64 %1) {
entry:
  %output = call ptr @malloc(i64 %1)
  %2 = getelementptr %str, ptr %0, i64 0, i32 0
  %buffer = load ptr, ptr %2, align 8
  %3 = getelementptr %str, ptr %0, i64 0, i32 1
  %len = load i64, ptr %3, align 4
  %4 = call ptr @memcpy(ptr %output, ptr %buffer, i64 %len)
  call void @free(ptr %buffer)
  store ptr %output, ptr %2, align 8
  %5 = getelementptr %str, ptr %0, i64 0, i32 2
  store i64 %1, ptr %5, align 4
  ret void
}

define fastcc void @Fux_str_add_char(ptr %0, i8 %1) {
entry:
  %2 = getelementptr %str, ptr %0, i64 0, i32 1
  %len = load i64, ptr %2, align 4
  %3 = getelementptr %str, ptr %0, i64 0, i32 2
  %maxlen = load i64, ptr %3, align 4
  %4 = icmp eq i64 %len, %maxlen
  br i1 %4, label %grow_begin, label %grow_close

grow_begin:                                       ; preds = %entry
  %5 = getelementptr %str, ptr %0, i64 0, i32 3
  %factor = load i64, ptr %5, align 4
  %6 = add i64 %maxlen, %factor
  call void @Fux_str_resize(ptr %0, i64 %6)
  br label %grow_close

grow_close:                                       ; preds = %grow_begin, %entry
  %7 = getelementptr %str, ptr %0, i64 0, i32 0
  %buffer = load ptr, ptr %7, align 8
  %8 = getelementptr ptr, ptr %buffer, i64 %len
  store i8 %1, ptr %8, align 1
  %9 = add i64 %len, 1
  store i64 %9, ptr %2, align 4
  ret void
}

; Function Attrs: nounwind
declare i32 @puts(ptr nocapture) #0

declare void @Fux_putch(i8)

define void @Fux_puts(ptr %0) {
entry:
  %buffer_ptr = getelementptr %str, ptr %0, i64 0, i32 0
  %buffer = load ptr, ptr %buffer_ptr, align 8
  %1 = call i32 @puts(ptr %buffer)
  ret void
}

declare void @Fux_read(ptr)

attributes #0 = { nounwind }
