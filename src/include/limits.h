#ifndef UBW_LIMITS_H_
#define UBW_LIMITS_H_

#define CHAR_BIT    8
#define CHAR_MIN    (-128)
#define CHAR_MAX    127

#define SCHAR_MIN   (-128)
#define SHRT_MIN    (-32768)
#define INT_MIN     (-2147483647 - 1)
#define LONG_MIN    (-2147483648L)      // MIPS1 (32-bit)

#define SCHAR_MAX   127
#define SHRT_MAX    32767
#define INT_MAX     2147483647
#define LONG_MAX    2147483647L

#define UCHAR_MAX   255
#define USHRT_MAX   65535
#define UINT_MAX    4294967295U
#define ULONG_MAX   4294967295UL

#endif // UBW_LIMITS_H_
