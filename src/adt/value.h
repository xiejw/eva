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

struct value_t *valueNewData(void *data);
struct value_t *valueNewI64(int64_t i);
struct value_t *valueNewF32(float f);
void            valueFree(struct value_t *);

// Used for dict.
int   valueCmpI64(void *privdata, const void *, const void *);
int   valueCmpF32(void *privdata, const void *, const void *);
void *valueDupI64(void *privdata, const void *);
void *valueDupF32(void *privdata, const void *);
void  valueFreeI64(void *privdata, void *);
void  valueFreeF32(void *privdata, void *);

#define valueGetKind(v) (((struct value_t *)(v))->kind)
#define valueGetI64(v)  (((struct value_t *)(v))->i64)
#define valueGetF32(v)  (((struct value_t *)(v))->f32)

uint64_t valueHashFnI64(const void *key);

#endif
