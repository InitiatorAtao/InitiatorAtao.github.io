#include<bits/stdc++.h>
using namespace std;
const int N=18+2,INF=0x3f3f3f3f;

int n,m,w[N][N],c[N][N],ans[N],ans_round=INF,now[N],now_round;
bool vis[N];

void dfs(int u,int p)
{
    now[p]=u;
    if(p>=n)
    {
        if(now_round<ans_round)
        {
            ans_round=now_round;
            for(int i=1;i<=n;++i)
                ans[i]=now[i];
        }
        return;
    }
    // cout<<"dfs in "<<u<<" "<<p<<" "<<now_round<<" "<<ans_round<<endl;
    // for(int i=1;i<=p;++i)
    //     cout<<now[i]<<" ";
    // cout<<endl;
    vis[u]=1;
    for(int i=1;i<=n;++i)
    {
        if(vis[i] || m<c[u][i] || now_round+w[u][i]>=ans_round)
            continue;
        now_round+=w[u][i];
        m-=c[u][i];
        dfs(i,p+1);
        m+=c[u][i];
        now_round-=w[u][i];
    }
    vis[u]=0;
}

int main()
{
    cin>>n>>m;
    for(int i=1;i<=n;++i)
        for(int j=1;j<=n;++j)
            cin>>w[i][j];
    for(int i=1;i<=n;++i)
        for(int j=1;j<=n;++j)
            cin>>c[i][j];
    for(int i=1;i<=n;++i)
        dfs(i,1);
    if(ans_round==INF)
        return cout<<-1<<endl,0;
    cout<<ans_round<<endl;
    for(int i=1;i<=n;++i)
        cout<<ans[i]<<" ";
    return 0;
}