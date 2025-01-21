#include<bits/stdc++.h>
#include"RIP.h"
using namespace std;
const int N=20+10;

int n;
Router net[N];
vector<int> e[N];
vector<Jump> update[N];

int main()
{
    cin>>n;
    for(int i=0;i<n;++i)
    {
        int k=0,l=0;
        vector<Router*> initiator;
        cin>>k;
        for(int j=0;j<k;++j)
        {
            cin>>l;
            e[i].push_back(l);
            initiator.push_back(&net[l]);
        }
        net[i].init(initiator);
    }
    bool updated=0;
    do
    {
        updated=0;
        for(int i=0;i<n;++i)
            update[i]=net[i].send();
        for(int i=0;i<n;++i)
            for(int j:e[i])
                updated|=net[i].receive((&net[j]),update[j]);
    }while(updated);
    for(int i=0;i<n;++i)
    {
        update[i]=net[i].send();
        for(int j=0;j<n;++j)
        {
            bool exist=0;
            for(Jump & k:update[i])
                if((&net[j])==k.dest)
                {
                    exist=1;
                    cout<<k.length<<" ";
                }
            if(!exist)
                cout<<0<<" ";
        }
        cout<<endl;
    }
    return 0;
}