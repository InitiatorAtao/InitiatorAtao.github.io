#include"Array.h"

Node& Array::operator[](int i){return vec[i];}

Array::Array(int len)
{
    vec=vector<Node>(len);
}

Array::Array(const Array& other)
{
    vec=other.vec;
}

Array::Array(Array&& other)
{
    vec=move(other.vec);
}

Array& Array::operator=(const Array& other)
{
    vec=other.vec;
    return *this;
}

Array& Array::operator=(Array&& other)
{
    vec=move(other.vec);
    return *this;
}