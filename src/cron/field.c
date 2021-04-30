#include "cron/field.h"

int
cronFieldMatch(cron_field_t* field, int v)
{
        if (field->kind == CRON_FIELD_KIND_ANY) return 1;
        assert(field->kind == CRON_FIELD_KIND_SINGLE_VALUE);
        return field->value == v;
}
