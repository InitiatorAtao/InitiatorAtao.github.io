#include<cstdio>
#include<cstdlib>   //这时候反而给 STL 了...
const int N=5e5+10,V=1e9+10;

struct Value {
    int a,b,c;
}v[N],tmpv[N],solved[N];

int n,tmpi[N],b[N];
long long t[N];

void msort(Value *l,Value *r) {
    if(l+1>=r)    return;
    Value *mid=l+((r-l)>>1);
    msort(l,mid);   msort(mid,r);
    Value *i=l,*j=mid,*k=&tmpv[0];
    while(i<mid && j<r) {
        if((*i).a<(*j).a || ((*i).a==(*j).a && (*i).b<=(*j).b))
            *(k++)=*(i++);
        else    *(k++)=*(j++);
    }
    while(i<mid)    *(k++)=*(i++);
    while(j<r)      *(k++)=*(j++);
    for(i=l,k=&tmpv[0];i<r;++i,++k)
        (*i)=(*k);
}

void msort(int *l,int *r) {
    if(l+1>=r)    return;
    int *mid=l+((r-l)>>1);
    msort(l,mid);   msort(mid,r);
    int *i=l,*j=mid,*k=&tmpi[0];
    while(i<mid && j<r) {
        if((*i)<=(*j))  *(k++)=*(i++);
        else            *(k++)=*(j++);
    }
    while(i<mid)    *(k++)=*(i++);
    while(j<r)      *(k++)=*(j++);
    for(i=l,k=&tmpi[0];i<r;++i,++k)
        (*i)=(*k);
}

inline long long ask(int p) {
    long long result=0;
    for(p+=1;p;p-=p&(-p))
        result=(result<t[p])? t[p]:result;
    return result;
}

inline void add(int p,long long value) {
    for(p+=1;p<N;p+=p&(-p))
        t[p]=(t[p]<value)? value:t[p];
}

int main() {
    scanf("%d",&n);
    for(int i=0;i<n;++i) {
        scanf("%d%d%d",&v[i].a,&v[i].b,&v[i].c);
    }
    msort(&v[0],&v[n]);
    int nn=0;
    for(int i=0;i<n;++i) {
        if(v[i].c<=0)   continue;
        if(!nn || solved[nn-1].a!=v[i].a || solved[nn-1].b!=v[i].b)
            solved[nn++]=v[i];
        else    solved[nn-1].c+=v[i].c;
    }
    for(int i=0;i<nn;++i) {
        b[i]=solved[i].b;
    }
    msort(&b[0],&b[nn]);
    int nnn=1;
    for(int i=1;i<nn;++i) {
        if(b[i]!=b[nnn-1])
            b[nnn++]=b[i];
    }
    for(int i=0;i<nn;++i) {
        int l=0,r=nnn;
        while(l<r) {
            int mid=(l+r+1)>>1;
            if(solved[i].b>=b[mid]) l=mid;
            else                    r=mid-1;
        }
        solved[i].b=l;
    }
    // puts("?");
    long long ans=0;
    for(int i=0;i<nn;++i) {
        long long f=ask(solved[i].b)+solved[i].c;
        ans=(ans<f)? f:ans;
        add(solved[i].b,f);
    }
    printf("%lld\n",ans);
    return 0;
}
