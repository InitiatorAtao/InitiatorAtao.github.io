#include<bits/stdc++.h>
using namespace std;
const int N=15+2,M=100+10;
int n,m,col[N];
vector<int> e[N];

bool dfs(int u,int c)
{
    for(int i=1;i<=c;++i)
    {
        bool fail=0;
        for(int v:e[u])
            if(col[v]==i)
                {fail=1;break;}
        if(fail)
            continue;
        col[u]=i;
        for(int v:e[u])
            if(!col[v] && !dfs(v,c))
                {fail=1;break;}
        if(!fail)
            return 1;
    }
    col[u]=0;
    return 0;
}

int main()
{
    cin>>n>>m;
    for(int i=1;i<=m;++i)
    {
        int u=0,v=0;
        cin>>u>>v;
        e[u].push_back(v);
        e[v].push_back(u);
    }
    for(int i=1;i<=n;++i)
    {
        bool fail=0;
        memset(col,0,sizeof col);
        for(int j=1;j<=n;++j)
            if(!col[j] && !dfs(j,i))
                {fail=1;break;}
        if(!fail)
        {
            cout<<i<<endl;
            for(int i=1;i<=n;++i)
                cout<<col[i]<<" ";
            return 0;
        }
    }
    return 0;
}