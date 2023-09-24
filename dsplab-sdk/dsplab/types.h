#pragma once

#include <math.h>

namespace DspLab {

//реальные отсчеты
typedef double real_t;

//комплексные отсчеты
struct complex_t
{
    complex_t(real_t _xi = 0, real_t _xq = 0)
    {
        xi = _xi;
        xq = _xq;
    }

    real_t xi;
    real_t xq;

    complex_t& operator+()
    {
        return *this;
    }

    complex_t& operator-()
    {
        xi = -xi;
        xq = -xq;
        return *this;
    }

    complex_t& operator+=(const complex_t& rhs)
    {
        xi += rhs.xi;
        xq += rhs.xq;
        return *this;
    }

    complex_t& operator-=(const complex_t& rhs)
    {
        xi -= rhs.xi;
        xq -= rhs.xq;
        return *this;
    }

    complex_t& operator*=(const complex_t& rhs)
    {
        xi = (xi * rhs.xi) - (xq * rhs.xq);
        xq = (xi * rhs.xq) + (xq * rhs.xi);
        return *this;
    }

    complex_t& operator/=(const complex_t& rhs)
    {
        double b = (rhs.xi * rhs.xi) + (rhs.xq * rhs.xq);
        xi = ((xi * rhs.xi) + (xq * rhs.xq)) / b;
        xq = ((rhs.xi * xq) - (xi * rhs.xq)) / b;
        return *this;
    }

    complex_t operator+(const complex_t& rhs) const
    {
        complex_t tmp = *this;
        tmp += rhs;
        return tmp;
    }

    complex_t operator-(const complex_t& rhs) const
    {
        complex_t tmp = *this;
        tmp -= rhs;
        return tmp;
    }

    complex_t operator*(const complex_t& rhs) const
    {
        complex_t tmp = *this;
        tmp *= rhs;
        return tmp;
    }

    complex_t operator/(const complex_t& rhs) const
    {
        complex_t tmp = *this;
        tmp /= rhs;
        return tmp;
    }

    complex_t& operator*=(const double& rhs)
    {
        xi = (xi * rhs);
        xq = (xq * rhs);
        return *this;
    }

    complex_t operator*(const double& rhs) const
    {
        complex_t tmp = *this;
        tmp *= rhs;
        return tmp;
    }

    complex_t& operator/=(const double& rhs)
    {
        xi = (xi / rhs);
        xq = (xq / rhs);
        return *this;
    }

    complex_t operator/(const double& rhs) const
    {
        complex_t tmp = *this;
        tmp /= rhs;
        return tmp;
    }

    double abs() const
    {
        return ::sqrt((xi * xi) + (xq * xq));
    }

    double fabs() const
    {
        double min, max;

        if (::fabs(xi) > ::fabs(xq)) {
            min = ::fabs(xq);
            max = ::fabs(xi);
        } else {
            min = ::fabs(xi);
            max = ::fabs(xq);
        }

        return max + (min / 2);
    }
};

}   // namespace DspLab
