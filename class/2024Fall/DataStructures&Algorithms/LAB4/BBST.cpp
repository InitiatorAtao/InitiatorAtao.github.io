#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
const int N=1e6+10,INF=-1;

inline int max(int a,int b) {
    return a<b? b:a;
}

inline int abs(int a) {
    return a<0? -a:a;
}

template<typename A,typename B>
struct Pair {
    A a;
    B b;
};

template<typename T>
void swap(T &a,T &b) {
    T c=a;  a=b; b=c;
}

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
    inline K lower_bound(K key) {
        if(!root) return INF;
        else if(search(key)) return key;
        else if(k[root]<key) return k[root];
        else if(ls[root]) return prev(),k[root];
        else return INF;
    }
};

template<typename K,typename V>
class AVL {
private:
    int pool[N],ls[N],_hot,rs[N],fa[N],root;
    int h[N],size[N],cnt,top;
    K k[N];
    V v[N];
    V (*update_function)(V,K,V,V);
    inline int new_node() {
        int result=top? (pool[--top]):(++cnt);
        ls[result]=rs[result]=fa[result]=0;
        h[result]=size[result]=1;
        k[result]=K();
        v[result]=V();
        return result;
    }
    inline int taller_child(int root) {
        return h[ls[root]]>h[rs[root]]? ls[root]:rs[root];
    }
    inline void update(int root) {
        int a=ls[root],b=rs[root];
        if(a) fa[a]=root;
        if(b) fa[b]=root;
        h[root]=max(h[a], h[b])+1;
        size[root]=size[a]+size[b]+1;
        if (update_function) {
            v[root]=update_function(v[a],k[root],v[root],v[b]);
        }
    }
    inline int connect(int l,int m,int r,int a,int b,int c,int d) {
        ls[m]=l;    rs[m]=r;
        ls[l]=a;    rs[l]=b;
        ls[r]=c;    rs[r]=d;
        update(l);
        update(r);
        update(m);
        return m;
    }
    inline int balance(int root) {
        //printf("balance in %d:%d\n",k[root],v[root]);
        int s1=taller_child(root);
        int s2=taller_child(s1);
        switch ((int(s1==rs[root])<<1) | int(s2==rs[s1])) {
            case 0b00:
                return connect(s2,s1,root,ls[s2],rs[s2],rs[s1],rs[root]);
            case 0b01:
                return connect(s1,s2,root,ls[s1],ls[s2],rs[s2],rs[root]);
            case 0b10:
                return connect(root,s2,s1,ls[root],ls[s2],rs[s2],rs[s1]);
            case 0b11:
                return connect(root,s1,s2,ls[root],ls[s1],ls[s2],rs[s2]);
            default:
                exit(1);
        }
    }
    inline int update() {
        int result=0;
        if(_hot) update(_hot);
        for (int u=_hot; u; update(u),u=fa[u]) {
            //printf("update in %d:%d with h[ls=%d:%d]=%d,h[rs=%d:%d]=%d\n",k[u],v[u],k[ls[u]],v[ls[u]],h[ls[u]],k[rs[u]],v[rs[u]],h[rs[u]]);
            if (abs(h[ls[u]]-h[rs[u]])>=2) {
                if(fa[u]) {
                    int pa=fa[u];
                    int &son=(ls[pa]==u)? ls[pa]:rs[pa];
                    son=balance(u);
                    update(pa);
                    result=son;
                }
                else {
                    fa[result=root=balance(u)]=0;

#### Splay

                    使用类模板 ``
                }
            }
        }
        return result;
    }
    int lower_bound(int &root,K key) {
        if (!root) return root;
        else if (key<k[root]) {
            //puts("search ls");
            _hot=root;
            return lower_bound(ls[root],key);
        }
        else if (k[root]<key) {
            //puts("search rs");
            _hot=root;
            int result=lower_bound(rs[root],key);
            return result? result:root;
        }
        else return root;
    }
    int& search(int &root,K key) {
        //printf("searching in %d:%d with key %d\n",k[root],v[root],key);
        if (!root) return root;
        else if (key<k[root]) {
            //puts("search ls");
            _hot=root;
            return search(ls[root],key);
        }
        else if (k[root]<key) {
            //puts("search rs");
            _hot=root;
            return search(rs[root],key);
        }
        else return root;
    }
    int insert(int &root,K key,V value) {
        _hot=0;
        int &result=search(root,key);
        //printf("root=%d\n",root);
        if (!result) {
            result=new_node();
            //printf("fa[%d]=%d\n",result,_hot);
            fa[result]=_hot;
            k[result]=key;
            v[result]=value;
        }
        update();
        return result;
    }
    int remove_node(int &u) {
        //printf("removing %d:%d",k[u],v[u]);
        int result=u;
        int pa=fa[u];
        if (!ls[u] && !rs[u]) u=0;
        else if(!ls[u] || !rs[u]) {
            u=ls[u]+rs[u];
            fa[u]=pa;
        }
        else {
            _hot=rs[u];
            while(ls[_hot]) _hot=ls[_hot];
            swap(k[result],k[_hot]);
            swap(v[result],v[_hot]);
            result=_hot;
            _hot=fa[_hot];
            (ls[_hot]==result? ls[_hot]:rs[_hot])=rs[result];
            //printf("result=%d:%d,_hot=%d:%d\n",k[result],v[result],k[_hot],v[_hot]);
        }
        pool[top++]=result;
        update();
        return result;
    }
    int remove(int &root,K key) {
        _hot=0;
        return remove_node(search(root,key));
    }
    void print(int u) {
        //printf("%d:%d ls %d:%d rs %d:%d\n",k[u],v[u],k[ls[u]],v[ls[u]],k[rs[u]],v[rs[u]]);
        if(ls[u]) print(ls[u]);
        if(rs[u]) print(rs[u]);
    }
public:
    AVL() {
        memset(ls, 0, sizeof ls);
        memset(rs, 0, sizeof rs);
        memset(h, 0, sizeof h);
        memset(size, 0, sizeof size);
        memset(fa, 0, sizeof fa);
        memset(pool, 0, sizeof pool);
        root=cnt=top=_hot=0;
        for(int i=0;i<N;++i)
            k[i]=K(),v[i]=V();
        update_function=nullptr;
    }
    inline void registe_update_function(V (*f)(V,K,V,V)) {
        update_function=f;
    }
    void print(){
        print(root);
    }
    V& insert(K key,V value) {
        return v[insert(root,key,value)];
    }
    V remove(K key) {
        return v[remove(root,key)];
    }
    K lower_bound(K key) {
        int result=lower_bound(root,key);
        return result? k[result]:INF;
    }
};

AVL<int,int> avl;
Splay<int,int> splay;
int n;
char buff[1024];

int main(int argc,char ** argv) {
    scanf("%d",&n);
    if(argc<=1 || std::tolower(argv[1][0])=='a') {
        while(n--) {
            int v=0;
            scanf("%s%d",buff,&v);
            switch(buff[0]) {
                case 'A': avl.insert(v,0); break;
                case 'B': avl.remove(v); break;
                case 'C': printf("%d\n",avl.lower_bound(v)); break;
                default: exit(2);
            }
        }
    }
    else {
        while(n--) {
            int v=0;
            scanf("%s%d",buff,&v);
            switch(buff[0]) {
                case 'A': splay[v]=0; break;
                case 'B': splay.erase(v); break;
                case 'C': printf("%d\n",splay.lower_bound(v)); break;
                default: exit(2);
            }
        }
    }
    return 0;
}
