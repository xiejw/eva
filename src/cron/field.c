#include "cron/field.h"

int cronFieldMatch(cron_field_t* field, int v) {
  if (field->kind == CRON_FIELD_KIND_ANY) return 1;
  assert(field->kind == CRON_FIELD_KIND_SINGLE_VALUE);
  return field->value == v;
}

cron_field_t* cronFieldAny() {
  cron_field_t* p = malloc(sizeof(cron_field_t));
  p->kind         = CRON_FIELD_KIND_ANY;
  return p;
}

cron_field_t* cronFieldSingleValue(int v) {
  cron_field_t* p = malloc(sizeof(cron_field_t));
  p->kind         = CRON_FIELD_KIND_SINGLE_VALUE;
  p->value        = v;
  return p;
}

void cronFieldFree(cron_field_t* p) { free(p); }
