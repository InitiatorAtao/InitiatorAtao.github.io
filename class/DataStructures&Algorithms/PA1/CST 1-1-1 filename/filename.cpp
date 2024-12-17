#include<cstdio>
#include<cstring>
#include<cstdlib>   //why don't you let us write C rather than C++???
const int N=501010,M=501010,K=110;
const int INF=0x3f3f3f3f;

int n,m,k;
char a[N],b[M];
int f[2][K<<1];

inline int min(int a,int b) {
    return a>b? b:a;
}

inline int max(int a,int b) {
    return a>b? a:b;
}

// short dfs(int i,int j) {
//     if(f[i][i-j+K]!=INF)                return f[i][i-j+K];
//     if(i==n && j==m)                    return 0;
//     if(i>n || j>m || i-j>k || j-i>k)    return INF;
//     short cost=min(dfs(i+1,j)+1,dfs(i,j+1)+1);
//     if(i<n && j<m) {
//         if(a[i]==b[j])  cost=min(cost,dfs(i+1,j+1));
//         else            cost=min(cost,dfs(i+1,j+1)+2);
//     }
//     f[i][i-j+K]=cost=min(cost,k+1);
//     return cost;
// }

int main() {
    memset(f,INF,sizeof(f));
    scanf("%d%d%d%s%s",&n,&m,&k,&a,&b);
    if(n-m>k || m-n>k) {
        puts("-1");
        return 0;
    }
    f[n&1][n-m+K]=0;
    for(int i=n-1;i>=0;--i) {
        memset(f[i&1],0x3f,sizeof(f[i&1]));
        for(int j=min(m-1,i+k);j>=max(0,i-k);--j) {
            f[i&1][i-j+K]=min(f[(i&1)^1][i+1-j+K],f[i&1][i-j-1+K])+1;
            if(a[i]==b[j])  f[i&1][i-j+K]=min(f[i&1][i-j+K],f[(i&1)^1][i-j+K]);
            else            f[i&1][i-j+K]=min(f[i&1][i-j+K],f[(i&1)^1][i-j+K]+2);
            if(f[i&1][i-j+K]>k)   f[i&1][i-j+K]=INF;
        }
    }
    // short result=dfs(0,0);
    // printf("%d\n",(result>k)? (-1):result);
    printf("%d\n",(f[0][K]>k)? (-1):f[0][K]);
    return 0;
}