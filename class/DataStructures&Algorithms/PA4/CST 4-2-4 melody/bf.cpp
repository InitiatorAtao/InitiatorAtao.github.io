#include <cstdio>
#include <cstdlib>
const int N=5e5+10,M=5e5+10,Q=1e6+10,INF=0x7fffffff;

int n,m,a[N+Q],lps[N+Q],dep[N+Q],p;

int main() {
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;++i) {
        scanf("%d",&a[i]);
        if(i>1) {
            while(p && a[i]!=a[p+1]) p=lps[p];
            if(a[i]==a[p+1])  ++p;
            lps[i]=p;
            dep[i]=dep[p]+1;
        }
    }
    /*
    for(int i=0;i<=n;++i)
        printf("%d ",lps[i]);
    puts("");
    for(int i=0;i<=n;++i)
        printf("%d ",dep[i]);
    puts("");
    */
    while (m--) {
        int op=0,q=0;
        scanf("%d%d",&op,&q);
        if(op==1) {
            while(q--) {
                scanf("%d",&a[++n]);
                while(p && a[n]!=a[p+1]) p=lps[p];
                if(a[n]==a[p+1]) ++p;
                lps[n]=p;
                dep[n]=dep[p]+1;
                //printf("%d %d\n",lps[n],dep[n]);
            }
        }
        else if(op==2) {
            int result=INF;
            while (q--) {
                int query=0,d=0;
                scanf("%d",&query);
                d=lps[query];
                if(result==INF) result=d;
                else {
                    while(dep[d]<dep[result]) result=lps[result];
                    while(dep[result]<dep[d]) d=lps[d];
                    while(d!=result) d=lps[d],result=lps[result];
                }
                while((result<<1)>query)  result=lps[result];
            }
            printf("%d\n",result);
        }
        else exit(404);
    }
    return 0;
}
