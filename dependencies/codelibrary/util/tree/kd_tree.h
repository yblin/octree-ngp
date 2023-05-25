/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
 * Copyright 2011-2021  Jose Luis Blanco (joseluisblancoc@gmail.com).
 *   All rights reserved.
 *
 * THE BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

//
// The following code is modified from nanoflann.
//

#ifndef CODELIBRARY_UTIL_TREE_KD_TREE_H_
#define CODELIBRARY_UTIL_TREE_KD_TREE_H_

#include <algorithm>
#include <cfloat>
#include <cmath>

#include "codelibrary/base/array.h"
#include "codelibrary/base/pool.h"
#include "codelibrary/util/metric/squared_euclidean.h"

namespace cl {

/**
 * KD Tree.
 *
 * KD tree is a space-partitioning data structure for organizing points in a
 * K-Dimensional space.
 */
template <typename Point, typename metric = metric::SquaredEuclidean>
class KDTree {
    using T = typename Point::value_type;

    // The maximum number of points at leaf.
    static const int MAX_LEAF_SIZE = 10;

    // Bounding box.
    struct BoundingBox {
        BoundingBox() = default;

        template <typename Iterator>
        BoundingBox(Iterator first, Iterator last) {
            if (first == last) return;

            int size = first->size();
            CHECK(size >= 0);

            min_values.resize(size);
            max_values.resize(size);

            Iterator iter = first;
            for (int i = 0; i < size; ++i) {
                min_values[i] = max_values[i] = (*iter)[i];
            }

            for (++iter; iter != last; ++iter) {
                CHECK(iter->size() == size);

                for (int i = 0; i < size; ++i) {
                    min_values[i] = std::min(min_values[i], (*iter)[i]);
                    max_values[i] = std::max(max_values[i], (*iter)[i]);
                }
            }
        }

        Array<T> min_values, max_values;
    };

    /**
     * A result-set class used when performing a k-nearest Neighbors based
     * search.
     */
    struct KNNResultSet {
        explicit KNNResultSet(int k, double radius = DBL_MAX)
            : capacity(k),
              count(0),
              indices(k, -1),
              distances(k, radius) {}

        /**
         * If the distance from the query point to the given point is smaller
         * than the distances to the other exist points, then add this point
         * into result set.
         *
         *  index    - the index of point in the KD tree.
         *  distance - the distance from the query point to this point.
         */
        void AddPoint(int index, double distance) {
            if (distance >= distances.back()) return;

            int i;
            for (i = count; i > 0; --i) {
                if (distance < distances[i - 1]) {
                    if (i < capacity) {
                        distances[i] = distances[i - 1];
                        indices[i] = indices[i - 1];
                    }
                } else {
                    break;
                }
            }

            if (i < capacity) {
                distances[i] = distance;
                indices[i] = index;
            }

            if (count < capacity) ++count;
        }

        /**
         * Return the current farthest distance in the result set.
         */
        double farthest_distance() const {
            return distances.back();
        }

        int capacity;
        int count;
        Array<int> indices;
        Array<double> distances;
    };

    /**
     * A result-set class used when performing a radius based search.
     */
    struct RadiusResultSet {
        explicit RadiusResultSet(double fixed_radius)
            : radius(fixed_radius) {}

        /**
         * If the distance from the query point to the given point is smaller
         * than a fixed radius, then add this point into result set.
         *
         *  index    - the index of point in the KD tree.
         *  distance - the distance from the query point to this point.
         */
        void AddPoint(int index, double distance) {
            if (distance <= radius) {
                indices.push_back(index);
            }
        }

        /**
         * Return the farthest distance in result set.
         */
        double farthest_distance() const {
            return radius;
        }

        double radius;
        Array<int> indices;
    };

    // Internal Leaf data.
    struct LeafData {
        int left, right; // Indices of points in leaf node.
    };

    // Internal Brench data.
    struct BrenchData {
        int div_dimension;   // Dimension used for subdivision.
        T div_low, div_high; // The values used for subdivision.
    };

    // Use union to save memory.
    union LeafOrBrench {
        LeafData leaf;
        BrenchData brench;
    };

public:
    // The Node for KDTree.
    // We set the Node to public to help access the KD tree.
    struct Node {
        LeafOrBrench leaf_or_brench;
        Node* left_child;  // Left child pointer.
        Node* right_child; // Right child pointer.
    };

    /**
     * KDTree constructor.
     */
    KDTree() = default;

    template <typename Iterator>
    KDTree(Iterator first, Iterator last)
        : points_(first, last) {
        size_ = points_.size();

        Build();
    }

    explicit KDTree(const Array<Point>& points)
        : KDTree(points.begin(), points.end()) {}

