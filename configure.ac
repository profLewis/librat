
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/hipl_format.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lc':
AC_CHECK_LIB([c], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lratlib':
AC_CHECK_LIB([ratlib], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h netdb.h stdlib.h string.h strings.h sys/file.h sys/param.h sys/socket.h sys/time.h unistd.h values.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([gethostbyname gethostname memmove pow sqrt strcasecmp strchr strdup strerror strncasecmp strrchr])


AC_CONFIG_FILES([
 makefile
 src/makefile
])
AC_OUTPUT
