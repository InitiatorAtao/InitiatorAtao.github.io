#include<bits/stdc++.h>
using namespace std;
const int N=(40+10)*2,M=(810*2+50*2),INF=0x3f3f3f3f;

int n,m,s,t,d[N],now[N],head[N],cnt=1;
struct edge{int u,v,w,n;}e[M];

inline void add(int f,int t,int v)
{
    e[++cnt]={f,t,v,head[f]};
    head[f]=cnt;
    e[++cnt]={t,f,0,head[t]};
    head[t]=cnt;
}

inline bool bfs()
{
    queue<int> q;
    memset(d,0,sizeof d);
    q.push(s);  d[s]=1; now[s]=head[s];
    while(!q.empty())
    {
        int u=q.front();    q.pop();
        if(u==t)    return 1;
        for(int i=head[u];i;i=e[i].n)
        {
            int v=e[i].v;
            if(!e[i].w || d[v])  continue;
            d[v]=d[u]+1;    now[v]=head[v];
            q.push(v);
        }
    }
    return 0;
}

int dinic(int u,int flow)
{
    if(u==t)    return flow;
    int rest=flow;
    for(int &i=now[u];i && rest;i=e[i].n)
    {
        int v=e[i].v,&w=e[i].w;
        if(!w || d[v]!=d[u]+1)  continue;
        int k=dinic(v,min(rest,w));
        if(!k)  {d[v]=0;    continue;}
        w-=k;   e[i^1].w+=k;
        rest-=k;
        if(!rest)   break;
    }
    return flow-rest;
}

int main()
{
    cin>>n>>m;
    for(int i=0;i<=n;++i)
    {
        int a=0;
        cin>>a;
        add(i<<1,i<<1|1,a);
    }
    s=0,t=(n+1)<<1;
    for(int i=1;i<=m;++i)
    {
        int u=0,v=0,w=0;
        cin>>u>>v>>w;
        if(w==-1)   add(u<<1|1,v<<1,INF);
        else        add(u<<1|1,v<<1,w);
    }
    int maxflow=0;
    while(bfs())
        maxflow+=dinic(s,INF);
    cout<<maxflow<<endl;
    return 0;
}