//All of this file needs implement
#include"RIP.h"

void Router::init(std::vector<Router*> initiator)
{
    router.push_back(Jump{this,this,0});
    for(Router *pr:initiator)
        router.push_back(Jump{pr,pr,1});
}

bool Router::receive(Router* from,std::vector<Jump> update)
{
    bool updated=0;
    for(Jump & i:update)
    {
        i.next=from;
        i.length+=1;
        bool exist=0;
        for(Jump & j:router)
            if(j.dest==i.dest)
            {
                exist=1;
                if(j.length>i.length)
                {
                    updated=1;
                    j=i;
                }
            }
        if(!exist)
        {
            updated=1;
            router.push_back(i);
        }
    }
    return updated;
}

std::vector<Jump> Router::send(){return router;}