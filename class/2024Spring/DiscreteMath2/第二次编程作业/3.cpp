#include<bits/stdc++.h>
using namespace std;
const int N=1e5+10;

double ans[N],v[N],s[N];
int n,depend_count[N];
vector<int> depend[N];
vector<double> need[N];

double num_to_time(int u,double w)
{
    int round=sqrt(w*2/v[u]/s[u]);
    while((double)round*(round+1)>=w*2/v[u]/s[u])   --round;
    return s[u]*round+(w-v[u]*s[u]*round*(round+1)/2)/v[u]/(round+1);
}

int main()
{
    scanf("%d",&n);
    for(int i=1;i<n;++i)
        scanf("%lf",&v[i]);
    for(int i=1;i<n;++i)
        scanf("%lf",&s[i]);
    queue<int> q;
    for(int i=1;i<=n;++i)
    {
        scanf("%d",&depend_count[i]);
        if(!depend_count[i])
            q.push(i);
        for(int j=1;j<=depend_count[i];++j)
        {
            int u=0;
            double w=0;
            scanf("%d%lf",&u,&w);
            depend[u].push_back(i);
            need[u].push_back(num_to_time(u,w));
        }
    }
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=0;i<depend[u].size();++i)
        {
            int v=depend[u][i];
            ans[v]=max(ans[v],ans[u]+need[u][i]);
            --depend_count[v];
            if(!depend_count[v])
                q.push(v);
        }
    }
    // for(int i=1;i<=n;++i)
    //     cout<<ans[i]<<" ";
    printf("%.2lf",ans[n]);
    return 0;
}