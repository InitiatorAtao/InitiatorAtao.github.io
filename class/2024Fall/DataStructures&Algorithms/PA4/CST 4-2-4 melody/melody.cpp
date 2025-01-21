#include <cassert>
#include <cstdio>
#include <cstdlib>
const int N=5e5+10,M=5e5+10,Q=1e6+10,L=22,INF=0x7fffffff;

int n,m,a[N+Q],dep[N+Q]={0,1},jmp[N+Q][L],p;    //jmp[i][j] is the j-th parent on LPS (longest prefix suffix) tree
                                                        //dep[i] is the depth in LPS tree
int main() {
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;++i) {
        scanf("%d",&a[i]);
        if(i>1) {
            while(p && a[i]!=a[p+1]) p=jmp[p][0];
            if(a[i]==a[p+1])  ++p;
            jmp[i][0]=p;
            dep[i]=dep[p]+1;
            for(int j=1;j<L;++j)
                jmp[i][j]=jmp[jmp[i][j-1]][j-1];
        }
    }
    while (m--) {
        int op=0,q=0;
        scanf("%d%d",&op,&q);
        if(op==1) {
            while(q--) {
                scanf("%d",&a[++n]);
                while(p && a[n]!=a[p+1]) p=jmp[p][0];
                if(a[n]==a[p+1]) ++p;
                jmp[n][0]=p;
                dep[n]=dep[p]+1;
                for(int j=1;j<L;++j)
                    jmp[n][j]=jmp[jmp[n][j-1]][j-1];
            }
        }
        else if(op==2) {
            int result=INF;
            while (q--) {
                int query=0,d=0;
                scanf("%d",&query);
                d=jmp[query][0];
                if(result==INF) result=d;
                else {                      //find LCA of d and result on LPS tree
                    //while(dep[d]<dep[result]) result=lps[result];
                    for(int i=L-1;i>=0;--i)
                        if(dep[jmp[result][i]]>=dep[d])
                            result=jmp[result][i];
                    //while(dep[result]<dep[d]) d=lps[d];
                    for(int i=L-1;i>=0;--i)
                        if(dep[jmp[d][i]]>=dep[result])
                            d=jmp[d][i];
                    //while(d!=result) d=lps[d],result=lps[result];
                    for(int i=L-1;i>=0;--i)
                        if(jmp[d][i]!=jmp[result][i])
                            d=jmp[d][i],result=jmp[result][i];
                    if(d!=result)
                        d=jmp[d][0],result=jmp[result][0];
                }
                //while((result<<1)>query)  result=lps[result];
                for(int i=L-1;i>=0;--i)
                    if((jmp[result][i]<<1)>query)
                        result=jmp[result][i];
                if((result<<1)>query)
                    result=jmp[result][0];
            }
            printf("%d\n",result);
        }
        else exit(404);
    }
    return 0;
}
