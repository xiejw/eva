#ifndef CRON_FIELD_H_
#define CRON_FIELD_H_

#include <assert.h>
#include <stdlib.h>

#define CRON_FIELD_KIND_ANY          0
#define CRON_FIELD_KIND_SINGLE_VALUE 1

typedef struct {
  int kind;  // 0
  int value;
} cron_field_t;

int           cronFieldMatch(cron_field_t* field, int v);
cron_field_t* cronFieldAny();
cron_field_t* cronFieldSingleValue(int v);
void          cronFieldFree(cron_field_t* p);

#endif
