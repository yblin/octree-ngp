﻿//
// Copyright 2012-2022 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_GEOMETRY_CIRCLE_2D_H_
#define CODELIBRARY_GEOMETRY_CIRCLE_2D_H_

#include "codelibrary/base/log.h"
#include "codelibrary/geometry/box_2d.h"
#include "codelibrary/geometry/point_2d.h"

namespace cl {

/**
 * 2D circle.
 */
template <typename T>
class Circle2D {
public:
    using value_type = T;

    Circle2D() = default;

    Circle2D(const Point2D<T>& center, const T& radius)
        : center_(center), radius_(radius) {
        CHECK(radius_ >= 0);

        bounding_box_ = Box2D<T>(center_.x - radius_, center_.x + radius_,
                                 center_.y - radius_, center_.y + radius_);
    }

    Circle2D(const T& x, const T& y, const T& radius)
        : Circle2D(Point2D<T>(x, y), radius) {}

    /**
     * The radius of the circle.
     */
    const T& radius() const {
        return radius_;
    }

    /**
     * Update radius.
     */
    void set_radius(const T& radius) {
        radius_ = radius;
    }

    /**
     * The bounding box of the circle.
     */
    const Box2D<T>& bounding_box() const {
        return bounding_box_;
    }

    /**
     * The center of the circle.
     */
    const Point2D<T>& center() const {
        return center_;
    }

protected:
    Box2D<T> bounding_box_; // The bounding box of circle.
    Point2D<T> center_;     // The center of circle.
    T radius_ = 0;          // The radius of circle.
};

using ICircle2D = Circle2D<int>;
using FCircle2D = Circle2D<float>;
using RCircle2D = Circle2D<double>;

} // namespace cl

#endif // CODELIBRARY_GEOMETRY_CIRCLE_2D_H_
