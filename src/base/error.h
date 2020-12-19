#ifndef ERROR_H_
#define ERROR_H_

// Design:
//
// - All error codes are negative numbers. 0 is success.
// - Limited error codes are pre-defined here. If possible matching to these
//   cannonical codes or just -1 (ERROR)
//
// - A special bit is used to indicate whether a thread local error message is
//   set.  This error message is stored in a global space. So, lifetime is
//   static.
//
//   With the bit stored in error code, it is easy to, based on this signal,
//   reset or concat the new error message.

typedef int error_t;

#define OK 0

#define ERROR      -1
#define ENOT_FOUND -2

extern error_t errNew(const char* fmt, ...);
extern error_t errWrapNote(error_t err, const char* fmt, ...);
extern error_t errNum();
extern void    errDump(char*);
extern void    errClear();

#endif
