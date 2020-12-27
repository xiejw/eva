#ifndef ERROR_H_
#define ERROR_H_

// Design:
//
// - All error codes are negative numbers. 0 is success.
// - Limited error codes are pre-defined here.
//   - If possible matching to these cannonical codes or just -1 (ERROR)
//
// - Upon success, the control flow should be as fast as checking a int.
// - Upon failure,
//   - if the calling API is not error_t type, use errNewWithNote, or emitNew.
//   - if the calling API is error_t type
//     - use errEmitNote to propagate the error to upstream.
//     - use errFree to free all resources if not ropagating any further.

typedef int error_t;

#define OK 0

#define ERROR     -1
#define EMALLOC   -2
#define ENOTEXIST -3
#define ENOTIMPL  -4

// APis for error handling.

extern error_t errNew(const char* fmt, ...);
extern error_t errNewWithNote(error_t err, const char* fmt, ...);
extern error_t errEmitNote(const char* fmt, ...);
extern void    errFree();

extern error_t errNum();
extern void    errDump(char*);

extern void errFatalAndExit(const char* fmt, ...);

// helper methods.
#define errMalloc() \
  errNewWithNote(EMALLOC, "malloc file: %s, loc: $d", __FILE__, __LINE__)

#endif
