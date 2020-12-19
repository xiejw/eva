#include "base/error.h"

#include "assert.h"
#include "stdio.h"

#include "adt/sds.h"

#define ERR_MSG_DEFAULT_LEN 128

typedef struct err_msg_list_t {
  union {
    sds_t   msg;
    error_t err;
  };
  struct err_msg_list_t* prev;
  struct err_msg_list_t* next;
} err_msg_list_t;

static err_msg_list_t* err_msg_header = NULL;

static void errNewHeader(error_t err) {
  assert(err_msg_header == NULL);
  err_msg_list_t* p = malloc(sizeof(err_msg_list_t));
  p->err            = err;
  p->next           = p;
  p->prev           = p;
  err_msg_header    = p;
}

static void errMsgAppend(error_t err, sds_t s) {
  err_msg_list_t* p          = malloc(sizeof(err_msg_list_t));
  p->msg                     = s;
  p->prev                    = err_msg_header->prev;
  p->next                    = err_msg_header;
  err_msg_header->prev->next = p;
  err_msg_header->prev       = p;
}

static void errMsgFree() {
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

static error_t errWrapNoteVprintf(error_t err, const char* fmt, va_list ap) {
  assert(err_msg_header == NULL);
  errNewHeader(err);
  assert(err != 0);
  sds_t s = sdsNewLen(SDS_NOINIT, ERR_MSG_DEFAULT_LEN);
  sdsClear(s);
  sdsCatVprintf(&s, fmt, ap);
  errMsgAppend(err, s);  // s ownership moved.
  return err;
}

error_t errWrapNote(error_t err, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  errWrapNoteVprintf(err, fmt, ap);
  va_end(ap);
  return err;
}

error_t errNew(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  errWrapNoteVprintf(ERROR, fmt, ap);
  va_end(ap);
  return ERROR;
}

error_t errNum() {
  assert(err_msg_header != NULL);
  return err_msg_header->err;
}

void errClear() { errMsgFree(); }
void errDump(char* msg) {
  assert(err_msg_header != NULL);
  fprintf(stderr, "%s:\n", msg);
  err_msg_list_t* p = err_msg_header->prev;
  while (p != err_msg_header) {
    fprintf(stderr, "  > %s\n", p->msg);
    p = p->prev;
  }
}
