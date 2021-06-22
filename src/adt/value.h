#ifndef VALUE_H_
#define VALUE_H_

#include <stdint.h>

// Represents a value in the core system used by many data strucutures, e.g.,
// dict.

enum value_kind_t {
        VKIND_INT,
        VKIND_DATA,
};

struct value_t {
        enum value_kind_t kind;
        union {
                int64_t i64;
                float   f32;
                void   *data;
        };
};

struc value_t *valueNewData(void *data);
struc value_t *valueNewI64(int64_t i);
struc value_t *valueNewF32(float f);
void           valueFree(struct value_t *);

#endif