    KDTree(const KDTree&) = delete;

    KDTree& operator=(const KDTree&) = delete;

    /**
     * Reset the input points and clear the current KD tree.
     */
    void ResetPoints(const Array<Point>& points) {
        clear();
        points_ = points;
        size_ = points_.size();

        Build();
    }

    /**
     * Swap the KD tree points and rebuild KD tree.
     */
    void SwapPoints(Array<Point>* points) {
        CHECK(points);

        points_.swap(*points);
        size_ = points_.size();

        Build();
    }

    /**
     * Clear the KD-tree.
     */
    void clear() {
        root_node_ = nullptr;
        size_ = 0;
        dimension_ = 0;
        points_.clear();
        indices_.clear();
        pool_.clear();
    }

    /**
     * Find the index of the nearest neighbor point in the KD tree to the given
     * point.
     */
    int FindNearestIndex(const Point& p) const {
        // KD tree can not be empty.
        CHECK(!empty());

        Array<double> distances(dimension_, 0.0);
        double distance_sqr;
        ComputeInitialDistances(p, &distance_sqr, &distances);

        KNNResultSet results(1);
        SearchLevel(root_node_, p, distance_sqr, &distances, &results);
        CHECK(results.count == 1);

        return results.indices[0];
    }

    /**
     * Find the nearest neighbor point in the KD tree to the given point.
     */
    Point FindNearestNeighbor(const Point& p) const {
        int index = FindNearestIndex(p);
        return points_[index];
    }

    /**
     * Find the k-nearest neighbors of the given point.
     */
    void FindKNearestNeighbors(const Point& p, int k,
                               Array<int>* neighbors) const {
        CHECK(neighbors);
        CHECK(k > 0 && k <= size_);

        Array<double> distances(dimension_, 0.0);
        double distance_sqr;
        ComputeInitialDistances(p, &distance_sqr, &distances);

        KNNResultSet results(k);
        SearchLevel(root_node_, p, distance_sqr, &distances, &results);

        neighbors->swap(results.indices);
        CHECK(neighbors->size() == k);
    }

    /**
     * Find the k-nearest neighbors of the given point.
     */
    void FindKNearestNeighbors(const Point& p, int k,
                               Array<Point>* neighbors) const {
        CHECK(neighbors);

        Array<int> indices;
        FindKNearestNeighbors(p, k, &indices);

        neighbors->resize(indices.size());
        for (int i = 0; i < indices.size(); ++i) {
            (*neighbors)[i] = points_[indices[i]];
        }
    }

    /**
     * Find the k-nearest neighbors within given radius of the given point.
     */
    void FindKNearestInRadiusNeighbors(const Point& p, int k, double radius,
                                       Array<int>* neighbors) const {
        CHECK(neighbors);
        CHECK(k > 0 && k <= size_);
        CHECK(radius > 0.0);

        Array<double> distances(dimension_, 0.0);
        double distance_sqr;
        ComputeInitialDistances(p, &distance_sqr, &distances);

        KNNResultSet results(k, radius);
        SearchLevel(root_node_, p, distance_sqr, &distances, &results);

        neighbors->swap(results.indices);
        neighbors->resize(results.count);
    }

    /**
     * Find the k-nearest neighbors within given radius of given point.
     */
    void FindKNearestInRadiusNeighbors(const Point& p, int k, double radius,
                                       Array<Point>* neighbors) const {
        CHECK(neighbors);

        Array<int> indices;
        FindKNearestInRadiusNeighbors(p, k, radius, &indices);

        neighbors->resize(indices.size());
        for (int i = 0; i < indices.size(); ++i) {
            (*neighbors)[i] = points_[indices[i]];
        }
    }

    /**
     * Find the fixed-radius neighbors of given point.
     */
    void FindRadiusNeighbors(const Point& p, double radius,
                             Array<int>* neighbors) const {
        CHECK(neighbors);
        CHECK(!empty());

        Array<double> distances(dimension_, 0.0);
        double distance_sqr;
        ComputeInitialDistances(p, &distance_sqr, &distances);

        RadiusResultSet results(radius);
        SearchLevel(root_node_, p, distance_sqr, &distances, &results);

        neighbors->swap(results.indices);
    }

    /**
     * Find the fixed-radius neighbors of the given point.
     */
    void FindRadiusNeighbors(const Point& p, double radius,
                             Array<Point>* neighbors) const {
        CHECK(neighbors);

        Array<int> indices;
        FindRadiusNeighbors(p, radius, &indices);

        neighbors->resize(indices.size());
        for (int i = 0; i < indices.size(); ++i) {
            (*neighbors)[i] = points_[indices[i]];
        }
    }

