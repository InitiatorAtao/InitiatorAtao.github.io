#include<bits/stdc++.h>
#include"iterator.h"

int node::create=0;
int node::destroy=0;

int main()
{
    {
        Container<int> ci;
        Container<node> cn;
        int n;
        std::cin>>n;
        for(int i=0;i<n;++i)
        {
            int v=0;
            std::cin>>v;
            ci.push_back(v);
            cn.push_back(node(v));
        }
        for(Iterator<int> i=ci.begin();i!=ci.end();++i)
            std::cout<<(*i)<<" ";
        std::cout<<std::endl;
        for(Iterator<node> i=cn.begin();i!=cn.end();++i)
            std::cout<<(*i).get_value()<<" ";
    }
    std::cout<<std::endl<<node::get_delta()<<std::endl;
    return 0;
}