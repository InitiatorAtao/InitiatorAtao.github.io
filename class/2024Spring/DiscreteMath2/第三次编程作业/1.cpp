#include<bits/stdc++.h>
using namespace std;
const int N=5010,Q=5010;
const long long INF=0x3f3f3f3f3f3f3f3f;

int n,q;
int x[N],y[N],pre[N];
bool vis[N];
long long dist[N][N],reach[N],cost;
struct Edge
{
    int u,v;
    long long w;
    bool operator==(const Edge& other)const
    {
        return u==other.u && v==other.v && w==other.w;
    }
};
vector<Edge> tree[N];

void prim()
{
    memset(reach,0x3f,sizeof reach);
    reach[1]=0;
    for(int i=1;i<=n;++i)
    {
        int p=0;
        for(int j=1;j<=n;++j)
            if(reach[j]<reach[p] && !vis[j])
                p=j;
        assert(p && reach[p]!=INF);
        vis[p]=1;
        if(pre[p])
        {
            tree[p].push_back(Edge({p,pre[p],dist[p][pre[p]]}));
            tree[pre[p]].push_back(Edge({pre[p],p,dist[pre[p]][p]}));
            cost+=dist[pre[p]][p];
        }
        for(int j=1;j<=n;++j)
            if(!vis[j] && reach[j]>dist[p][j])
            {
                reach[j]=dist[p][j];
                pre[j]=p;
            }
    }
}

Edge dfs(int u,int goal)
{
    vis[u]=1;
    for(const Edge& e:tree[u])
    {
        if(e.v==goal)
            return e;
        if(!vis[e.v])
        {
            Edge res=dfs(e.v,goal);
            if(res.w>0)
                return res.w>e.w? res:e;
        }
    }
    return Edge({-1,-1,-1});
}

int main()
{
    scanf("%d%d",&n,&q);
    for(int i=1;i<=n;++i)
        scanf("%d%d",&x[i],&y[i]);
    for(int i=1;i<n;++i)
        for(int j=i+1;j<=n;++j)
        {
            long long dx=abs(x[i]-x[j]),dy=abs(y[i]-y[j]);
            dist[i][j]=dist[j][i]=dx*dx*dx+dy*dy*dy;
        }
    prim();
    printf("%lld\n",cost);
    while(q--)
    {
        int u=0,v=0;
        long long c=0;
        scanf("%d%d%lld",&u,&v,&c);
        memset(vis,0,sizeof vis);
        Edge m=dfs(u,v);
        if(m.w>c)
        {
            tree[m.u].erase(find(tree[m.u].begin(),tree[m.u].end(),m));
            swap(m.u,m.v);
            tree[m.u].erase(find(tree[m.u].begin(),tree[m.u].end(),m));
            cost-=m.w;
            tree[u].push_back(Edge({u,v,c}));
            tree[v].push_back(Edge({v,u,c}));
            cost+=c;
        }
        printf("%lld\n",cost);
    }
    return 0;
}