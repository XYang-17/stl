#ifndef _YXXX_CONFIG_H_
#define _YXXX_CONFIG_H_

#ifndef _YXXX_CONSTEXPR
    #ifdef _GLIBCXX_CXX_CONFIG_H
        #define _YXXX_CONSTEXPR _GLIBCXX_CONSTEXPR
        #define _YXXX_USE_CONSTEXPR _GLIBCXX_USE_CONSTEXPR
    #elif __cplusplus >= 201103L
        #define _YXXX_CONSTEXPR constexpr
        #define _YXXX_USE_CONSTEXPR constexpr
    #else
        #define _YXXX_CONSTEXPR
        #define _YXXX_USE_CONSTEXPR const
    #endif
#endif

#ifndef _NOTEXCEPT
    #ifdef _GLIBCXX_CXX_CONFIG_H
        #define _YXXX_NOEXCEPT _GLIBCXX_NOEXCEPT
        #define _YXXX_NOEXCEPT_IF(...) _GLIBCXX_NOEXCEPT_IF(__VA_ARGS__)
        #define _YXXX_USE_NOEXCEPT _GLIBCXX_USE_NOEXCEPT
        #define _YXXX_THROW(_EXC) _GLIBCXX_THROW(_EXC)
    #elif __cplusplus >= 201103L
        #define _YXXX_NOEXCEPT noexcept
        #define _YXXX_NOEXCEPT_IF(...) noexcept(__VA_ARGS__)
        #define _YXXX_USE_NOEXCEPT noexcept
        #define _YXXX_THROW(_EXC)
    #else
        #define _YXXX_NOEXCEPT
        #define _YXXX_NOEXCEPT_IF(...)
        #define _YXXX_USE_NOEXCEPT throw()
        #define _YXXX_THROW(_EXC) throw(_EXC)
    #endif
#endif

#ifndef _YXXX_NOTHROE
    #ifdef _GLIBCXX_CXX_CONFIG_H
        #define _YXXX_NOTHROE _GLIBCXX_NOTHROW
    #else
        #define _YXXX_NOTHROE _YXXX_USE_NOEXCEPT
    #endif
#endif

#ifdef _GLIBCXX_CXX_CONFIG_H
    #define _YXXX_USE_WCHAR_T _GLIBCXX_USE_WCHAR_T
#else
    #define _YXXX_USE_WCHAR_T 1
#endif

#endif

