/*
 * Matrix.h
 *
 *  Simple, not indented to be efficient, Matrix impl...
 *
 *  Created on: Mar 21, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_MATH_MATRIX_H_
#define SOFTEL_MATH_MATRIX_H_

#include <softel/core/common.h>

namespace SF {
namespace MATH {

// consider template this...
typedef double TMatrixVal;

class Matrix {
public:
    Matrix(size_t r, size_t c);
    virtual ~Matrix();

    size_t r() const {
        return r_;
    }
    size_t c() const {
        return c_;
    }

    void set(const Matrix& m);
    void setRow(const std::vector<TMatrixVal>& row, size_t r);
    void setCol(const std::vector<TMatrixVal>& col, size_t c);
    void set(size_t r, size_t c, TMatrixVal val);
    TMatrixVal get(size_t r, size_t c) const;

    void add(const Matrix& m);
    void sub(const Matrix& m);
    void mult(TMatrixVal m);
    void mult(const Matrix& m);
    void transpose();
    void inverse();
    TMatrixVal det();
    static TMatrixVal det(const Matrix& m);

    TMatrixVal adj(size_t r, size_t c) const;
    void cofactor(Matrix& result) const;

    bool isSquare() const;

    void norm();
    std::string toString() const;

private:
    size_t r_;
    size_t c_;

    double e_;

    std::vector<std::vector<TMatrixVal>> m_;
};

} /* namespace MATH */
} /* namespace SF */

#endif /* SOFTEL_MATH_MATRIX_H_ */
