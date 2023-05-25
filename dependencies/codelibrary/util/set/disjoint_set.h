﻿//
// Copyright 2014-2022 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_UTIL_SET_DISJOINT_SET_H_
#define CODELIBRARY_UTIL_SET_DISJOINT_SET_H_

#include "codelibrary/base/array.h"

namespace cl {

/**
 * Disjoint-set structure.
 *
 * A Disjoint-Set structure (also called union-find or merge-find set) is used
 * to keep track of a set of elements partitioned into a number of disjoint
 * (non overlapping) subsets. It has two operations:
 * 1. Find: Determine which subset a particular element is in.
 *    This can be used for determining if two elements are in the same subset.
 * 2. Union: merge two subsets into a single subset.
 */
class DisjointSet {
public:
    /**
     * Construct the DisjointSet by size of the elements.
     */
    explicit DisjointSet(int size = 0)
        : size_(size), numbers_(size, 0), subsets_(size) {
        CHECK(size_ >= 0);

        for (int i = 0; i < size_; ++i) {
            subsets_[i] = i;
        }
    }

    /**
     * Reset the disjoint set.
     */
    void Reset(int size) {
        CHECK(size >= 0);

        size_ = size;

        numbers_.assign(size_, 0);
        subsets_.resize(size_);
        for (int i = 0; i < size_; ++i) {
            subsets_[i] = i;
        }
    }

    /**
     * Find the subset identifier that the i-th element currently belongs to.
     */
    int Find(int i) const {
        CHECK(i >= 0 && i < size_);

        while (i != subsets_[i]) {
            subsets_[i] = subsets_[subsets_[i]];
            i = subsets_[i];
        }
        return i;
    }

    /**
     * Merge the subset i into j.
     *
     * Unlike the function 'Union()', this function always merge the subset i
     * into subset j, but do not consider the cardinality of both subsets.
     *
     * Return the label ID of merged subset (always be j).
     */
    int Link(int i, int j) {
        CHECK(subsets_[i] == i);
        CHECK(subsets_[j] == j);
        CHECK(i != j);

        numbers_[j] += numbers_[i] + 1;
        subsets_[i] = j;
        return j;
    }

    /**
     * Merge the two subsets into ones.
     *
     * To reduce the path length in Find operation, the subset with the larger
     * elements will be merged into the subset with the smaller elements.
     *
     * Return the label ID of merged subset.
     */
    int Union(int i, int j) {
        int a = Find(i);
        int b = Find(j);
        if (a != b) {
            if (numbers_[a] > numbers_[b]) {
                numbers_[b] += numbers_[a] + 1;
                subsets_[a] = b;
                return b;
            }

            numbers_[a] += numbers_[b] + 1;
            subsets_[b] = a;
            return a;
        }

        return a;
    }

    /**
     * Convert disjoint set to clusters.
     */
    void ToClusters(Array<Array<int>>* clusters) const {
        CHECK(clusters);
        clusters->clear();

        int n_clusters = 0;
        Array<int> map(size_, -1);
        for (int i = 0; i < size_; ++i) {
            int p = Find(i);
            if (map[p] == -1) map[p] = n_clusters++;
        }

        clusters->resize(n_clusters);
        for (int i = 0; i < size_; ++i) {
            int t = map[Find(i)];
            (*clusters)[t].push_back(i);
        }
    }

    /**
     * Return the number of elements belong to i's parent.
     */
    int Number(int i) const {
        CHECK(i >= 0 && i < size_);

        return numbers_[Find(i)] + 1;
    }

    int size()   const { return size_;      }
    bool empty() const { return size_ == 0; }

private:
    int size_;                   // The number of elements.
    Array<int> numbers_;         // The size of disjoint subsets.
    mutable Array<int> subsets_; // The subset id of each element.
};

} // namespace cl

#endif // CODELIBRARY_UTIL_SET_DISJOINT_SET_H_
