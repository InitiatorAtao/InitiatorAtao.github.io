#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
const int N=1e5+10,M=1e5+10,S=N+N+M;

template<typename T>
inline void swap(T &a,T &b) {
    T c=a; a=b; b=c;
}

template<typename A,typename B>
struct Pair {
    A a;
    B b;
};

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

Vector<int> ans[M];
int stack[N];
template<typename K,typename V>
class Splay {
private:
    int ls[N],rs[N],fa[N],s[N],pool[N],root,top,cnt;
    bool r[N];
    K k[N];
    V v[N];
    inline int new_node(K key=K(),V value=V()) {
        int u=(top? pool[--top]:++cnt);
        ls[u]=rs[u]=fa[u]=r[u]=0;
        s[u]=1;
        k[u]=key;   v[u]=value;
        return u;
    }
    inline void pushdown(int u) {
        if(!r[u]) return;
        swap(ls[u],rs[u]);
        if(ls[u]) r[ls[u]]^=1;
        if(rs[u]) r[rs[u]]^=1;
        r[u]=0;
    }
    inline void update(int u) {
        if(!u) return;
        int l=ls[u],r=rs[u];
        if(l) fa[l]=u;
        if(r) fa[r]=u;
        s[u]=s[l]+s[r]+1;
    }
    inline void right(int u) {//右旋
        int f=fa[u];
        int p=fa[f];
        ls[f]=rs[u];
        rs[u]=f;
        if(p) (ls[p]==f? ls[p]:rs[p])=u;
        else fa[u]=0,root=u;
        update(f);
        update(u);
        update(p);
    }
    inline void left(int u) {//左旋
        int f=fa[u];
        int p=fa[f];
        rs[f]=ls[u];
        ls[u]=f;
        if(p) (ls[p]==f? ls[p]:rs[p])=u;
        else fa[u]=0,root=u;
        update(f);
        update(u);
        update(p);
    }
    inline void rotate(int u) {//单旋
        if (!fa[u]) return;
        (ls[fa[u]]==u)? right(u):left(u);
    }
    inline bool rotate2(int u) {//双旋
        int f=fa[u];
        int p=fa[f];
        if (!p) return false;
        if ((ls[f]==u)^(ls[p]==f))  rotate(u),rotate(u);
        else                        rotate(f),rotate(u);
        return true;
    }
    inline void splay(int u) {//伸展至根
        while(rotate2(u));
        rotate(u);
    }
    inline bool search(K key) {
        int u=root,f=0;
        while (u && k[u]!=key) {
            pushdown(u);
            f=u; u=(key<k[u])? ls[u]:rs[u];
        }
        pushdown(u);
        splay(u? u:f);
        return u;
    }
    inline bool insert(K key,V value) {
        int u=root,f=0;
        while (u && k[u]!=key) {
            pushdown(u);
            f=u; u=(key<k[u])? ls[u]:rs[u];
        }
        if(!u) {
            u=new_node(key,value);
            if(f)   (key<k[f]? ls[f]:rs[f])=u;
            else    root=u;
            update(f);
            splay(u);
            return true;
        }
        splay(u);
        return false;
    }
    inline void prev() {
        pushdown(root);
        int u=ls[root];
        pushdown(u);
        while(rs[u]) u=rs[u],pushdown(u);
        splay(u);
    }
    inline void next() {
        int u=rs[root];
        pushdown(root);
        pushdown(u);
        while(ls[u]) u=ls[u],pushdown(u);
        splay(u);
    }
    inline void remove() {
        //puts("removing: ");
        //print();
        K key=k[root];
        next(); search(key);
        pool[top++]=root;
        if (rs[root]) {
            assert(!ls[rs[root]]);
            ls[rs[root]]=ls[root];
            root=rs[root];
        }
        else root=ls[root];
        fa[root]=0;
        update(root);
        //puts("removed: ");
        //print();
    }
    inline bool remove(K key) {
        if(!search(key)) return false;
        remove();
        return true;
    }
    inline void kth(int k) {
        int u=root;
        pushdown(u);
        while (u && s[ls[u]]+1!=k) {
            if (k<s[ls[u]]+1) u=ls[u];
            else k-=s[ls[u]]+1,u=rs[u];
            pushdown(u);
        }
        splay(u);
    }
public:
    Splay() {
        memset(ls, 0, sizeof ls);
        memset(rs, 0, sizeof rs);
        memset(fa, 0, sizeof fa);
        memset(s, 0, sizeof s);
        memset(pool, 0, sizeof pool);
        memset(r, 0, sizeof r);
        root=top=cnt=0;
        for (int i=0; i<N; ++i) {
            k[i]=K();
            v[i]=V();
        }
    }
    inline int size() const{return s[root];}
    inline V& operator[](K key) {
        insert(key, V());
        return v[root];
    }
    inline bool erase(K key) {
        return remove(key);
    }
    inline void print(int id) {
        if (root) {
            int u=root,top=0;
            while (u || top) {
                while(u) stack[top++]=u,u=ls[u];
                u=stack[--top];
                ans[id].push_back(v[u]);
                u=rs[u];
            }
        }
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

struct operation {
    int i,id,op;
    bool operator<(const operation &other) {
        if(i==other.i) return op<other.op;
        return i<other.i;
    }
}s[S];

Splay<int, int> splay;
int n,m,top;

int main() {
    scanf("%d%d",&n,&m);
    for (int i=1; i<=n; ++i) {
        int l=0,r=0;
        scanf("%d%d",&l,&r);
        s[top++]={l,i,0};  //flip come in
        s[top++]={r,i,2};  //flip come out
    }
    for (int i=1; i<=m; ++i) {
        int k=0;
        scanf("%d",&k);
        s[top++]={k,i,1};  //check flip
    }
    merge_sort(s, s+top);
    for (int i=0; i<top; ++i) {
        //printf("operation %d %d %d\n",s[i].i,s[i].id,s[i].op);
        switch (s[i].op) {
            case 0:
                splay[s[i].id]=s[i].id;
                //splay.print();
                break;
            case 1:
                splay.print(s[i].id);
                break;
            case 2:
                assert(splay.erase(s[i].id));
                //splay.print();
        }
    }
    for (int i=1; i<=m; ++i) {
        if (ans[i].size()) {
            for (int j=0; j<ans[i].size(); ++j) {
                printf("%d ",ans[i][j]);
            }
            puts("");
        }
        else puts("-1");
    }
    return 0;
}
