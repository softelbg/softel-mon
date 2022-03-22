/*
 * Matrix.cc
 *
 *  Created on: Mar 21, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/math/Matrix.h>

using namespace std;

namespace SF {
namespace MATH {

Matrix::Matrix(size_t r, size_t c)
: r_(r), c_(c), e_(0.00001)
{
    m_.resize(r);

    for (size_t i = 0; i < r; i++) {
        m_[i].resize(c);
    }
}

Matrix::~Matrix() {
    // TODO Auto-generated destructor stub
}

std::string Matrix::toString() const {

    std::stringstream ss;

    for (size_t r = 0; r < r_; r++) {
        for (size_t c = 0; c < c_ - 1; c++) {
            ss << m_[r][c] << " ";
        }
        ss << m_[r][c_ - 1] << std::endl;
    }

    return ss.str();
}

void Matrix::set(size_t r, size_t c, TMatrixVal val) {
    m_[r][c] = val;
}

TMatrixVal Matrix::get(size_t r, size_t c) const {
    return m_[r][c];
}

void Matrix::add(const Matrix& m) {
    assert(m.r_ == r_ && m.c_ == c_);

    for (size_t r = 0; r < r_; r++) {
        for (size_t c = 0; c < c_; c++) {
            m_[r][c] += m.m_[r][c];
        }
    }
}

void Matrix::sub(const Matrix& m) {
    assert(m.r_ == r_ && m.c_ == c_);

    for (size_t r = 0; r < r_; r++) {
        for (size_t c = 0; c < c_; c++) {
            m_[r][c] -= m.m_[r][c];
        }
    }
}

void Matrix::transpose() {

    size_t r2 = c_;
    size_t c2 = r_;

    Matrix m2(r2, c2);

    for (size_t r = 0; r < r_; r++) {
        for (size_t c = 0; c < c_; c++) {
            m2.m_[c][r] = m_[r][c];
        }
    }

    set(m2);
}

void Matrix::inverse() {
    TMatrixVal d = det();
    assert(d != 0);

    DLOGF << "-------" << std::endl << toString() << "-------" << std::endl;
    DLOGF << "det = " << d << std::endl;

    Matrix cof(r_, c_);
    cofactor(cof);
    DLOGF << "----  cofactor  ---" << std::endl << cof.toString() << "-------" << std::endl;
    cof.transpose();
    DLOGF << "-------" << std::endl << cof.toString() << "-------" << std::endl;

    d = 1 / d;
    DLOGF << "1 / det = " << d << std::endl;
    cof.mult(d);
    DLOGF << "-------" << std::endl << cof.toString() << "-------" << std::endl;

    m_ = cof.m_;
}

void Matrix::mult(TMatrixVal m) {
    for (size_t r = 0; r < r_; r++) {
        for (size_t c = 0; c < c_; c++) {
            m_[r][c] *= m;
        }
    }
}

void Matrix::mult(const Matrix& m) {
    assert(c_ == m.r_ && r_ == m.c_);

    Matrix m2(r_, r_);

    for (size_t rr = 0; rr < r_; rr++) {
        for (size_t cc = 0; cc < r_; cc++) {
            m2.m_[rr][cc] = 0;
            for (size_t r2 = 0; r2 < c_; r2++) {
                m2.m_[rr][cc] += m_[rr][r2] * m.m_[r2][cc];
            }
        }
    }

    set(m2);
}

void Matrix::set(const Matrix& m) {
    m_ = m.m_;
    r_ = m.r_;
    c_ = m.c_;
}

TMatrixVal Matrix::det() {
    assert(isSquare());

    return det(*this);
}

TMatrixVal Matrix::det(const Matrix& m) {
    assert(m.isSquare());

    //DLOG << "Calculate det of matrix -------------" << std::endl << m.toString() << std::endl << "----------" << std::endl;

    if (m.r_ == 2) {
        return m.m_[0][0] * m.m_[1][1] - m.m_[1][0] * m.m_[0][1];
    }

    TMatrixVal d = 0;

    // TODO: make proper choice of these...
    size_t r = 0;

    for (size_t c = 0; c < m.c_; c++) {
        d += m.adj(r, c) * m.m_[r][c];
    }

    return d;
}

void Matrix::setRow(const std::vector<TMatrixVal>& row, size_t r) {
    assert(r < r_);
    assert(row.size() == c_);

    m_[r] = row;
}

void Matrix::setCol(const std::vector<TMatrixVal>& col, size_t c) {
    assert(c < c_ && col.size() == r_);

    for (size_t r = 0; r < r_; r++) {
        m_[r][c] = col[r];
    }
}

TMatrixVal Matrix::adj(size_t r, size_t c) const {
    Matrix m2(r_ - 1, c_ - 1);

    for (size_t r2 = 0; r2 < m2.r_; r2++) {
        for (size_t c2 = 0; c2 < m2.c_; c2++) {
            size_t cp = 0;
            size_t rp = 0;
            if (c2 >= c) {
                cp = 1;
            }
            if (r2 >= r) {
                rp = 1;
            }

            m2.m_[r2][c2] = m_[r2 + rp][c2 + cp];
        }
    }

    int sgn = (r + c) % 2 == 0 ? 1 : -1;
    double d = det(m2);

    //DLOGF << "ADJ[" << r << "][" << c << "] sgn = " << sgn << " det = " << d << " m = " << m_[r][c] << std::endl;

    return sgn * d;
}

void Matrix::cofactor(Matrix& m) const {
    assert(m.isSquare());
    assert(m.r_ == r_);

    for (size_t r = 0; r < m.r_; r++) {
        for (size_t c = 0; c < m.c_; c++) {
            m.m_[r][c] = adj(r, c);
        }
    }
}

bool Matrix::isSquare() const {
    return r_ == c_ && r_ > 1;
}

void Matrix::norm() {
    for (size_t r = 0; r < r_; r++) {
        for (size_t c = 0; c < c_; c++) {
            int m1 = (int) ceil(m_[r][c]) ;
            int m2 = (int) floor(m_[r][c]);
            if (m_[r][c] - m2 < e_) {
                m_[r][c] = m2;
            }
            if (m1 - m_[r][c] < e_) {
                m_[r][c] = m1;
            }
        }
    }
}

} /* namespace MATH */
} /* namespace SF */
