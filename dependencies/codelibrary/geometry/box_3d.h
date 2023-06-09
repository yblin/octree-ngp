﻿//
// Copyright 2013-2022 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_GEOMETRY_BOX_3D_H_
#define CODELIBRARY_GEOMETRY_BOX_3D_H_

#include <algorithm>
#include <limits>
#include <ostream>

#include "codelibrary/base/log.h"
#include "codelibrary/base/message.h"

namespace cl {

/**
 * 3D axis-aligned minimum bounding box.
 *
 * The axis-aligned minimum bounding box for a given point set is its minimum
 * bounding box subject to the constraint that the edges of the box are parallel
 * to the (Cartesian) coordinate axis.
 */
template <typename T>
class Box3D {
public:
    using value_type = T;

    /**
     * The default box is an invalid box.
     */
    Box3D() = default;

    /**
     * Require lower bounds no greater than upper bounds.
     */
    Box3D(const T& x_min, const T& x_max, const T& y_min, const T& y_max,
          const T& z_min, const T& z_max)
        : x_min_(x_min), x_max_(x_max),
          y_min_(y_min), y_max_(y_max),
          z_min_(z_min), z_max_(z_max) {
        CHECK(x_min_ <= x_max_ && y_min_ <= y_max_ && z_min_ <= z_max_);
    }

    template <typename Iterator>
    Box3D(Iterator first, Iterator last) {
        for (Iterator i = first; i != last; ++i) {
            x_min_ = std::min(x_min_, (*i)[0]);
            x_max_ = std::max(x_max_, (*i)[0]);
            y_min_ = std::min(y_min_, (*i)[1]);
            y_max_ = std::max(y_max_, (*i)[1]);
            z_min_ = std::min(z_min_, (*i)[2]);
            z_max_ = std::max(z_max_, (*i)[2]);
        }
    }

    /**
     * Return true if this box is invalid.
     */
    bool empty() const {
        return x_min_ > x_max_ || y_min_ > y_max_ || z_min_ > z_max_;
    }

    bool operator ==(const Box3D& rhs) const {
        return x_min_ == rhs.x_min_ && x_max_ == rhs.x_max_ &&
               y_min_ == rhs.y_min_ && y_max_ == rhs.y_max_ &&
               z_min_ == rhs.z_min_ && z_max_ == rhs.z_max_;
    }

    bool operator !=(const Box3D& rhs) const {
        return x_min_ != rhs.x_min_ || x_max_ != rhs.x_max_ ||
               y_min_ != rhs.y_min_ || y_max_ != rhs.y_max_ ||
               z_min_ != rhs.z_min_ || z_max_ != rhs.z_max_;
    }

    /**
     * Return the X length of box.
     *
     * Note that x_length may be negative if x_max_ and x_min_ are extreme.
     */
    T x_length() const {
        return empty() ? 0 : x_max_ - x_min_;
    }

    /**
     * Return the Y length of box.
     *
     * Note that y_length may be negative if y_max_ and y_min_ are extreme.
     */
    T y_length() const {
        return empty() ? 0 : y_max_ - y_min_;
    }

    /**
     * Return the Z length of box.
     *
     * Note that z_length may be negative if z_max_ and z_min_ are extreme.
     */
    T z_length() const {
        return empty() ? 0 : z_max_ - z_min_;
    }

    /**
     * The lower x value of box.
     */
    const T& x_min() const {
        return x_min_;
    }

    /**
     * The upper x value of box.
     */
    const T& x_max() const {
        return x_max_;
    }

    /**
     * The lower y value of box.
     */
    const T& y_min() const {
        return y_min_;
    }

    /**
     * The upper y value of box.
     */
    const T& y_max() const {
        return y_max_;
    }

    /**
     * The lower z value of box.
     */
    const T& z_min() const {
        return z_min_;
    }

    /**
     * The upper z value of box.
     */
    const T& z_max() const {
        return z_max_;
    }

    /**
     * Return the bounding box of this box (itself).
     */
    const Box3D& bounding_box() const {
        return *this;
    }

    /**
     * Join this box with another box, the result is the hull of two boxes.
     */
    void Join(const Box3D& box) {
        x_min_ = std::min(x_min_, box.x_min_);
        y_min_ = std::min(y_min_, box.y_min_);
        z_min_ = std::min(z_min_, box.z_min_);
        x_max_ = std::max(x_max_, box.x_max_);
        y_max_ = std::max(y_max_, box.y_max_);
        z_max_ = std::max(z_max_, box.z_max_);
    }

    /**
     * Return the minimum value of the i-th dimension.
     */
    T min(int i) const {
        CHECK(i >= 0 && i < 3);
        return i == 0 ? x_min_ : (i == 1 ? y_min_ : z_min_);
    }

    /**
     * Return the maximum value of the i-th dimension.
     */
    T max(int i) const {
        CHECK(i >= 0 && i < 3);
        return i == 0 ? x_max_ : (i == 1 ? y_max_ : z_max_);
    }

    friend std::ostream& operator <<(std::ostream& os, const Box3D& b) {
        Message msg(b.x_min(), b.x_max(), b.y_min(), b.y_max(),
                    b.z_min(), b.z_max());
        return os << msg;
    }

private:
    T x_min_ = std::numeric_limits<T>::max();    // The lower x value of box.
    T x_max_ = std::numeric_limits<T>::lowest(); // The upper x value of box.
    T y_min_ = std::numeric_limits<T>::max();    // The lower y value of box.
    T y_max_ = std::numeric_limits<T>::lowest(); // The upper y value of box.
    T z_min_ = std::numeric_limits<T>::max();    // The lower z value of box.
    T z_max_ = std::numeric_limits<T>::lowest(); // The upper z value of box.
};

using IBox3D = Box3D<int>;
using FBox3D = Box3D<float>;
using RBox3D = Box3D<double>;

} // namespace cl

#endif // CODELIBRARY_GEOMETRY_BOX_3D_H_
