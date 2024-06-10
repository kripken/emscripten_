#include "emscripten_internal.h"

// Use the simple/naive version when building with asan. Also use it when we
// want the smallest output and lack bulk memory (which would be yet smaller).
#if __has_feature(address_sanitizer) || (defined(EMSCRIPTEN_OPTIMIZE_FOR_OZ) && !defined(__wasm_bulk_memory__))

void *__memset(void *str, int c, size_t n) {
  unsigned char *s = (unsigned char *)str;
#pragma clang loop unroll(disable)
  while(n--) *s++ = c;
  return str;
}

// Use bulk memory when available, as it is the most compact and VMs can also
// make it the most efficient.
#elif defined(__wasm_bulk_memory__)

void *__memset(void *str, int c, size_t n) {
  return _emscripten_memset_bulkmem(str, c, n);
}

// Otherwise use a fast but large userspace implementation.
#else

#define memset __memset
#include "musl/src/string/memset.c"
#undef memset

#endif

weak_alias(__memset, emscripten_builtin_memset);
weak_alias(__memset, memset);
