; ModuleID = 'fux compiler'
source_filename = "fux compiler"

%str = type { ptr, i64, i64, i64 }

declare common ptr @malloc(i64 %0)

declare common void @free(ptr %0)

declare common ptr @memcpy(ptr %0, ptr %1, i64 %2)

define common fastcc void @createDefaultStr(ptr %0) {
entry:
  %1 = getelementptr ptr, ptr %0, i64 0
  store ptr null, ptr %1, align 8
  ret void
}

declare common void @putch(i8 %0)

declare common void @puts(%str %0)

declare common %str @read()