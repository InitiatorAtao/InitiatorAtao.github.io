#include<cstdio>
#include<cstdlib>
const int N=40+5,V=(1<<20)+10,P=1e9+10;

int n,p,c[2][N],pv[2];
long long value[2][V],tmp[V];

void dfs(int l,int r,long long now,int v) {
    if(r<=l) {
        value[v][pv[v]++]=now;
        return;
    }
    dfs(l+1,r,now+c[0][l],v);
    dfs(l+1,r,now+c[1][l],v);
}

void msort(long long *l,long long *r) {
    if(l+1>=r)    return;
    long long *mid=l+((r-l)>>1);
    msort(l,mid);   msort(mid,r);
    long long *i=l,*j=mid,*k=&tmp[0];
    while(i<mid && j<r) {
        if((*i)<=(*j))  *(k++)=*(i++);
        else            *(k++)=*(j++);
    }
    while(i<mid)    *(k++)=*(i++);
    while(j<r)      *(k++)=*(j++);
    for(i=l,k=&tmp[0];i<r;++i,++k)
        (*i)=(*k);
}

int main() {
    scanf("%d%d",&n,&p);
    for(int i=0;i<n;++i) {
        scanf("%d%d",&c[0][i],&c[1][i]);
    }
    if(n==1) {
        printf("%d",(c[0][0]<=p)+(c[1][0]<=p));
        return 0;
    }
    int mid=n>>1;
    dfs(0,mid,0,0);
    dfs(mid,n,0,1);
    msort(&value[0][0],&value[0][pv[0]]);
    msort(&value[1][0],&value[1][pv[1]]);
    // for(int i=0;i<2;++i) {
    //     for(int j=0;j<pv[i];++j) {
    //         printf("%lld ",value[i][j]);
    //     }
    //     puts("");
    // }
    long long ans=0;
    for(int i=0,j=pv[1]-1;i<pv[0] && j>=0;++i) {
        while(j>=0 && value[0][i]+value[1][j]>p) --j;
        ans+=(j+1);
    }
    printf("%lld",ans);
    return 0;
}