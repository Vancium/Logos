#pragma once

#include "../../defines.h"

//Disable assertions
#define LASSERTIONS_ENABLED

#ifdef LASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

LAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define LASSERT(expr) { if(expr) {} else { report_assertion_failure(#expr, "", __FILE__, __LINE__); debugBreak();}}

#define LASSERT_MSG(expr, message) { if(expr) {} else { report_assertion_failure(#expr, message, __FILE__, __LINE__); debugBreak();}}

#ifdef _DEBUG
#define LASSERT_DEBUG(expr) { if(expr) {} else { report_assertion_failure(#expr, "", __FILE__, __LINE__); debugBreak();}}
#else 
#define LASLASSERT_DEBUG(expr) // no-op
#endif

#else
#define LASLASSERT(expr) // no-op
#define LLASSERT_MSG(expr, message) // no-op
#define LLASSERT_DEBUG(expr) // no-op

#endif


