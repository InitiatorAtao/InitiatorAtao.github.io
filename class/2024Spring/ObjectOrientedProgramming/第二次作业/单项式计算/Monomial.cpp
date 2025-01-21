#include<bits/stdc++.h>
#include"Monomial.h"
using namespace std;

Monomial::Monomial()
{
    coe=deg=0;
}

Monomial::Monomial(int c,int d):coe(c),deg(d){}

Monomial Monomial::operator+(const Monomial &x)
{
    //assert(deg==x.deg);
    // cout<<coe<<" "<<x.coe<<" "<<deg<<" "<<x.deg<<endl;
    if(coe==0)
        return Monomial(x.coe,x.deg);
    return Monomial(coe+x.coe,coe+x.coe==0? 0:deg);
}
Monomial Monomial::operator-(const Monomial &x)
{
    // assert(deg==x.deg);
    if(coe==0)
        return Monomial(-x.coe,x.deg);
    return Monomial(coe-x.coe,coe-x.coe==0? 0:deg);
}
Monomial Monomial::operator*(const Monomial &x)
{
    return Monomial(coe*x.coe,coe*x.coe==0? 0:deg+x.deg);
}
Monomial Monomial::operator/(const Monomial &x)
{
    return Monomial(coe/x.coe,coe==0? 0:deg-x.deg);
}
Monomial Monomial::derive()
{
    if(deg==0)
        return Monomial();
    else
        return Monomial(coe*deg,deg-1);
}

int Monomial::definite_integral(const int a,const int b)
{
    Monomial i(coe/(deg+1),deg+1);
    return (i.get_val(b)-i.get_val(a));
}

int Monomial::get_val(int x)
{
    int res=coe;
    for(int i=-1;i>=deg;--i)
        res/=x;
    for(int i=1;i<=deg;++i)
        res*=x;
    return res;
}

ostream & operator<<(ostream &os,const Monomial &x)
{
    os<<x.coe<<"x^"<<x.deg<<endl;
    return os;
}

istream & operator>>(istream &is,Monomial &x)
{
    string rubbish;
    is>>rubbish;
    int px=rubbish.find("x^");
    x.coe=stoi(rubbish.substr(0,px));
    x.deg=stoi(rubbish.substr(px+2));
    // cout<<x.coe<<" "<<x.deg<<endl;
    return is;
}