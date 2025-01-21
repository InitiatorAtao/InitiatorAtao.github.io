#include "node.h"
#include<bits/stdc++.h>
using namespace std;

float Node::get_val()const
{
    if(type==0)
        return val;
    else if(type==1)
        return l->get_val()+r->get_val();
    else if(type==2)
        return l->get_val()-r->get_val();
    else if(type==3)
        return l->get_val()*r->get_val();
    else if(type==4)
        return l->get_val()/r->get_val();
    else if(type==5)
        return val;
}

void Node::backward(float _grad){}

Node operator+(const Node &x,const Node &y)
{
    return Node(1,&x,&y);
}
Node operator-(const Node &x,const Node &y)
{
    return Node(2,&x,&y);
}
Node operator*(const Node &x,const Node &y)
{
    return Node(3,&x,&y);
}
Node operator/(const Node &x,const Node &y)
{
    return Node(4,&x,&y);
}