    int size()                   const { return size_;      }
    bool empty()                 const { return size_ == 0; }
    const metric& distance()     const { return distance_;  }
    const Array<Point>& points() const { return points_;    }
    const Node* root_node()      const { return root_node_; }

private:
    /**
     * Build the KD tree.
     */
    void Build() {
        if (empty()) return;

        dimension_ = points_[0].size();
        CHECK(dimension_ > 0);

        pool_.clear();
        indices_.resize(size_);
        for (int i = 0; i < size_; ++i) {
            indices_[i] = i;
        }
        box_ = BoundingBox(points_.begin(), points_.end());

        root_node_ = DivideTree(0, size_, box_);
    }

    /**
     * Compute initial distances.
     *
     * Parameters:
     *  p            - the point for searching.
     *  distance_sqr - the square of distance between point to bounding box.
     *  distances    - the distance between point to each dimension of bounding
     *                 box (need initialized before).
     */
    void ComputeInitialDistances(const Point& p, double* distance_sqr,
                                 Array<double>* distances) const {
        *distance_sqr = 0.0;

        for (int i = 0; i < dimension_; ++i) {
            if (p[i] < box_.min_values[i]) {
                (*distances)[i] = (p[i] - box_.min_values[i]) *
                                  (p[i] - box_.min_values[i]);
                *distance_sqr += (*distances)[i];
            }
            if (p[i] > box_.max_values[i]) {
                (*distances)[i] = (p[i] - box_.max_values[i]) *
                                  (p[i] - box_.max_values[i]);
                *distance_sqr += (*distances)[i];
            }
        }
    }

    /**
     * Perform an exact search in the tree starting from a node.
     */
    template <typename ResultSet>
    void SearchLevel(const Node* node, const Point& query_point,
                     double min_distance_sqr, Array<double>* distances,
                     ResultSet* results) const {
        // If this this is a leaf node, then do check and return.
        if (node->left_child == nullptr && node->right_child == nullptr) {
            for (int i = node->leaf_or_brench.leaf.left;
                 i < node->leaf_or_brench.leaf.right; ++i) {
                double dis = distance_(points_[indices_[i]], query_point);
                if (dis < results->farthest_distance())
                    results->AddPoint(indices_[i], dis);
            }
            return;
        }

        // Find which child branch should be taken first.
        int d = node->leaf_or_brench.brench.div_dimension;
        T value = query_point[d];
        double diff1 = value - node->leaf_or_brench.brench.div_low;
        double diff2 = value - node->leaf_or_brench.brench.div_high;

        Node* best_child;
        Node* other_child;
        double cut_distance;
        if ((diff1 + diff2) < 0.0) {
            best_child = node->left_child;
            other_child = node->right_child;
            double t = value - node->leaf_or_brench.brench.div_high;
            cut_distance = t * t;
        } else {
            best_child = node->right_child;
            other_child = node->left_child;
            double t = value - node->leaf_or_brench.brench.div_low;
            cut_distance = t * t;
        }

        // Call recursively to search next level down.
        SearchLevel(best_child, query_point, min_distance_sqr, distances,
                    results);

        double distance = (*distances)[d];

        // Compute the square of distance between search point to the bounding
        // box of the points in other_child.
        double lower_bound = min_distance_sqr + cut_distance - distance;

        (*distances)[d] = cut_distance;
        if (lower_bound <= results->farthest_distance()) {
            SearchLevel(other_child, query_point, lower_bound, distances,
                        results);
        }
        (*distances)[d] = distance;
    }

    /**
     * Create a tree node that subdivides the list of vertices from
     * indices[left, right).
     *
     * The routine is called recursively on each sublist.
     */
    Node* DivideTree(int left, int right, const BoundingBox& box) {
        Node* node = pool_.Allocate();

        // If too few exemplars remain, then make this a leaf node.
        if ((right - left) <= MAX_LEAF_SIZE) {
            // Mark as leaf node.
            node->left_child = node->right_child = nullptr;
            node->leaf_or_brench.leaf.left = left;
            node->leaf_or_brench.leaf.right = right;
        } else {
            int cut_index;
            int cut_dimension;
            T cut_value;
            MiddleSplit(left, right, box, &cut_index, &cut_dimension,
                        &cut_value);

            node->leaf_or_brench.brench.div_dimension = cut_dimension;

            BoundingBox left_box(box);
            left_box.max_values[cut_dimension] = cut_value;
            node->left_child = DivideTree(left, cut_index, left_box);

            BoundingBox right_box(box);
            right_box.min_values[cut_dimension] = cut_value;
            node->right_child = DivideTree(cut_index, right, right_box);

            node->leaf_or_brench.brench.div_low =
                    left_box.max_values[cut_dimension];
            node->leaf_or_brench.brench.div_high =
                    right_box.min_values[cut_dimension];
        }

        return node;
    }

