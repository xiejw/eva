#ifndef CRON_FIELD_H_
#define CRON_FIELD_H_

#include <assert.h>
#include <stdlib.h>

#define CRON_FIELD_KIND_ANY          0
#define CRON_FIELD_KIND_SINGLE_VALUE 1

typedef struct {
        int kind;
        int value;
} cron_field_t;

int cronFieldMatch(cron_field_t* field, int v);

// inline helper methods.
static inline void cronFieldSetAsAny(cron_field_t* p)
{
        p->kind = CRON_FIELD_KIND_ANY;
}

static inline void cronFieldSetSingleValue(cron_field_t* p, int v)
{
        p->kind  = CRON_FIELD_KIND_SINGLE_VALUE;
        p->value = v;
}

#endif
