#include "base/error.h"

#include "assert.h"
#include "stdio.h"

#include "adt/sds.h"

#define ERR_MSG_DEFAULT_LEN 128

typedef struct err_msg_list_t {
  union {
    sds_t   msg;  // used by nodes.
    error_t err;  // used by header.
  };
  struct err_msg_list_t* prev;
  struct err_msg_list_t* next;
} err_msg_list_t;

// -----------------------------------------------------------------------------
// helper methods prototypes.
// -----------------------------------------------------------------------------

static err_msg_list_t* err_msg_header = NULL;

static void    errNewHeader(error_t err);
static void    errMsgAppend(const char* fmt, va_list ap);
static error_t errWrapNoteVprintf(error_t err, const char* fmt, va_list ap);

// -----------------------------------------------------------------------------
// implementation.
// -----------------------------------------------------------------------------

// emit a new note and return the errro code stored.
error_t errEmitNote(const char* fmt, ...) {
  assert(err_msg_header != NULL);
  va_list ap;
  va_start(ap, fmt);
  errMsgAppend(fmt, ap);
  va_end(ap);
  return err_msg_header->err;
}

error_t errNewWithNote(error_t err, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  errWrapNoteVprintf(err, fmt, ap);
  va_end(ap);
  return err;
}

// Same as errNewWithNote(ERROR, fmt, ...)
error_t errNew(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  errWrapNoteVprintf(ERROR, fmt, ap);
  va_end(ap);
  return ERROR;
}

// current error code.
error_t errNum() {
  assert(err_msg_header != NULL);
  return err_msg_header->err;
}

// clears all allocated objs for error messages. no-op if no error.
void errFree() {
  if (err_msg_header == NULL) return;

  err_msg_list_t* p = err_msg_header->next;
  while (p != err_msg_header) {
    sdsFree(p->msg);
    p = p->next;
    free(p->prev);
  }
  free(err_msg_header);
  err_msg_header = NULL;
}

// prints to stderr for all messages with leading title `msg`.
void errDump(char* title) {
  assert(err_msg_header != NULL);
  fprintf(stderr, "%s:\n", title);
  err_msg_list_t* p = err_msg_header->prev;
  while (p != err_msg_header) {
    fprintf(stderr, "  > %s\n", p->msg);
    p = p->prev;
  }
}

error_t errFatalAndExit(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  fflush(stderr);
  exit(1);
  return ERROR;
}

// -----------------------------------------------------------------------------
// helper methods. implementation.
// -----------------------------------------------------------------------------

void errNewHeader(error_t err) {
  assert(err_msg_header == NULL);
  err_msg_list_t* p = malloc(sizeof(err_msg_list_t));
  p->err            = err;
  p->next           = p;
  p->prev           = p;
  err_msg_header    = p;
}

void errMsgAppend(const char* fmt, va_list ap) {
  sds_t s = sdsEmptyWithCap(ERR_MSG_DEFAULT_LEN);
  sdsCatVprintf(&s, fmt, ap);

  err_msg_list_t* p          = malloc(sizeof(err_msg_list_t));
  p->msg                     = s;  // s ownership moved.
  p->prev                    = err_msg_header->prev;
  p->next                    = err_msg_header;
  err_msg_header->prev->next = p;
  err_msg_header->prev       = p;
}

error_t errWrapNoteVprintf(error_t err, const char* fmt, va_list ap) {
  assert(err_msg_header == NULL);
  assert(err != OK);

  errNewHeader(err);
  errMsgAppend(fmt, ap);
  return err;
}
