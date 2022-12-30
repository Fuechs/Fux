; ModuleID = 'fux compiler'
source_filename = "fux compiler"

%str = type { ptr, i64, i64, i64 }

@example = common global %str

define common fastcc void @createDefaultStr(ptr %0) {
entry:
  ret void
}