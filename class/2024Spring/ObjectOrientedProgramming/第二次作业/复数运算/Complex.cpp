#include"Complex.h"
#include<cmath>

inline long double abs(const long double a){return a>0?a:-a;}

Complex::Complex(long double Re,long double Im):Re(Re),Im(Im){}

Complex Complex::operator+(const Complex &other)const
{
    return Complex(Re+other.Re,Im+other.Im);
}

Complex Complex::operator-(const Complex & other)const
{
    return Complex(Re-other.Re,Im-other.Im);
}

Complex Complex::operator*(const Complex & other)const
{
    return Complex(Re*other.Re-Im*other.Im,Re*other.Im+Im*other.Re);
}

Complex Complex::operator/(const Complex & other)const
{
    return (*this)*(~other)/(other.Re*other.Re+other.Im*other.Im);
}

Complex Complex::operator/(const long double other)const
{
    return Complex(Re/other,Im/other);
}

Complex Complex::operator~()const
{
    return Complex(Re,-Im);
}

Complex Complex::operator-()const
{
    return Complex(-Re,-Im);
}

std::ostream & operator<<(std::ostream &os,const Complex &c)
{
    static const long double EPS=1e-5;
    os.setf(std::ios::fixed);
    os.setf(std::ios::showpoint);
    os.precision(2);
    bool r=(abs(c.Re)>=EPS),i=(abs(c.Im)>=EPS);
    // std::cout<<"outing "<<c.Re<<" "<<c.Im<<std::endl;
    if(r && i) os<<c.Re<<(c.Im>0? " + ":" - ")<<abs(c.Im)<<"i";
    else if(r && !i)    os<<c.Re;
    else if(!r && i)    os<<c.Im<<"i";
    else os<<"0.00";
    return os;
}

Complex operator+(const long double a,const Complex &b)
{
    return Complex(a+b.Re,b.Im);
}

Complex operator-(const long double a,const Complex &b)
{
    return Complex(a-b.Re,-b.Im);
}

Complex operator*(const long double a,const Complex &b)
{
    return Complex(a)*b;
}

Complex operator/(const long double a,const Complex &b)
{
    return Complex(a)/b;
}

long double abs(const Complex &c)
{
    return std::sqrt(c.Re*c.Re+c.Im*c.Im);
}

Complex operator"" i(unsigned long long Im)
{
    return Complex(0,Im);
}

Complex operator"" i(long double Im)
{
    return Complex(0,Im);
}