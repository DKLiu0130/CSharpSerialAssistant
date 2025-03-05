#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

// ��������
typedef enum {
    VECTOR_OK = 0,
    VECTOR_FULL,
    VECTOR_EMPTY,
    VECTOR_BAD_INDEX
} VectorError;

typedef struct {
    void* buffer;       // �û��ṩ���ڴ滺����
    size_t elem_size;   // ����Ԫ�ش�С���ֽڣ�
    size_t capacity;    // ���������Ԫ�ظ�����
    size_t size;        // ��ǰԪ������
} Vector;

// ��ʼ��Vector�����û���ǰ����buffer�ڴ棩
void vector_init(Vector* vec, void* buffer, size_t elem_size, size_t capacity);

// Ԫ�ز���
VectorError vector_push_back(Vector* vec, const void* value);
VectorError vector_pop_back(Vector* vec);
VectorError vector_at(const Vector* vec, size_t index, void** element);

// ��Ϣ��ȡ
size_t vector_size(const Vector* vec);
size_t vector_capacity(const Vector* vec);

#endif // VECTOR_H