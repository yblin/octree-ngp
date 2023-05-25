﻿//
// Copyright 2017-2021 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_MATH_NUMBER_DETERMINANT_H_
#define CODELIBRARY_MATH_NUMBER_DETERMINANT_H_

namespace cl {

/**
 * Compute the determinant of a 2*2 matrix.
 */
template <typename T>
T Determinant(const T& a00,  const T& a01, const T& a10,  const T& a11) {
    // We use explicit expressions to eliminate the extra copy constructions.
    T t = a10 * a01; // It is the RVO (return value optimization) trick.

    T det = a00 * a11;
    det -= t; // Faster than det = t1 - t2 for BigInt type.
    return det;
}

/**
 * Compute the determinant of a 3*3 matrix.
 */
template <class T>
T Determinant(const T& a00,  const T& a01,  const T& a02,
              const T& a10,  const T& a11,  const T& a12,
              const T& a20,  const T& a21,  const T& a22) {
    // We minimize the internal parameters for speed.
    T det = Determinant(a00, a01, a10, a11);
    T m02 = Determinant(a00, a01, a20, a21);
    T m12 = Determinant(a10, a11, a20, a21);

    // m01 * a22 - m02 * a12 + m12 * a02
    det *= a22;
    m02 *= a12;
    m12 *= a02;
    det += m12;
    det -= m02;
    return det;
}

/**
 * Compute the determinant of a 4*4 matrix.
 */
template <typename T>
T Determinant(const T& a00,  const T& a01,  const T& a02,  const T& a03,
              const T& a10,  const T& a11,  const T& a12,  const T& a13,
              const T& a20,  const T& a21,  const T& a22,  const T& a23,
              const T& a30,  const T& a31,  const T& a32,  const T& a33) {
    // First compute the determinant of 2x2 matrices.
    T m01 = Determinant(a10, a11, a00, a01);
    T m02 = Determinant(a20, a21, a00, a01);
    T m03 = Determinant(a30, a31, a00, a01);
    T m12 = Determinant(a20, a21, a10, a11);
    T m13 = Determinant(a30, a31, a10, a11);
    T m23 = Determinant(a30, a31, a20, a21);

    // Now compute the minors of rank 3.
    // tmp1 = m12 * a02 - m02 * a12
    // tmp2 = m01 * a32 - m03 * a12
    // tmp3 = m02 * a32 - m03 * a22
    // tmp4 = m23 * a12 - m13 * a22
    T tmp1 = Determinant(m12, m02, a12, a02);
    T tmp2 = Determinant(m01, m03, a12, a32);
    T tmp3 = Determinant(m02, m03, a22, a32);
    T tmp4 = Determinant(m23, m13, a22, a12);

    // m01 = (m12 * a02 - m02 * a12) + m01 * a22.
    m01 *= a22;
    m01 += tmp1;

    // m13 = (m01 * a32 - m03 * a12) + m13 * a02.
    m13 *= a02;
    m13 += tmp2;

    // m23 = (m02 * a32 - m03 * a22) + m23 * a02.
    m23 *= a02;
    m23 += tmp3;

    // m12 = (m23 * a12 - m13 * a22) + m12 * a32.
    m12 *= a32;
    m12 += tmp4;

    // Now compute the minors of rank 4.
    // det = m12*a03 - m23*a13 + m13*a23 - m01*a33.
    tmp1 = Determinant(m12, m23, a13, a03);
    T det = Determinant(m13, m01, a33, a23);
    det += tmp1;
    return det;
}

} // namespace cl

#endif // CODELIBRARY_MATH_NUMBER_DETERMINANT_H_
