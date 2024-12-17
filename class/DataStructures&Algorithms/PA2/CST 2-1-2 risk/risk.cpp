#include <cassert>
#include <cstdio>
#include <cstdlib>
const int N=1e6+10,X=2e6+10,T=1e5+10;

unsigned n,t,x[N],m[N],p[T],q[T],s[T<<1],ps[T],qs[T];
unsigned sta[N],head,tail,ss[T<<1];
unsigned tmp[T<<1];

inline unsigned min(unsigned a,unsigned b) {
    return a<b? a:b;
}

void msort(unsigned *l,unsigned *r) {
    if(l+1>=r)    return;
    unsigned *mid=l+((r-l)>>1);
    msort(l,mid);   msort(mid,r);
    unsigned *i=l,*j=mid,*k=&tmp[0];
    while(i<mid && j<r) {
        if((*i)<=(*j))  *(k++)=*(i++);
        else            *(k++)=*(j++);
    }
    while(i<mid)    *(k++)=*(i++);
    while(j<r)      *(k++)=*(j++);
    for(i=l,k=&tmp[0];i<r;++i,++k)
        (*i)=(*k);
}

inline unsigned* upper_bound(unsigned *l,unsigned *r,unsigned v) {
    while (l<r) {
        unsigned *mid=l+((r-l)>>1);
        if (v<(*mid))  r=mid;
        else           l=mid+1;
    }
    return l;
}

inline unsigned* lower_bound(unsigned *l,unsigned *r,unsigned v) {
    while (l<r) {
        unsigned *mid=l+((r-l)>>1);
        if (v<=(*mid)) r=mid;
        else           l=mid+1;
    }
    return l;
}

inline unsigned* unique(unsigned *l,unsigned *r) {
    unsigned *i=l;
    for (unsigned *j=l; j!=r; ++j) {
        if (i==l || (*(i-1))!=(*j)) {
            (*(i++))=(*j);
        }
    }
    return i;
}

int main() {
    scanf("%u",&n);
    for (int i=0; i<n; ++i) {
        scanf("%u",&x[i]);
    }
    for (int i=0; i<n; ++i) {
        scanf("%u",&m[i]);
    }
    scanf("%u",&t);
    for (int i=0; i<t; ++i) {
        scanf("%u%u",&p[i],&q[i]);
        s[i<<1]=p[i];
        s[i<<1|1]=q[i];
    }
    msort(s, s+(t<<1));
    unsigned st=unique(s,s+(t<<1))-s;
    for (int i=0; i<t; ++i) {
        ps[i]=lower_bound(s,s+st,p[i])-s;
        qs[i]=lower_bound(s,s+st,q[i])-s;
    }
    for (unsigned i=0; i<n; ++i) {
        unsigned line=i-min(m[i],i);
        while (head<tail && sta[head]<line) ++head; 
        if (head<tail) {
            //printf("in day %u,head=%u, tail=%u, sta[head]=%u[%u]\n",i,head,tail,x[sta[head]],sta[head]);
            ++ss[upper_bound(s, s+st, x[sta[head]])-s];
        }
        else ++ss[upper_bound(s, s+st, 0)-s];
        while (head<tail && x[sta[tail-1]]<=x[i]) --tail;
        sta[tail++]=i;
    }
    //printf("%u ",ss[0]);
    for (int i=1; i<st; ++i) {
        ss[i]+=ss[i-1];
        //printf("%u ",ss[i]);
    }
    //puts("");
    for (int i=0; i<t; ++i) {
        printf("%u %u\n",ss[ps[i]],ss[qs[i]]-ss[ps[i]]);
    }
    /*
    for (int i=0; i<st; ++i) {
        printf("%u ",s[i]);
    }
    puts("");
    for (int i=0; i<t; ++i) {
        printf("%u ",ps[i]);
    }
    puts("");
    for (int i=0; i<t; ++i) {
        printf("%u ",qs[i]);
    }
    puts("");
    */
    return 0;
}
