﻿//
// Copyright 2011-2023 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_BASE_POOL_H_
#define CODELIBRARY_BASE_POOL_H_

#include <algorithm>
#include <climits>

#include "codelibrary/base/log.h"

namespace cl {

/**
 * A simple but efficient object memory pool.
 *
 * Pool is used for dynamic management of objects of the same size, it can
 * allocate and deallocate memory for objects quickly.
 *
 * Pool implements a lazy lifecycle strategy. In this strategy objects are
 * default-constructed the first time they are allocated and destroyed when the
 * pool itself is destroyed.
 *
 * The time comparison between Pool and new/delete is given below:
 *                           Pool          new/delete
 * 10,000,000 allocate     0.093(s)         1.609(s)
 * 10,000,000 deallocate   0.032(s)         1.625(s)
 */
template <typename T>
class Pool {
    static const int MAX_CHUNCK_SIZE = 1048576;

    // The chunk for object pool.
    struct Chunk {
        explicit Chunk(int n)
            : size(n) {
            data = new (std::nothrow) T[n];
            CHECK(data) << "Memory is not enough.";

            ptr_data = new (std::nothrow) T*[n];
            CHECK(ptr_data) << "Memory is not enough.";

            for (int i = 0; i < size; ++i) {
                ptr_data[i] = &data[i];
            }
        }

        ~Chunk() {
            delete[] ptr_data;
            delete[] data;
        }

        int size = 0;           // The size of chunk.
        Chunk* prev = nullptr;  // The previous chunk of chunk list.
        Chunk* next = nullptr;  // The next chunk of chunk list.
        int used_size = 0;      // The size of used memory.
        T* data = nullptr;      // The actual data pointer.
        T** ptr_data = nullptr; // The pointer to the actual data.
    };

public:
    /**
     * Pool is perform as a free chunk list.
     * The default size of first chunk is 32.
     */
    explicit Pool(int first_chunk_size = 32)
        : first_chunk_size_(first_chunk_size) {
        CHECK(first_chunk_size_ > 0);
        first_chunk_ = cur_chunk_ = new Chunk(first_chunk_size_);
    }

    Pool(const Pool&) = delete;

    ~Pool() {
        ClearChunks();
    }

    void operator =(const Pool&) = delete;

    /**
     * Allocate an object from pool.
     *
     * Note that the returned pointer is not initialized.
     */
    T* Allocate() {
        CHECK(n_available_ < INT_MAX);

        if (cur_chunk_->used_size == cur_chunk_->size) {
            if (cur_chunk_->next == nullptr) {
                int n = std::min(cur_chunk_->size + cur_chunk_->size,
                                 MAX_CHUNCK_SIZE);
                if (n < 0) n = MAX_CHUNCK_SIZE;

                // If not enough memory, we create a new chunk.
                auto t = new Chunk(n);
                cur_chunk_->next = t;
                t->prev = cur_chunk_;
            }
            cur_chunk_ = cur_chunk_->next;
        }
        ++n_available_;
        n_allocated_ = std::max(n_allocated_, n_available_);

        return cur_chunk_->ptr_data[cur_chunk_->used_size++];
    }

    /**
     * Recycle a object, by putting it back to the pool.
     * Will failed if object is null.
     */
    void Deallocate(T* object) {
        CHECK(object);
        CHECK(n_available_ > 0);

        if (cur_chunk_->used_size == 0) {
            cur_chunk_ = cur_chunk_->prev;
        }
        --n_available_;
        cur_chunk_->ptr_data[--cur_chunk_->used_size] = object;
    }

    /**
     * Clear the pool.
     */
    void clear() {
        ClearChunks();
        n_available_ = 0;
        n_allocated_ = 0;
        cur_chunk_ = first_chunk_ = new Chunk(first_chunk_size_);
    }

    /**
     * Current available objects.
     */
    int n_available() const {
        return n_available_;
    }

    /**
     * The total number of allocated objects.
     */
    int n_allocated() const {
        return n_allocated_;
    }

private:
    /**
     * Clear the chunks.
     */
    void ClearChunks() {
        while (first_chunk_) {
            Chunk* p = first_chunk_->next;
            delete first_chunk_;
            first_chunk_ = p;
        }
    }

    int n_available_ = 0;          // The number of available objects.
    int n_allocated_ = 0;          // The number of allocated objects.
    Chunk* cur_chunk_ = nullptr;   // The current chunk.
    Chunk* first_chunk_ = nullptr; // The first chunk.
    int first_chunk_size_ = 0;     // The size of first chunk.
};

} // namespace cl

#endif // CODELIBRARY_BASE_POOL_H_
