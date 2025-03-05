/*
 * @file Vector.c
 * @brief Stimulate vector in c++
 * @author Ldk, InnoLegend team.
 * @date Mar.21st,2024
 */

#include "vector.h"

/**
 * @brief Initialize the vector
 * @param vec Pointer to the vector to initialize
 * @param buffer Pointer to the buffer to use for storing elements
 * @param elem_size Size of each element in the vector
 * @param capacity Maximum number of elements the vector can hold
 * @details This function initializes the vector with the provided buffer, element size, and capacity.
 */
void vector_init(Vector* vec, void* buffer, size_t elem_size, size_t capacity) {
    vec->buffer = buffer;
    vec->elem_size = elem_size;
    vec->capacity = capacity;
    vec->size = 0;
}

/**
 * @brief Add an element to the end of the vector
 * @param vec Pointer to the vector
 * @param value Pointer to the value to add
 * @return VectorError indicating success or failure
 * @details This function adds an element to the end of the vector. If the vector is full, it returns VECTOR_FULL.
 */
VectorError vector_push_back(Vector* vec, const void* value) {
    if (vec->size >= vec->capacity) return VECTOR_FULL;
    
    // Manually copy bytes instead of using memcpy
    char* dest = (char*)vec->buffer + vec->size * vec->elem_size;
    const char* src = (const char*)value;
    for (size_t i = 0; i < vec->elem_size; i++) {
        dest[i] = src[i];
    }
    
    vec->size++;
    return VECTOR_OK;
}

/**
 * @brief Get a pointer to an element at a specific index
 * @param vec Pointer to the vector
 * @param index Index of the element to retrieve
 * @param element Pointer to store the address of the element
 * @return VectorError indicating success or failure
 * @details This function retrieves a pointer to an element at the specified index. If the index is out of bounds, it returns VECTOR_BAD_INDEX.
 */
VectorError vector_at(const Vector* vec, size_t index, void** element) {
    if (index >= vec->size) return VECTOR_BAD_INDEX;
    *element = (char*)vec->buffer + index * vec->elem_size;
    return VECTOR_OK;
}

/**
 * @brief Remove the last element from the vector
 * @param vec Pointer to the vector
 * @return VectorError indicating success or failure
 * @details This function removes the last element from the vector. If the vector is empty, it returns VECTOR_EMPTY.
 */
VectorError vector_pop_back(Vector* vec) {
    if (vec->size == 0) return VECTOR_EMPTY;
    vec->size--;
    return VECTOR_OK;
}

/**
 * @brief Get the current size of the vector
 * @param vec Pointer to the vector
 * @return The current size of the vector
 * @details This function returns the number of elements currently in the vector.
 */
size_t vector_size(const Vector* vec) { return vec->size; }

/**
 * @brief Get the capacity of the vector
 * @param vec Pointer to the vector
 * @return The capacity of the vector
 * @details This function returns the maximum number of elements the vector can hold.
 */
size_t vector_capacity(const Vector* vec) { return vec->capacity; }

/*
Example:

#include "vector.h"

int main() {
    // User-defined static memory (can store 10 integers)
    int buffer[10];
    Vector vec;
    vector_init(&vec, buffer, sizeof(int), 10);

    // Add an element
    int val = 42;
    VectorError err = vector_push_back(&vec, &val);
    if (err != VECTOR_OK) {
        // Handle error (e.g., vector is full)
        return -1;
    }

    // Access an element
    int* elem;
    if (vector_at(&vec, 0, (void**)&elem) == VECTOR_OK) {
        *elem = 100; // Directly modify the element
    }

    return 0;
}
*/