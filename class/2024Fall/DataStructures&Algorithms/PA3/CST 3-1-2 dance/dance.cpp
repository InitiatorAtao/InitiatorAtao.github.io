#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
const int N=5e5+10,M=5e5+10;

inline void swap(int &a,int &b) {
    a^=b; b^=a; a^=b;
}

template<typename T>
inline void swap(T &a,T &b) {
    T c=a; a=b; b=c;
}

template<typename A,typename B>
struct Pair {
    A a;
    B b;
};

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
            if(f) (key<k[f]? ls[f]:rs[f])=u;
            update(f);
            splay(u);
            return true;
        }
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
        K key=k[root];
        next(); search(key);
        pool[top++]=root;
        root=rs[root];
        fa[root]=0;
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
    inline void insert_kth(int k_th,K key,V value) {
        int u=new_node(key,value);
        if (k_th>s[root]) {
            ls[u]=root;
            update(u);
            root=u;
            return;
        }
        kth(k_th);
        ls[u]=ls[root];
        ls[root]=0;
        rs[u]=root;
        update(root);
        update(u);
        root=u;
    }
    inline Pair<K, V> remove_kth(int k_th) {
        K key;  V value;
        if (k_th==s[root]) {
            kth(k_th);
            key=k[root]; value=v[root];
            pushdown(root);
            pool[top++]=root;
            root=ls[root];
            fa[root]=0;
        }
        else {
            kth(k_th+1);
            kth(k_th);
            key=k[root]; value=v[root];
            pushdown(root);
            pushdown(rs[root]);
            assert(!ls[rs[root]]);
            ls[rs[root]]=ls[root];
            pool[top++]=root;
            root=rs[root];
            fa[root]=0;
            update(root);
        }
        return {key,value};
    }
    inline void rebase(int k) {
        if(k<=1) return;
        kth(k);
        r[ls[root]]^=1;
        kth(k-1);
        r[rs[root]]^=1;
        r[root]^=1;
    }
    inline void reverse(int left,int right) {
        int length=(right-left+s[root])%s[root]+1;
        rebase(left);
        if (length>=s[root]) {
            r[root]^=1;
        }
        else {
            kth(length+1);
            r[ls[root]]^=1;
        }
        rebase((s[root]-left+1)%s[root]+1);
    }
    inline int size() {
        return s[root];
    }
    inline void print(int u) {
        pushdown(u);
        if(ls[u]) print(ls[u]);
        printf("%d ",v[u]);
        if(rs[u]) print(rs[u]);
    }
    inline void print() {
        print(root);
        puts("");
    }
};

Splay<int, int> splay;
int n,m;
char s[10];

int main() {
    scanf("%d%d",&n,&m);
    while (m--) {
        scanf("%s",s);
        int a=0,b=0,c=0,d=0;
        switch (s[0]) {
            case 'I':
                scanf("%d%d",&a,&b);
                splay.insert_kth(b+1, 0, a);
                break;
            case 'S':
                scanf("%d%d",&a,&b);
                if(a==b) break;
                if(a>b) swap(a,b);
                a+=1;   b+=1;
                c=splay.remove_kth(b).b;
                d=splay.remove_kth(a).b;
                splay.insert_kth(a, 0, c);
                splay.insert_kth(b, 0, d);
                break;
            case 'R':
                scanf("%d",&a);
                b=splay.size();
                splay.rebase(((-a)%b+b)%b+1);
                break;
            case 'F':
                scanf("%d%d",&a,&b);
                splay.reverse(a+1, b+1);
        }
    }
    splay.print();
    return 0;
}
