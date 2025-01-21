#include<bits/stdc++.h>
using namespace std;
const int N=110,M=10010,K=110,mod=1000003;

int n,m,s,t,k;

class matrix
{
    public:
    int val[N][N];
    matrix(){memset(val,0,sizeof val);}
    matrix operator*(const matrix &other)
    {
        matrix result;
        for(int i=1;i<=n;++i)
            for(int j=1;j<=n;++j)
                for(int k=1;k<=n;++k)
                    result.val[i][j]=(1ll*result.val[i][j]+1ll*val[i][k]*other.val[k][j]%mod)%mod;
        return result;
    }
};

int main()
{
    cin>>n>>m>>s>>t>>k;
    matrix a,res;
    for(int i=1;i<=n;++i)
        res.val[i][i]=1;
    for(int i=1;i<=m;++i)
    {
        int u,v;
        cin>>u>>v;
        ++a.val[u][v];
    }
    while(k)
    {
        if(k&1) res=res*a;
        a=a*a;
        k>>=1;
    }
    cout<<res.val[s][t]<<endl;
    return 0;
}