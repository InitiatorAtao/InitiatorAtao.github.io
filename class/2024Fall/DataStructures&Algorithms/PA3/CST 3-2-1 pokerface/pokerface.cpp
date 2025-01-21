#include <cassert>
#include <cstdio>
#include <cstdlib>
const int N=1e5+10,M=1e5+10,S=N+N+M;

template<typename T>
void swap(T &a,T &b) {
    T c=a; a=b; b=c;
}

template<typename T>
T min(const T &a,const T &b) {
    return a<b? a:b;
}

template<typename T>
class Vector {
public:
    Vector() {
        value=new T[MIN_SIZE];
        capacity=MIN_SIZE;
        tail=0;
    }
    Vector operator=(const Vector<T> other) {
        capacity=other.capacity;
        tail=other.tail;
        value=new T[capacity];
        for(int i=0; i<tail; ++i) {
            value[i]=other.value[i];
        }
        return (*this);
    }
    Vector(const Vector<T> &other) {
        capacity=other.capacity;
        tail=other.tail;
        value=new T[capacity];
        for(int i=0; i<tail; ++i) {
            value[i]=other.value[i];
        }
    }
    void expand() {
        capacity*=2;
        T *new_value=new T[capacity];
        for (int i=0; i<tail; ++i) {
            new_value[i]=value[i];
        }
        delete [] value;
        value=new_value;
    }
    void shrink() {
        if (tail*2<capacity && capacity>MIN_SIZE) {
            capacity/=2;
            T *new_value=new T[capacity];
            for(int i=0;i<tail;++i) {
                new_value[i]=value[i];
            }
            delete [] value;
            value=new_value;
        }
    }
    void push_back(int x) {
        if (tail>=capacity) expand();
        value[tail++]=x;
    }
    T pop_back() {
        if (!tail) exit(1);
        T result=value[--tail];
        shrink();
        return result;
    }
    T& operator[](int i) {
        if(i>=tail) exit(1);
        return value[i];
    }
    T operator[](int i)const{
        if(i>=tail) exit(1);
        return value[i];
    }
    int size()const{return tail;}
    ~Vector() {delete [] value;}
    bool operator==(const Vector<T>& other)const {
        if(size()!=other.size()) return false;
        for(int i=0;i<size();++i)
            if((*this)[i]!=other[i])
                return false;
        return true;
    }
private:
    static const int MIN_SIZE=2;
    int capacity,tail;
    T *value;
};

template<typename V,typename C>
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
    void build(int u,int L,int R,V* value) {
        l[u]=L;  r[u]=R;
        if(L==R) {
            v[u]=value[L]; c[u]=C();
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
    void build(int L,int R,V *value) {
        build(1,L,R,value);
    }
    void modify(int L,int R,const C &change) {
        modify(1,L,R,change);
    }
    V query(int L,int R) {
        return query(1,L,R);
    }
};

Vector<int> merge_function(const Vector<int> &a,const Vector<int> &b) {
    Vector<int> result;
    int j=0;
    for(int i=0;i<a.size();++i) {
        while (j<b.size() && b[j]<a[i])
            result.push_back(b[j++]);
        while (j<b.size() && b[j]==a[i])
            ++j;
        if (i==0 || a[i-1]!=a[i])
            result.push_back(a[i]);
    }
    while (j<b.size())
        result.push_back(b[j++]);
    return result;
}

Vector<int> v[N];
SegTree<Vector<int>, Vector<int>> seg_tree(merge_function,merge_function,merge_function);

template<typename T>
void merge_sort(T *l,T *r) {
    static T tmp[N];
    if(r-l<=1) return;
    T *mid=l+((r-l)>>1),*t=tmp;
    merge_sort(l, mid);
    merge_sort(mid, r);
    T *i=l,*j=mid;
    while(i<mid && j<r) {
        if(*i<=*j)  *(t++)=*(i++);
        else        *(t++)=*(j++);
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

int n,m,top,l[N],r[N],k[M],s[S];

int main() {
    scanf("%d%d",&n,&m);
    for (int i=1; i<=n; ++i) {
        scanf("%d%d",&l[i],&r[i]);
        s[top++]=l[i];  s[top++]=r[i];
    }
    for (int i=1; i<=m; ++i) {
        scanf("%d",&k[i]);
        s[top++]=k[i];
    }
    merge_sort(s, s+top);
    top=unique(s, s+top)-s;
    seg_tree.build(0, top-1, v);
    for (int i=1; i<=n; ++i) {
        l[i]=lower_bound(s, s+top, l[i])-s;
        r[i]=lower_bound(s, s+top, r[i])-s;
        Vector<int> change;
        change.push_back(i);
        seg_tree.modify(l[i], r[i], change);
    }
    for (int i=1; i<=m; ++i) {
        k[i]=lower_bound(s, s+top, k[i])-s;
        Vector<int> result=seg_tree.query(k[i], k[i]);
        if (result.size()) {
            for (int j=0; j<result.size(); ++j) {
                printf("%d ",result[j]);
            }
            puts("");
        }
        else puts("-1");
    }
    return 0;
}
