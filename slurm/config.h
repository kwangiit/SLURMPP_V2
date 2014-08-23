/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define the BG_BRIDGE_SO value */
/* #undef BG_BRIDGE_SO */

/* Define the BG_DB2_SO value */
/* #undef BG_DB2_SO */

/* Define the BG_SERIAL value */
/* #undef BG_SERIAL */

/* Define BLCR installation home */
#define BLCR_HOME ""

/* Define location of cpuset directory */
/* #undef CPUSET_DIR */

/* Define to 1 for debugger partial task attach support. */
#define DEBUGGER_PARTIAL_ATTACH 1

/* Define to 1 if using glib-2.32.0 or higher */
#define GLIB_NEW_THREADS 1

/* Define to 1 if licensed under terms of the GNU General Public License. */
#define GPL_LICENSED 1

/* Define to 1 if using gtk+-2.14.0 or higher */
#define GTK2_USE_GET_FOCUS 1

/* Define to 1 if using gtk+-2.10.0 or higher */
#define GTK2_USE_RADIO_SET 1

/* Define to 1 if using gtk+-2.12.0 or higher */
#define GTK2_USE_TOOLTIP 1

/* Make sure we get the 1.8 HDF5 API */
#define H5_NO_DEPRECATED_SYMBOLS 1

/* Define to 1 if 3-dimensional architecture */
/* #undef HAVE_3D */

/* Define to 1 if 4-dimensional architecture */
/* #undef HAVE_4D */

/* Define to 1 for AIX operating system */
/* #undef HAVE_AIX */

/* Define to 1 for Cray XT/XE systems using ALPS */
/* #undef HAVE_ALPS_CRAY */

/* Define to 1 for emulating a Cray XT/XE system using ALPS */
/* #undef HAVE_ALPS_CRAY_EMULATION */

/* Define to 1 if running against an ALPS emulation */
/* #undef HAVE_ALPS_EMULATION */

/* Define to 1 if emulating or running on Blue Gene system */
/* #undef HAVE_BG */

/* Define to 1 if emulating or running on Blue Gene/L system */
/* #undef HAVE_BGL */

/* Define to 1 if emulating or running on Blue Gene/P system */
/* #undef HAVE_BGP */

/* Define to 1 if emulating or running on Blue Gene/Q system */
/* #undef HAVE_BGQ */

/* Define to 1 if have Blue Gene files */
/* #undef HAVE_BG_FILES */

/* Define to 1 if using code where blocks have actions */
/* #undef HAVE_BG_GET_ACTION */

/* Define to 1 if emulating or running on Blue Gene/L or P system */
/* #undef HAVE_BG_L_P */

/* Define to 1 if using code with new iocheck */
/* #undef HAVE_BG_NEW_IO_CHECK */

/* Define to 1 if you have the `cfmakeraw' function. */
#define HAVE_CFMAKERAW 1

/* Define to 1 for systems with a Cray network */
/* #undef HAVE_CRAY_NETWORK */

/* Define to 1 if you have the <curses.h> header file. */
#define HAVE_CURSES_H 1

/* Define to 1 if you have the declaration of `hstrerror', and to 0 if you
   don't. */
#define HAVE_DECL_HSTRERROR 0

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
   don't. */
#define HAVE_DECL_STRERROR_R 1

/* Define to 1 if you have the declaration of `strsignal', and to 0 if you
   don't. */
#define HAVE_DECL_STRSIGNAL 1

/* Define to 1 if you have the declaration of `sys_siglist', and to 0 if you
   don't. */
#define HAVE_DECL_SYS_SIGLIST 0

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if function EVP_MD_CTX_cleanup exists. */
#define HAVE_EVP_MD_CTX_CLEANUP 1

/* Define to 1 if you have the `fdatasync' function. */
#define HAVE_FDATASYNC 1

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if freeipmi library found */
/* #undef HAVE_FREEIPMI */

/* Define to 1 if running slurmd on front-end only */
/* #undef HAVE_FRONT_END */

/* Defined if you have HDF5 support */
/* #undef HAVE_HDF5 */

/* Define to 1 if you have the `hstrerror' function. */
#define HAVE_HSTRERROR 1

/* Define to 1 if hwloc library found */
/* #undef HAVE_HWLOC */

/* Define to 1 if hwloc library supports PCI devices */
/* #undef HAVE_HWLOC_PCI */

/* Define to 1 if you have the `inet_aton' function. */
#define HAVE_INET_ATON 1

/* Define to 1 if you have the `inet_ntop' function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have the `inet_pton' function. */
#define HAVE_INET_PTON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <kstat.h> header file. */
/* #undef HAVE_KSTAT_H */

