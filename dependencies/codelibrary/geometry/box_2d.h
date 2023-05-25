﻿//
// Copyright 2011-2022 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_GEOMETRY_BOX_2D_H_
#define CODELIBRARY_GEOMETRY_BOX_2D_H_

#include <algorithm>
#include <limits>
#include <ostream>

#include "codelibrary/base/log.h"
#include "codelibrary/base/message.h"

namespace cl {

/**
 * 2D axis-aligned minimum bounding box.
 *
 * The axis-aligned minimum bounding box for a given point set is its minimum
 * bounding box subject to the constraint that the edges of the box are parallel
 * to the (Cartesian) coordinate axis.
 */
template <typename T>
class Box2D {
public:
    using value_type = T;

    /**
     * The default box is an invalid box.
     */
    Box2D() = default;

    /**
     * Require lower bounds no greater than upper bounds.
     */
    Box2D(const T& x_min, const T& x_max, const T& y_min, const T& y_max)
        : x_min_(x_min), x_max_(x_max), y_min_(y_min), y_max_(y_max) {
        CHECK(x_min_ <= x_max_ && y_min_ <= y_max_);
    }

    /**
     * Compute a smallest bounding box of the given points [first, last).
     */
    template <typename Iterator>
    Box2D(Iterator first, Iterator last) {
        for (Iterator i = first; i != last; ++i) {
            x_min_ = std::min(x_min_, (*i)[0]);
            x_max_ = std::max(x_max_, (*i)[0]);
            y_min_ = std::min(y_min_, (*i)[1]);
            y_max_ = std::max(y_max_, (*i)[1]);
        }
    }

    /**
     * Return true if the box is invalid.
     */
    bool empty() const {
        return x_min_ > x_max_ || y_min_ > y_max_;
    }

    /**
     * The lower x value of box.
     */
    const T& x_min() const {
        return x_min_;
    }

    /**
     * The higher x value of box.
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
     * The higher y value of box.
     */
    const T& y_max() const {
        return y_max_;
    }

    bool operator ==(const Box2D& rhs) const {
        return x_min_ == rhs.x_min_ && x_max_ == rhs.x_max_ &&
               y_min_ == rhs.y_min_ && y_max_ == rhs.y_max_;
    }

    bool operator !=(const Box2D& rhs) const {
        return x_min_ != rhs.x_min_ || x_max_ != rhs.x_max_ ||
               y_min_ != rhs.y_min_ || y_max_ != rhs.y_max_;
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
     * Return the bounding box of this box (itself).
     */
    const Box2D<T>& bounding_box() const {
        return *this;
    }

    /**
     * Join this box with another box, the result is the hull of two boxes.
     */
    void Join(const Box2D& box) {
        x_min_ = std::min(x_min_, box.x_min_);
        y_min_ = std::min(y_min_, box.y_min_);
        x_max_ = std::max(x_max_, box.x_max_);
        y_max_ = std::max(y_max_, box.y_max_);
    }

    /**
     * Return the minimum value of the i-th dimension.
     */
    T min(int i) const {
        CHECK(i >= 0 && i < 2);
        return i == 0 ? x_min_ : y_min_;
    }

    /**
     * Return the maximum value of the i-th dimension.
     */
    T max(int i) const {
        CHECK(i >= 0 && i < 2);
        return i == 0 ? x_max_ : y_max_;
    }

    friend std::ostream& operator <<(std::ostream& os, const Box2D& b) {
        Message msg(b.x_min(), b.x_max(), b.y_min(), b.y_max());
        return os << msg;
    }

protected:
    // The lower x value of box.
    T x_min_ = std::numeric_limits<T>::max();
    // The upper x value of box.
    T x_max_ = std::numeric_limits<T>::lowest();
    // The lower y value of box.
    T y_min_ = std::numeric_limits<T>::max();
    // The upper y value of box.
    T y_max_ = std::numeric_limits<T>::lowest();
};

using IBox2D = Box2D<int>;
using FBox2D = Box2D<float>;
using RBox2D = Box2D<double>;

} // namespace cl

#endif // CODELIBRARY_GEOMETRY_BOX_2D_H_
