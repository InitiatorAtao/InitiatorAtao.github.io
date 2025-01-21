#include<bits/stdc++.h>
using namespace std;
const int N=2e5+10,M=1e6+10;

vector<int> e[N];
int n,m,s;
bool vis[N];

void dfs(int u)
{
    vis[u]=1;
    cout<<u<<" ";
    for(int v:e[u])
        if(!vis[v])
            dfs(v);
}

inline void bfs(int s)
{
    memset(vis,0,sizeof vis);
    queue<int> q;
    q.push(s);  vis[s]=1;
    while(!q.empty())
    {
        int u=q.front();    q.pop();
        reverse(e[u].begin(),e[u].end());
        cout<<u<<" ";
        for(int v:e[u])
            if(!vis[v])
                vis[v]=1,q.push(v);
    }
    cout<<endl;
}

int main()
{
    cin>>n>>m>>s;
    for(int i=1;i<=m;++i)
    {
        int u,v;
        cin>>u>>v;
        e[u].push_back(v);
    }
    bfs(s);
    memset(vis,0,sizeof vis);
    dfs(s);
    return 0;
}