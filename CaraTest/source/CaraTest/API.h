
#ifndef CARATEST_API_H
#define CARATEST_API_H

#ifdef CARATEST_STATIC_DEFINE
#  define CARATEST_API
#  define CARATEST_NO_EXPORT
#else
#  ifndef CARATEST_API
#    ifdef CaraTest_EXPORTS
        /* We are building this library */
#      define CARATEST_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define CARATEST_API __declspec(dllimport)
#    endif
#  endif

#  ifndef CARATEST_NO_EXPORT
#    define CARATEST_NO_EXPORT 
#  endif
#endif

#ifndef CARATEST_DEPRECATED
#  define CARATEST_DEPRECATED __declspec(deprecated)
#endif

#ifndef CARATEST_DEPRECATED_EXPORT
#  define CARATEST_DEPRECATED_EXPORT CARATEST_API CARATEST_DEPRECATED
#endif

#ifndef CARATEST_DEPRECATED_NO_EXPORT
#  define CARATEST_DEPRECATED_NO_EXPORT CARATEST_NO_EXPORT CARATEST_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CARATEST_NO_DEPRECATED
#    define CARATEST_NO_DEPRECATED
#  endif
#endif

#endif /* CARATEST_API_H */
