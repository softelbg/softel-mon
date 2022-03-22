/*
 * math.cc
 *
 *  Created on: Mar 21, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <softel/core/common.h>

#include <softel/math/Matrix.h>

using namespace std;
using namespace SF::MATH;

int main(int argc, char *argv[]) {

    DLOG << "Built " << __DATE__ << "::" << __TIME__ << std::endl;

    // Test Mult
    Matrix m1(2, 4);
    m1.setRow({1, 5, 3, 4}, 0);
    m1.setRow({2, 1, 4, 2}, 1);

    Matrix m2(4, 2);
    m2.setCol({2, 4, 1, 3}, 0);
    m2.setCol({3, 5, 2, 1}, 1);

    Matrix m3 = m1;
    m3.mult(m2);

    DLOG << "Test Mult" << std::endl << m1.toString() << " * " << std::endl << m2.toString() << " = " << std::endl << m3.toString() << std::endl;


    // Test Matrix determinant = -306
    Matrix m4(3, 3);
    m4.setRow({6, 1, 1}, 0);
    m4.setRow({4, -2, 5}, 1);
    m4.setRow({2, 8, 7}, 2);
    double d4 = m4.det();
    assert(d4 == -306);
    DLOG << m4.toString() << "Det = " << d4 << std::endl;


    // Test Inverse
    Matrix m5(3, 3);
    m5.setRow({-1, -2, 2}, 0);
    m5.setRow({2, 1, 1}, 1);
    m5.setRow({3, 4, 5}, 2);
    Matrix m6 = m5;
    m6.inverse();
    Matrix m7 = m5;
    m7.mult(m6);
    m7.norm();
    DLOG << " Test Inversion " << std::endl << m5.toString() << std::endl;
    DLOG << " Inverse " << std::endl << m6.toString() << std::endl << m7.toString() << std::endl;


    return 0;
}