/* Define to 1 if you have the `expat' library (-lexpat). */
/* #undef HAVE_LIBEXPAT */

/* Define to 1 if you have the `job' library (-ljob). */
/* #undef HAVE_LIBJOB */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <linux/sched.h> header file. */
#define HAVE_LINUX_SCHED_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <mcheck.h> header file. */
#define HAVE_MCHECK_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mtrace' function. */
#define HAVE_MTRACE 1

/* Define to 1 if using MySQL libaries */
#define HAVE_MYSQL 1

/* Define to 1 for running on a Cray in native mode without ALPS */
/* #undef HAVE_NATIVE_CRAY */

/* Define to 1 if alpscomm functions new to CLE 5.2UP01 are defined */
/* #undef HAVE_NATIVE_CRAY_GA */

/* Define to 1 if you have the <ncurses.h> header file. */
#define HAVE_NCURSES_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* define if you have nrt.h */
/* #undef HAVE_NRT_H */

/* define if numa library installed */
/* #undef HAVE_NUMA */

/* Define to 1 if ofed library found */
/* #undef HAVE_OFED */

/* Define to 1 if using code with pma_query_via */
/* #undef HAVE_OFED_PMA_QUERY_VIA */

/* define if you have openssl. */
#define HAVE_OPENSSL 1

/* define if you have the PAM library */
/* #undef HAVE_PAM */

/* Define to 1 if you have the <pam/pam_appl.h> header file. */
/* #undef HAVE_PAM_PAM_APPL_H */

/* Define to 1 if you have the <paths.h> header file. */
#define HAVE_PATHS_H 1

/* define if you have permapi_h */
/* #undef HAVE_PERMAPI_H */

/* Define if you have Posix semaphores. */
/* #undef HAVE_POSIX_SEMS */

/* Define to 1 if you have the <proctrack.h> header file. */
/* #undef HAVE_PROCTRACK_H */

/* Define if libc sets program_invocation_name */
#define HAVE_PROGRAM_INVOCATION_NAME 1

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 1

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Have PTHREAD_PRIO_INHERIT. */
/* #undef HAVE_PTHREAD_PRIO_INHERIT */

/* Define to 1 if you have the `ptrace64' function. */
/* #undef HAVE_PTRACE64 */

/* Define to 1 if you have the <pty.h> header file. */
#define HAVE_PTY_H 1

/* Define if you are compiling with readline. */
/* #undef HAVE_READLINE */

/* Define to 1 for running on a real Cray system */
/* #undef HAVE_REAL_CRAY */

/* Define to 1 if you have the `sched_setaffinity' function. */
#define HAVE_SCHED_SETAFFINITY 1

/* Define to 1 if you have the <security/pam_appl.h> header file. */
/* #undef HAVE_SECURITY_PAM_APPL_H */

/* Define to 1 if you have the `setproctitle' function. */
/* #undef HAVE_SETPROCTITLE */

/* Define to 1 if you have the `setresuid' function. */
#define HAVE_SETRESUID 1

/* Define to 1 if you have the <socket.h> header file. */
/* #undef HAVE_SOCKET_H */

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `strerror_r' function. */
#define HAVE_STRERROR_R 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strndup' function. */
#define HAVE_STRNDUP 1

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* define if Sun Constellation system */
/* #undef HAVE_SUN_CONST */

/* Define to 1 if you have the `sysctlbyname' function. */
/* #undef HAVE_SYSCTLBYNAME */

/* Define to 1 if you have the <sysint.h> header file. */
/* #undef HAVE_SYSINT_H */

/* Define to 1 if you have the <sys/dr.h> header file. */
/* #undef HAVE_SYS_DR_H */

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/prctl.h> header file. */
#define HAVE_SYS_PRCTL_H 1

/* Define to 1 if you have the <sys/ptrace.h> header file. */
#define HAVE_SYS_PTRACE_H 1

/* Define to 1 if you have the <sys/sem.h> header file. */
#define HAVE_SYS_SEM_H 1

/* Define to 1 if you have the <sys/shm.h> header file. */
#define HAVE_SYS_SHM_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/statfs.h> header file. */
#define HAVE_SYS_STATFS_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#define HAVE_SYS_SYSCTL_H 1

/* Define to 1 if you have the <sys/syslog.h> header file. */
#define HAVE_SYS_SYSLOG_H 1

/* Define to 1 if you have the <sys/systemcfg.h> header file. */
/* #undef HAVE_SYS_SYSTEMCFG_H */

