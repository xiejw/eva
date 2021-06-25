#ifndef VALUE_H_
#define VALUE_H_

#include <stdint.h>

// Represents a value in the core system used by many data strucutures, e.g.,
// dict.

enum value_kind_t {
        VKIND_I64,
        VKIND_U64,
        VKIND_F64,
        VKIND_F32,
        VKIND_DATA,
};

struct value_t {
        enum value_kind_t kind;  // might not see for perf reason.
        union {
                int64_t  i64;
                uint64_t u64;
                double   f64;
                float    f32;
                void    *data;
        };
};

struct value_t *valueNewData(void *data);
struct value_t *valueNewI64(int64_t i);
struct value_t *valueNewF32(float f);
void            valueFree(struct value_t *);

#define valueGetKind(v) (((struct value_t *)(v))->kind)
#define valueGetI64(v)  (((struct value_t *)(v))->i64)
#define valueGetU64(v)  (((struct value_t *)(v))->u64)
#define valueGetF32(v)  (((struct value_t *)(v))->f32)
#define valueGetF64(v)  (((struct value_t *)(v))->f64)

#define valueSetI64(v, _val_) _VALUE_SET_I64_IMPL(v, _val_)
#define valueSetU64(v, _val_) _VALUE_SET_U64_IMPL(v, _val_)
#define valueSetF32(v, _val_) _VALUE_SET_F32_IMPL(v, _val_)
#define valueSetF64(v, _val_) _VALUE_SET_F64_IMPL(v, _val_)

// Used for dict.
int valueCmpI64(void *privdata, const void *, const void *);
int valueCmpU64(void *privdata, const void *, const void *);
int valueCmpF32(void *privdata, const void *, const void *);
int valueCmpF64(void *privdata, const void *, const void *);

void *valueDupI64(void *privdata, const void *);
void *valueDupU64(void *privdata, const void *);
void *valueDupF32(void *privdata, const void *);
void *valueDupF64(void *privdata, const void *);

void valueFreeI64(void *privdata, void *);
void valueFreeu64(void *privdata, void *);
void valueFreeF32(void *privdata, void *);
void valueFreeF64(void *privdata, void *);

uint64_t valueHashFnI64(const void *key);
uint64_t valueHashFnU64(const void *key);
uint64_t valueHashFnF32(const void *key);
uint64_t valueHashFnF64(const void *key);

// macro impl
#define _VALUE_SET_I64_IMPL(v, _val_)                   \
        do {                                            \
                ((struct value_t *)(v))->i64 = (_val_); \
        } while (0)

#define _VALUE_SET_U64_IMPL(v, _val_)                   \
        do {                                            \
                ((struct value_t *)(v))->u64 = (_val_); \
        } while (0)

#define _VALUE_SET_F32_IMPL(v, _val_)                   \
        do {                                            \
                ((struct value_t *)(v))->f32 = (_val_); \
        } while (0)

#define _VALUE_SET_F64_IMPL(v, _val_)                   \
        do {                                            \
                ((struct value_t *)(v))->f64 = (_val_); \
        } while (0)

#endif
