#ifndef COMMON_H__
#define COMMON_H__

#define NODISCARD

#ifndef _MSC_VER
  #undef NODISCARD
  #define NODISCARD [[nodiscard]]
#endif // _MSC_VER

#if _MSC_VER >= 1700
  #undef NODISCARD
  #define NODISCARD _Check_return_
#endif // _MSC_VER >= 1700



#endif // COMMON_H__