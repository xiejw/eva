#include "base/error.h"

#include "assert.h"
#include "stdio.h"
#include "string.h"

// eva
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
error_t
errEmitNote(const char* fmt, ...)
{
        assert(err_msg_header != NULL);
        va_list ap;
        va_start(ap, fmt);
        errMsgAppend(fmt, ap);
        va_end(ap);
        return err_msg_header->err;
}

error_t
errNewWithNote(error_t err, const char* fmt, ...)
{
        va_list ap;
        va_start(ap, fmt);
        errWrapNoteVprintf(err, fmt, ap);
        va_end(ap);
        return err;
}

// Same as errNewWithNote(ERROR, fmt, ...)
error_t
errNew(const char* fmt, ...)
{
        va_list ap;
        va_start(ap, fmt);
        errWrapNoteVprintf(ERROR, fmt, ap);
        va_end(ap);
        return ERROR;
}

// current error code.
error_t
errNum()
{
        assert(err_msg_header != NULL);
        return err_msg_header->err;
}

// clears all allocated objs for error messages. no-op if no error.
void
errFree()
{
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

// print to stderr for all messages with leading title `msg` (a new line will
// be appended after the msg if absent).
void
errDump(const char* fmt, ...)
{
        assert(err_msg_header != NULL);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        if (fmt[strlen(fmt) - 1] != '\n')
                fprintf(stderr, "\n");  // force a new line if absent.

        err_msg_list_t* p = err_msg_header->prev;
        while (p != err_msg_header) {
                fprintf(stderr, "  > %s\n", p->msg);
                p = p->prev;
        }
}

// print an error message and quit immediately. If there is an error already,
// also print erro stack by calling errDump.
error_t
errFatalAndExit_(char* file, int line_no, const char* fmt, ...)
{
        fprintf(stderr, "fatal error: at %s line %d:\n", file, line_no);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "\n");

        if (err_msg_header != NULL) {
                errDump("\nfound existing error as follows:");
        }

        fflush(stderr);
        exit(1);
        return ERROR;
}

// -----------------------------------------------------------------------------
// helper methods. implementation.
// -----------------------------------------------------------------------------

void
errNewHeader(error_t err)
{
        assert(err_msg_header == NULL);
        err_msg_list_t* p = malloc(sizeof(err_msg_list_t));
        p->err            = err;
        p->next           = p;
        p->prev           = p;
        err_msg_header    = p;
}

void
errMsgAppend(const char* fmt, va_list ap)
{
        sds_t s = sdsEmptyWithCap(ERR_MSG_DEFAULT_LEN);
        sdsCatVprintf(&s, fmt, ap);

        err_msg_list_t* p          = malloc(sizeof(err_msg_list_t));
        p->msg                     = s;  // s ownership moved.
        p->prev                    = err_msg_header->prev;
        p->next                    = err_msg_header;
        err_msg_header->prev->next = p;
        err_msg_header->prev       = p;
}

error_t
errWrapNoteVprintf(error_t err, const char* fmt, va_list ap)
{
        assert(err_msg_header == NULL);
        assert(err != OK);

        errNewHeader(err);
        errMsgAppend(fmt, ap);
        return err;
}
