#include<cstdio>
#include<cstdlib>
const int N=2e5+10,M=2e5+10;

int n,m,x[N],y[N],tmp[N];

void msort(int *l,int *r) {
    if(l+1>=r)    return;
    int *mid=l+((r-l)>>1);
    msort(l,mid);   msort(mid,r);
    int *i=l,*j=mid,*k=&tmp[0];
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
    scanf("%d",&n);
    for(int i=0;i<n;++i)
        scanf("%d",&x[i]);
    for(int i=0;i<n;++i)
        scanf("%d",&y[i]);
    msort(&x[0],&x[n]);
    msort(&y[0],&y[n]);
    scanf("%d",&m);
    while(m--) {
        int nx=0,ny=0,l=0,r=n;
        scanf("%d%d",&nx,&ny);
        while(l<r) {
            int mid=(l+r)>>1;
            if(1ll*nx*y[mid]+1ll*ny*x[mid]>=1ll*x[mid]*y[mid])
                l=mid+1;
            else    r=mid;
        }
        printf("%d\n",l);
    }
    return 0;
}