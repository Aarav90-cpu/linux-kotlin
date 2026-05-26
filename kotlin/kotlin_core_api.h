#ifndef KONAN_KOTLIN_CORE_H
#define KONAN_KOTLIN_CORE_H
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
typedef bool            kotlin_core_KBoolean;
#else
typedef _Bool           kotlin_core_KBoolean;
#endif
typedef unsigned short     kotlin_core_KChar;
typedef signed char        kotlin_core_KByte;
typedef short              kotlin_core_KShort;
typedef int                kotlin_core_KInt;
typedef long long          kotlin_core_KLong;
typedef unsigned char      kotlin_core_KUByte;
typedef unsigned short     kotlin_core_KUShort;
typedef unsigned int       kotlin_core_KUInt;
typedef unsigned long long kotlin_core_KULong;
typedef float              kotlin_core_KFloat;
typedef double             kotlin_core_KDouble;
typedef float __attribute__ ((__vector_size__ (16))) kotlin_core_KVector128;
typedef void*              kotlin_core_KNativePtr;
struct kotlin_core_KType;
typedef struct kotlin_core_KType kotlin_core_KType;

typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Byte;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Short;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Int;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Long;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Float;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Double;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Char;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Boolean;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_Unit;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_UByte;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_UShort;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_UInt;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kotlin_ULong;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kernel_module_Module;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kernel_print_LogLevel;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kernel_types_Errno;
typedef struct {
  kotlin_core_KNativePtr pinned;
} kotlin_core_kref_kernel_types_Gfp;


typedef struct {
  /* Service functions. */
  void (*DisposeStablePointer)(kotlin_core_KNativePtr ptr);
  void (*DisposeString)(const char* string);
  kotlin_core_KBoolean (*IsInstance)(kotlin_core_KNativePtr ref, const kotlin_core_KType* type);
  kotlin_core_kref_kotlin_Byte (*createNullableByte)(kotlin_core_KByte);
  kotlin_core_KByte (*getNonNullValueOfByte)(kotlin_core_kref_kotlin_Byte);
  kotlin_core_kref_kotlin_Short (*createNullableShort)(kotlin_core_KShort);
  kotlin_core_KShort (*getNonNullValueOfShort)(kotlin_core_kref_kotlin_Short);
  kotlin_core_kref_kotlin_Int (*createNullableInt)(kotlin_core_KInt);
  kotlin_core_KInt (*getNonNullValueOfInt)(kotlin_core_kref_kotlin_Int);
  kotlin_core_kref_kotlin_Long (*createNullableLong)(kotlin_core_KLong);
  kotlin_core_KLong (*getNonNullValueOfLong)(kotlin_core_kref_kotlin_Long);
  kotlin_core_kref_kotlin_Float (*createNullableFloat)(kotlin_core_KFloat);
  kotlin_core_KFloat (*getNonNullValueOfFloat)(kotlin_core_kref_kotlin_Float);
  kotlin_core_kref_kotlin_Double (*createNullableDouble)(kotlin_core_KDouble);
  kotlin_core_KDouble (*getNonNullValueOfDouble)(kotlin_core_kref_kotlin_Double);
  kotlin_core_kref_kotlin_Char (*createNullableChar)(kotlin_core_KChar);
  kotlin_core_KChar (*getNonNullValueOfChar)(kotlin_core_kref_kotlin_Char);
  kotlin_core_kref_kotlin_Boolean (*createNullableBoolean)(kotlin_core_KBoolean);
  kotlin_core_KBoolean (*getNonNullValueOfBoolean)(kotlin_core_kref_kotlin_Boolean);
  kotlin_core_kref_kotlin_Unit (*createNullableUnit)(void);
  kotlin_core_kref_kotlin_UByte (*createNullableUByte)(kotlin_core_KUByte);
  kotlin_core_KUByte (*getNonNullValueOfUByte)(kotlin_core_kref_kotlin_UByte);
  kotlin_core_kref_kotlin_UShort (*createNullableUShort)(kotlin_core_KUShort);
  kotlin_core_KUShort (*getNonNullValueOfUShort)(kotlin_core_kref_kotlin_UShort);
  kotlin_core_kref_kotlin_UInt (*createNullableUInt)(kotlin_core_KUInt);
  kotlin_core_KUInt (*getNonNullValueOfUInt)(kotlin_core_kref_kotlin_UInt);
  kotlin_core_kref_kotlin_ULong (*createNullableULong)(kotlin_core_KULong);
  kotlin_core_KULong (*getNonNullValueOfULong)(kotlin_core_kref_kotlin_ULong);

  /* User functions. */
  struct {
    struct {
      struct {
        struct {
          struct {
            kotlin_core_KType* (*_type)(void);
            void (*exit)(kotlin_core_kref_kernel_module_Module thiz);
            kotlin_core_KInt (*init)(kotlin_core_kref_kernel_module_Module thiz);
          } Module;
        } module;
        struct {
          struct {
            kotlin_core_KType* (*_type)(void);
            kotlin_core_kref_kernel_print_LogLevel (*_instance)();
            kotlin_core_KInt (*get_ALERT)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_CRIT)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_DEBUG)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_EMERG)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_ERR)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_INFO)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_NOTICE)(kotlin_core_kref_kernel_print_LogLevel thiz);
            kotlin_core_KInt (*get_WARNING)(kotlin_core_kref_kernel_print_LogLevel thiz);
          } LogLevel;
          void (*pr_alert)(const char* msg);
          void (*pr_crit)(const char* msg);
          void (*pr_debug)(const char* msg);
          void (*pr_emerg)(const char* msg);
          void (*pr_err)(const char* msg);
          void (*pr_info)(const char* msg);
          void (*pr_notice)(const char* msg);
          void (*pr_warn)(const char* msg);
        } print;
        struct {
          struct {
            kotlin_core_KType* (*_type)(void);
            kotlin_core_kref_kernel_types_Errno (*_instance)();
            kotlin_core_KInt (*get_E2BIG)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EACCES)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EAGAIN)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EBADF)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EBUSY)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ECHILD)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EEXIST)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EFAULT)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EINTR)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EINVAL)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EIO)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENODATA)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENODEV)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENOENT)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENOEXEC)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENOMEM)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENOSPC)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENOSYS)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ENXIO)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EOPNOTSUPP)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EOVERFLOW)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_EPERM)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ERANGE)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ESRCH)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_ETIMEDOUT)(kotlin_core_kref_kernel_types_Errno thiz);
            kotlin_core_KInt (*get_SUCCESS)(kotlin_core_kref_kernel_types_Errno thiz);
          } Errno;
          struct {
            kotlin_core_KType* (*_type)(void);
            kotlin_core_kref_kernel_types_Gfp (*_instance)();
            kotlin_core_KUInt (*get_ATOMIC)(kotlin_core_kref_kernel_types_Gfp thiz);
            kotlin_core_KUInt (*get_KERNEL)(kotlin_core_kref_kernel_types_Gfp thiz);
            kotlin_core_KUInt (*get_ZERO)(kotlin_core_kref_kernel_types_Gfp thiz);
          } Gfp;
        } types;
        kotlin_core_KBoolean (*get_IS_MODULE)();
        const char* (*get_KOTLIN_KERNEL_VERSION)();
      } kernel;
    } root;
  } kotlin;
} kotlin_core_ExportedSymbols;
extern kotlin_core_ExportedSymbols* kotlin_core_symbols(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif  /* KONAN_KOTLIN_CORE_H */
