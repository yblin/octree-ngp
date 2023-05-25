﻿//
// Copyright 2014 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_UTIL_METRIC_COSINE_H_
#define CODELIBRARY_UTIL_METRIC_COSINE_H_

#include <cmath>

#include "codelibrary/base/log.h"

namespace cl {
namespace metric {

/**
 * Cosine is a measure of similarity between two vectors of an inner product
 * space that measures the cosine of the angle between them.
 *
 * The range of cosine metric is [-1, 1].
 */
class Cosine {
public:
    Cosine() = default;

    Cosine(const Cosine&) = delete;

    Cosine& operator=(const Cosine&) = delete;

    template <typename T>
    double operator() (const T& a, const T& b) const {
        auto size1 = a.size();
        auto size2 = b.size();
        CHECK(size1 == size2);

        double t0 = 0.0, t1 = 0.0, t2 = 0.0;
        for (decltype(size1) i = 0; i < size1; ++i) {
            t0 += static_cast<double>(a[i]) * b[i];
            t1 += static_cast<double>(a[i]) * a[i];
            t2 += static_cast<double>(b[i]) * b[i];
        }

        return t0 / std::sqrt(t1 * t2);
    }
};

} // namespace metric
} // namespace cl

#endif // CODELIBRARY_UTIL_METRIC_COSINE_H_
