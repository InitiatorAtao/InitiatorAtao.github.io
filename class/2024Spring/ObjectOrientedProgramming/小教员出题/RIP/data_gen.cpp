#include<bits/stdc++.h>
using namespace std;
const int N=20+10;

bool vis[N];
int n,link[N][N],dist[N][N];
queue<int> q;

int main()
{
    srand(time(0));
    int n;
    float edge_density;
    string filename;
    cin>>n>>edge_density>>filename;
    for(int i=0;i<n;++i)
        for(int j=0;j<i;++j)
            link[i][j]=link[j][i]=(1.0*rand()/RAND_MAX<edge_density);
    for(int i=0;i<n;++i)
    {
        memset(vis,0,sizeof vis);
        q.push(i);
        vis[i]=1;
        while(!q.empty())
        {
            int u=q.front();
            cout<<u<<endl;
            q.pop();
            for(int v=0;v<n;++v)
                if(link[u][v] && !vis[v])
                {
                    dist[i][v]=dist[i][u]+1;
                    q.push(v);
                    vis[v]=1;
                }
        }
    }
    freopen(("data/"+filename+".in").c_str(),"w",stdout);
    cout<<n<<endl;
    for(int i=0;i<n;++i)
    {
        vector<int> v;
        for(int j=0;j<n;++j)
            if(link[i][j])
                v.push_back(j);
        cout<<v.size()<<" ";
        for(int j:v)
            cout<<j<<" ";
        cout<<endl;
    }
    freopen(("data/"+filename+".ans").c_str(),"w",stdout);
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
            cout<<dist[i][j]<<" ";
        cout<<endl;
    }
    return 0;
}