    /**
     * Compute the minimum and maximum element value of k-th component of
     * points in [left, right).
     */
    void ComputeMinMax(int left, int right, int k, T* min_elem, T* max_elem) {
        *min_elem = *max_elem = points_[indices_[left]][k];
        for (int i = left + 1; i < right; ++i) {
            T val = points_[indices_[i]][k];
            *min_elem = std::min(*min_elem, val);
            *max_elem = std::max(*max_elem, val);
        }
    }

    /**
     * Middle split the vertices from indices[left] to indices[right].
     *
     * Parameters:
     *  left      - the first index to split.
     *  right     - the last index to split.
     *  box       - the approximate bounding box of points[left, right).
     *  index     - the index in [left, right) for splitting.
     *  cut_feat  - the dimension to split.
     *  cut_value - the position of split.
     */
    void MiddleSplit(int left, int right, const BoundingBox& box,
                     int* cut_index, int* cut_dimension, T* cut_value) {
        // Find the largest span from the approximate bounding box.
        T max_span = box.max_values[0] - box.min_values[0];
        *cut_dimension = 0;
        for (int i = 1; i < dimension_; ++i) {
            T span = box.max_values[i] - box.min_values[i];
            if (span > max_span) {
                max_span = span;
                *cut_dimension = i;
            }
        }

        // Compute exact span on the found dimension.
        T min_elem, max_elem;
        ComputeMinMax(left, right, *cut_dimension, &min_elem, &max_elem);
        *cut_value = (min_elem + max_elem) / 2;
        max_span = max_elem - min_elem;

        // Check if a dimension of a largest span exists.
        for (int i = 0; i < dimension_; ++i) {
            if (i == *cut_dimension) continue;
            T span = box.max_values[i] - box.min_values[i];
            if (span > max_span) {
                ComputeMinMax(left, right, i, &min_elem, &max_elem);
                span = max_elem - min_elem;
                if (span > max_span) {
                    max_span = span;
                    *cut_dimension = i;
                    *cut_value = (min_elem + max_elem) / 2;
                }
            }
        }

        int lim1, lim2;
        PlaneSplit(left, right, *cut_dimension, *cut_value, &lim1, &lim2);

        int count = right - left;
        if (lim1 > left + count / 2) {
            *cut_index = lim1;
        } else if (lim2 < left + count / 2) {
            *cut_index = lim2;
        } else {
            // If either list is empty, it means that all remaining features are
            // identical. Split in the middle to maintain a balanced tree.
            *cut_index = left + count / 2;
        }
    }

    /**
     * Subdivide the list of points by a plane perpendicular on axis
     * corresponding to the 'cut_dimension' dimension at 'cut_value' position.
     *
     * Out parameters:
     *   points[indices[left      .. left+lim1-1]][cut_dimension] < cut_value;
     *   points[indices[left+lim1 .. left+lim2-1]][cut_dimension] = cut_value;
     *   points[indices[left+lim2 .. right]][cut_dimension]       > cut_value.
     */
    void PlaneSplit(int left, int right, int cut_dimension, const T& cut_value,
                    int* lim1, int* lim2) {
        int l = left, r = right - 1;

        // Move vector indices for left subtree to front of list.
        for (;;) {
            while (l <= r &&
                   points_[indices_[l]][cut_dimension] < cut_value) ++l;
            while (r != 0 && l <= r &&
                   points_[indices_[r]][cut_dimension] >= cut_value) --r;
            if (l > r || r == 0) break;
            std::swap(indices_[l], indices_[r]);
            ++l;
            --r;
        }

        *lim1 = l;
        r = right - 1;
        for (;;) {
            while (l <= r &&
                   points_[indices_[l]][cut_dimension] <= cut_value) ++l;
            while ((r != 0) && l <= r &&
                   points_[indices_[r]][cut_dimension] > cut_value) --r;
            if (l > r || r == 0) break;
            std::swap(indices_[l], indices_[r]);
            ++l;
            --r;
        }
        *lim2 = l;
    }

    int size_ = 0;              // The number of points.
    int dimension_ = 0;         // The dimension of the KD tree.
    Node* root_node_ = nullptr; // The pointer to the root of tree.
    BoundingBox box_;           // The bounding box of points.
    metric distance_;           // The distance metric.
    Array<Point> points_;       // The points data of KD tree.
    Array<int> indices_;        // The indices of the points.
    Pool<Node> pool_;           // The memory pool for nodes of KD tree.
};

} // namespace cl

#endif // CODELIBRARY_UTIL_TREE_KD_TREE_H_
