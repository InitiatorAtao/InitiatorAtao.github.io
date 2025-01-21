#include<bits/stdc++.h>
using namespace std;
const int N=14,INF=0x3f3f3f3f;
const string name[14]={"G2","A3","B3","C3","D3","E3","F3","G3","A4","B4","C4","D4","E4","F4"};

int n=14;

int cost[(1<<N)+10],dist[(1<<N)+10],pre[(1<<N)+10],pre_name[(1<<N)+10];

inline int d(int a,int b)
{
    int d=(a-b>0? a-b:b-a);
    if(d==2)                    return 3;
    if(d==9 || d==5 || d==7)    return 4;
    if(d==4)                    return 5;
    if(d==11 || d==3)           return 6;
    if(d==6)                    return 7;
    if(d==1 || d==13)           return 8;
    return 1000;
}

void initiator()
{
    for(int p=0;p<(1<<n);++p)
    {
        for(int i=0;i<n;++i)
            if(((p>>i)&1))
                for(int j=i+1;j<n;++j)
                    if(((p>>j)&1))
                        cost[p]^=d(i,j);
    }
}

int get_cost(int p)
{
    int c=0;
    for(int i=0;i<n;++i)
        if(((p>>i)&1))
            for(int j=i+1;j<n;++j)
                if(((p>>j)&1))
                {
                    int now_dist=d(i,j);
                    // cout<<"dist of "<<i<<" "<<j<<"="<<now_dist<<endl;
                    c^=now_dist;
                }
    return c;
}

void dijkstra(int s)
{
    memset(dist,0x3f,sizeof dist);
    priority_queue<pair<int,int> > q;
    q.push(make_pair(0,s));
    dist[s]=0;
    while(!q.empty())
    {
        int u=q.top().second;
        q.pop();
        // cout<<"dijkstra in "<<u<<endl;
        for(int i=0;i<n;++i)
        {
            int v=u^(1<<i);
            if(dist[v]>dist[u]+cost[v])
            {
                dist[v]=dist[u]+cost[v];
                pre[v]=u;
                pre_name[v]=i;
                q.push(make_pair(-dist[v],v));
            }
        }
    }
}

int main()
{
    initiator();
    cout<<get_cost(12312)<<endl;
    cout<<get_cost(12440)<<endl;
    cout<<get_cost(12472)<<endl;
    // cout<<get_cost(10760)<<endl;
    // return 0;
    string s;
    cin>>s;
    int p=0;
    for(int i=0;i<n;++i)
        p|=((s[i]=='1')<<i);
    dijkstra(p);
    vector<string> ans;
    for(int i=0;i!=p;i=pre[i])
        ans.push_back(name[pre_name[i]]);
    reverse(ans.begin(),ans.end());
    for(string i:ans)
        cout<<i<<" ";
    cout<<endl<<dist[0];
    return 0;
}