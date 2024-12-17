#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <functional>   //test functional used
typedef long long LL;
const int N=0x7fffffff,M=2e5+10,S=M<<1;

template<typename T>
inline void swap(T &a,T &b) {
    T c=a; a=b; b=c;
}

template<typename A,typename B>
struct Pair {
    A a;
    B b;
};

template<typename V,typename C> //common seg-tree template, V refers to value, C refers to change (lazy tag)
class SegTree {
private:
    V v[S<<2];
    C c[S<<2];
    int l[S<<2],r[S<<2];
    C (*change_merge)(const C&,const C&);
    V (*value_merge)(const V&,const V&);
    V (*change_apply)(const V&,const C&);
    inline void pushdown(int u) {
        int ls=u<<1,rs=u<<1|1;
        v[ls]=change_apply(v[ls],c[u]);
        c[ls]=change_merge(c[ls],c[u]);
        v[rs]=change_apply(v[rs],c[u]);
        c[rs]=change_merge(c[rs],c[u]);
        c[u]=C();
    }
    void build(int u,int L,int R,V (*value)(int)) {
        l[u]=L;  r[u]=R;
        if(L==R) {
            v[u]=value(L); c[u]=C();
            return;
        }
        int mid=(L+R)>>1;
        build(u<<1, L, mid, value);
        build(u<<1|1, mid+1, R, value);
        v[u]=value_merge(v[u<<1],v[u<<1|1]);
        c[u]=C();
    }
    void modify(int u,int L,int R,const C& change) {
        if (L<=l[u] && r[u]<=R) {
            v[u]=change_apply(v[u],change);
            c[u]=change_merge(c[u],change);
            return;
        }
        int mid=(l[u]+r[u])>>1; pushdown(u);
        if(L<=mid)  modify(u<<1, L, R, change);
        if(R>mid)   modify(u<<1|1, L, R, change);
        v[u]=value_merge(v[u<<1],v[u<<1|1]);
    }
    V query(int u,int L,int R) {
        if (L<=l[u] && r[u]<=R) return v[u];
        int mid=(l[u]+r[u])>>1; pushdown(u);
        V result=V();
        if(L<=mid)  result=value_merge(query(u<<1, L, R),result);
        if(R>mid)   result=value_merge(query(u<<1|1, L, R),result);
        return result;
    }
public:
    SegTree(
            C (*change_merge_function)(const C&,const C&),
            V (*value_merge_function)(const V&,const V&),
            V (*change_apply_function)(const V&,const C&))
        :change_merge(change_merge_function),
        value_merge(value_merge_function),
        change_apply(change_apply_function) {
    }
    void build(int L,int R,V (*value)(int)) {
        build(1,L,R,value);
    }
    void modify(int L,int R,const C &change) {
        modify(1,L,R,change);
    }
    V query(int L,int R) {
        return query(1,L,R);
    }
};

template<typename T>
void merge_sort(T *l,T *r) {
    static T tmp[S];
    if(r-l<=1) return;
    T *mid=l+((r-l)>>1),*t=tmp;
    merge_sort(l, mid);
    merge_sort(mid, r);
    T *i=l,*j=mid;
    while(i<mid && j<r) {
        if(*j<*i)   *(t++)=*(j++);
        else        *(t++)=*(i++);
    }
    while(i<mid)*(t++)=*(i++);
    while(j<r)  *(t++)=*(j++);
    while(tmp<t) *(--r)=*(--t);
}

template<typename T>
T* unique(T *l,T *r) {
    T* i=l;
    for (T* j=l+1; j<r; *(++i)=*(j++)) {
        while(j<r && *i==*j) ++j;
    }
    return i+1;
}

template<typename T>
T* lower_bound(T *l,T *r,const T &c) {
    while (l<r) {
        T *mid=l+((r-l)>>1);
        if((*mid)<c)l=mid+1;
        else        r=mid;
    }
    return l;
}

struct RangeValue {
    int l,r;
    long long sum;
};
RangeValue value_add(
        const RangeValue &a,
        const RangeValue &b) {
    return {a.l,b.r,a.sum+b.sum};
}
RangeValue range_add(
        const RangeValue &a,
        const RangeValue &b) {
    return {a.l,a.r,a.sum+b.sum*(a.r-a.l+1)};
}
SegTree<RangeValue, RangeValue>seg_tree(value_add,value_add,range_add);

char op[M];
int n,m,l[M],r[M],s[S],top;

int main() {
    scanf("%d%d",&n,&m);
    for (int i=0; i<m; ++i) {
        scanf("%s%d%d",&op[i],&l[i],&r[i]);
        s[top++]=l[i]-1;  s[top++]=r[i];
    }
    merge_sort(s, s+top);
    top=unique(s, s+top)-s;
    seg_tree.build(0, top-1, [](int i){
        return RangeValue{(i?s[i-1]+1:0),s[i],0ll};
    });
    for (int i=0; i<m; ++i) {
        int left=l[i],right=r[i];
        l[i]=lower_bound(s,s+top,l[i]-1)-s+1;
        r[i]=lower_bound(s,s+top,r[i])-s;
        if (op[i]=='H') {
            seg_tree.modify(l[i], r[i], RangeValue{left,right,1});
        }
        else printf("%lld\n",seg_tree.query(l[i], r[i]).sum);
    }
    return 0;
}