/* Define to 1 if you have the <sys/termios.h> header file. */
#define HAVE_SYS_TERMIOS_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/vfs.h> header file. */
#define HAVE_SYS_VFS_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <termcap.h> header file. */
#define HAVE_TERMCAP_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `unsetenv' function. */
#define HAVE_UNSETENV 1

/* Define to 1 if you have the <utmp.h> header file. */
#define HAVE_UTMP_H 1

/* Define to 1 if you have the <values.h> header file. */
#define HAVE_VALUES_H 1

/* Define to 1 if using XCPU for job launch */
/* #undef HAVE_XCPU */

/* Define if you have __progname. */
#define HAVE__PROGNAME 1

/* Define to 1 if you have the external variable, _system_configuration with a
   member named physmem. */
/* #undef HAVE__SYSTEM_CONFIGURATION */

/* Define the libnrt.so location */
/* #undef LIBNRT_SO */

/* Define to 1 for --get-user-env to load user environment without .login */
/* #undef LOAD_ENV_NO_LOGIN */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 for memory leak debugging. */
/* #undef MEMORY_LEAK_DEBUG */

/* Enable multiple slurmd on one node */
/* #undef MULTIPLE_SLURMD */

/* Define to 1 if with non thread-safe code */
/* #undef MYSQL_NOT_THREAD_SAFE */

/* Define to 1 if you are building a production release. */
#define NDEBUG 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "slurm-dev@schedmd.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "slurm"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "slurm 14.11"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "slurm"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://slurm.schedmd.com"

/* Define to the version of this package. */
#define PACKAGE_VERSION "14.11"

/* Define PAM module installation directory. */
#define PAM_DIR ""

/* Define the project's name. */
#define PROJECT "slurm"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define to 1 if ptrace takes five arguments. */
/* #undef PTRACE_FIVE_ARGS */

/* Define the project's release. */
#define RELEASE "pre1"

/* Define to 1 for salloc to kill child processes at job termination */
/* #undef SALLOC_KILL_CMD */

/* Define to 1 to require salloc execution in the foreground. */
/* #undef SALLOC_RUN_FOREGROUND */

/* Define to 1 if sched_getaffinity takes three arguments. */
#define SCHED_GETAFFINITY_THREE_ARGS 1

/* Define to 1 if sched_getaffinity takes two arguments. */
/* #undef SCHED_GETAFFINITY_TWO_ARGS */

/* Define to 1 if setpgrp takes two arguments. */
/* #undef SETPGRP_TWO_ARGS */

/* Define if you need setproctitle padding */
#define SETPROCTITLE_PS_PADDING '\0'

/* Define to the setproctitle() emulation type */
#define SETPROCTITLE_STRATEGY PS_USE_CLOBBER_ARGV

/* Define the default port number for slurmctld */
#define SLURMCTLD_PORT 6817

/* Define the default port count for slurmctld */
#define SLURMCTLD_PORT_COUNT 1

/* Define the default port number for slurmdbd */
#define SLURMDBD_PORT 6819

/* Define the default port number for slurmd */
#define SLURMD_PORT 6818

/* API current age */
#define SLURM_API_AGE 0

/* API current version */
#define SLURM_API_CURRENT 28

/* API current major */
#define SLURM_API_MAJOR 28

/* API current rev */
#define SLURM_API_REVISION 0

/* Define the API's version */
#define SLURM_API_VERSION 0x1c0000

/* Define if your architecture's byteorder is big endian. */
/* #undef SLURM_BIGENDIAN */

/* Define the project's major version. */
#define SLURM_MAJOR "14"

/* Define the project's micro version. */
#define SLURM_MICRO "0"

/* Define the project's minor version. */
#define SLURM_MINOR "11"

/* Define Slurm installation prefix */
#define SLURM_PREFIX "/users/kwangiit/slurmpp_v2/install"

/* Define to 1 if running slurm simulator */
/* #undef SLURM_SIMULATOR */

/* SLURM Version Number */
#define SLURM_VERSION_NUMBER 0x0e0b00

/* Define the project's version string. */
#define SLURM_VERSION_STRING "14.11.0-pre1"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if strerror_r returns char *. */
/* #undef STRERROR_R_CHAR_P */

/* 3-dimensional architecture */
/* #undef SYSTEM_DIMENSIONS */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define slurm_ prefix function aliases for plugins */
#define USE_ALIAS 1

/* define if using ISO 8601 time format */
#define USE_ISO_8601 /**/

/* Define the project's version. */
#define VERSION "14.11.0"

/* Define if you have pthreads. */
#define WITH_PTHREADS 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define location of XCPU directory */
/* #undef XCPU_DIR */

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */
