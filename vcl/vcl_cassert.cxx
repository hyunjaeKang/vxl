/*
  fsm@robots.ox.ac.uk
*/
#include "vcl_compiler.h"

#if defined(VCL_SUNPRO_CC) || defined(VCL_KAI)
// fsm@robots: This is a silly hack to enable us to link code compiled with SunPro
// against a library (such as libMesaGL) compiled with gcc. The gcc assert macro
// uses a function call __eprintf() which is defined in gcc/libgcc2.c in the gcc
// sources.
#include <cstdio>
#include <cstdlib>
extern "C" void
__eprintf (char const *string, char const *expression,
	   unsigned int line, char const *filename)
{
  std::fprintf(stderr, string, expression, line, filename);
  std::fflush(stderr);
  std::abort();
}
#endif

#ifdef GNU_LIBSTDCXX_V3
#include <cstdio>
#include <cstdlib>
void vcl_cassert_failure(char const *FILE, int LINE, char const *expr)
{
  std::fprintf(stderr, "%s:%d assertion failure \'%s\'\n", FILE, LINE, expr);
  std::fflush(stderr);
  std::abort();
}
#endif
