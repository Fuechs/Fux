; ModuleID = 'fux compiler'
source_filename = "fux compiler"

%str = type { ptr, i64, i64, i64 }

@example = common global %str

declare common ptr @malloc(i64)

declare common void @free(ptr)

declare common ptr @memcpy(ptr, ptr, i64)

define common fastcc void @createDefaultStr(ptr %0) {
entry:
  %1 = getelementptr ptr, ptr %0, i64 0
  store ptr null, ptr %1, align 8
  ret void
}