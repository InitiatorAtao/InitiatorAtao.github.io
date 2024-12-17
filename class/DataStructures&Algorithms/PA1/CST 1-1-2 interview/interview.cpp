#include<cstdio>
#include<cstdlib>
const int N=1e6+10,NM=1e7+10;

int n,m,id[N],next[N],prev[N],p;

int main() {
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;++i) {
        scanf("%d",&id[i]);
        if(i==0) {
            next[i]=i;
            prev[i]=i;
            p=i;
        }
        else {
            for(int j=0;j<m;++j) {
                p=next[p];
            }
            int np=next[p];
            prev[i]=p;  next[p]=i;
            prev[np]=i; next[i]=np;
        }
    }
    for(int i=0,j=n-1;i<n;++i,j=prev[j])
        printf("%d ",id[j]);
    return 0;
}