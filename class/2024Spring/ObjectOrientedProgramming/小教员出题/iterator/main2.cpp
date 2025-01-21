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
        
        std::cout<<std::endl<<ci.begin().any(ci.end(),
                                            std::function<bool(int)>([](int x){return x%2==0;}))<<std::endl;
        std::cout<<cn.begin().all(cn.end(),
                                            std::function<bool(const node&)>([](const node& x){return x.get_value()%2==1;}))<<std::endl;

        std::cout<<ci.begin().fold(ci.end(),0,
                                            std::function<int(int,int)>([](int a,int b){return a+b;}))<<std::endl;
        std::cout<<cn.begin().fold(cn.end(),std::string(),
                                            std::function<std::string(std::string,node)>([](std::string s,node a){
                                                return s+std::to_string(a.get_value());
                                            }))<<std::endl;
    }
    std::cout<<node::get_delta()<<std::endl;
    return 0;
}