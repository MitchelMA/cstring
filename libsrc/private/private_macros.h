#ifndef CSTRING_PRIVATE_H__
#define CSTRING_PRIVATE_H__

#ifndef NODISCARD
    #define NODISCARD
    #if defined(__GNUC__) && !defined(__clang__)
        #undef NODISCARD
        #define NODISCARD [[nodiscard]]
    #endif // __GNUC__ && !__clang__
#endif // !NODISCARD

#ifndef UNUSED
    #define UNUSED(VAR) (void)(VAR)
#endif // !UNUSED

#endif // !CSTRING_PRIVATE_H__
