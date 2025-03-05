#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

// 错误类型
typedef enum {
    VECTOR_OK = 0,
    VECTOR_FULL,
    VECTOR_EMPTY,
    VECTOR_BAD_INDEX
} VectorError;

typedef struct {
    void* buffer;       // 用户提供的内存缓冲区
    size_t elem_size;   // 单个元素大小（字节）
    size_t capacity;    // 最大容量（元素个数）
    size_t size;        // 当前元素数量
} Vector;

// 初始化Vector（需用户提前分配buffer内存）
void vector_init(Vector* vec, void* buffer, size_t elem_size, size_t capacity);

// 元素操作
VectorError vector_push_back(Vector* vec, const void* value);
VectorError vector_pop_back(Vector* vec);
VectorError vector_at(const Vector* vec, size_t index, void** element);

// 信息获取
size_t vector_size(const Vector* vec);
size_t vector_capacity(const Vector* vec);

#endif // VECTOR_H