#include<bits/stdc++.h>
using namespace std;
const int N=110,M=10010,W=10010;

struct node
{
    int from,to,weight,next;
}e[M];
int n,m,c,head[N],a[N][N];

inline void add(int f,int t,int w)
{
    e[++c]={f,t,w,head[f]};
    head[f]=c;
}

int main()
{
    cin>>n>>m;
    for(int i=0;i<m;++i)
    {
        int f,t,w;
        cin>>f>>t>>w;
        add(f,t,w);
        a[f][t]=(a[f][t]==0? w:-1);
    }
    for(int i=1;i<=n;++i,cout<<endl)
        for(int j=1;j<=n;++j)
            cout<<a[i][j]<<" ";
    for(int i=1;i<=n;++i)
    {
        cout<<head[i]<<endl;
    }
    for(int i=1;i<=c;++i)
        cout<<e[i].to<<" "<<e[i].weight<<" "<<e[i].next<<endl;
    return 0;
}