#include "feature.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
const int N=5e5+10,K=5e5+10,W=1e9+10,X=1e3+10;

int n,w[N],id[N],ls[N],rs[N],h[N],size[N],root,cnt;
long long sum[N];

inline int max(int a,int b) {
    return a<b? b:a;
}

inline int abs(int a) {
    return a<0? -a:a;
}

inline void swap(int &a,int &b) {
    a^=b;   b^=a;   a^=b;
}
template<typename T>
void swap(T &a,T &b) {
    T c=a; a=b; b=c;
}

template<typename A,typename B>
struct Pair {
    A a;
    B b;
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
                }
            }
        }
        return result;
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
    int& kth(int &root,int kk) {
        //printf("%dth in %d:%d(%d,%d)\n",kk,k[root],v[root],size[ls[root]],size[rs[root]]);
        if (!root) return root;
        else if (kk<=size[ls[root]]) {
            _hot=root;
            return kth(ls[root],kk);
        }
        else if (size[ls[root]]+1<kk) {
            _hot=root;
            return kth(rs[root],kk-size[ls[root]]-1);
        }
        else return root;
    }
    int remove_kth(int &root,int k) {
        _hot=0;
        return remove_node(kth(root,k));
    }
    V prefix_search(int root,K key) {
        if (!root) return V();
        else if (key<k[root]) {
            _hot=root;
            return prefix_search(ls[root],key);
        }
        else if (k[root]<key) {
            _hot=root;
            return update_function(v[ls[root]],k[root],v[root],prefix_search(rs[root],key));
        }
        else return update_function(v[ls[root]],k[root],v[root],V());
    }
    V prefix_kth(int root,int kk) {
        //printf("prefix %dth in %d:%d(%d,%d)\n",kk,k[root],v[root],size[ls[root]],size[rs[root]]);
        if (!root) return V();
        else if (kk<size[ls[root]]) {
            _hot=root;
            return prefix_kth(ls[root],kk);
        }
        else if (kk==size[ls[root]]) return v[ls[root]];
        else if (kk==size[ls[root]]+1) return update_function(v[ls[root]],k[root],v[root],V());
        else {
            _hot=root;
            return update_function(v[ls[root]],k[root],v[root],prefix_kth(rs[root],kk-size[ls[root]]-1));
        }
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
    V& search(K key) {
        return v[search(root,key)];
    }
    Pair<K,V&> kth(int k) {
        int result=kth(root,k);
        return Pair<K,V&>{w[result],v[result]};
    }
    Pair<K,V> remove_kth(int kk) {
        int result=remove_kth(root,kk);
        return Pair<K,V>{k[result],v[result]};
    }
    V prefix_search(K key) {
        return prefix_search(root,key);
    }
    V prefix_kth(int k) {
        return prefix_kth(root,k);
    }
};

class Weight {
public:
    int w;
    Weight(int ww=0):w(ww){}
    bool operator<(const Weight& other) {
        return abs(w)>abs(other.w);
    }
};

class Linear {
public:
    int x;
    long long sum;
    Linear(int xx=0,long long s=0):x(xx),sum(s){}
};

Linear update_function(Linear l,Weight w,Linear m,Linear r) {
    //printf("calculating update %lld+%d*%d+%lld\n",l.sum,w.w,m.x,r.sum);
    return Linear{m.x,l.sum+1ll*w.w*m.x+r.sum};
}

AVL<int,int> weight_sample;
AVL<Weight,Linear> abs_weight_linear;

void init(int nn, const int *ww, const int *x){	
    //puts("initing...");
    n=nn;
    abs_weight_linear.registe_update_function(update_function);
    for (int i=1; i<=n; ++i) {
        w[i]=ww[i];
        weight_sample.insert(w[i], x[i]);
        abs_weight_linear.insert(Weight{w[i]}, Linear{x[i],1ll*w[i]*x[i]});
    }
    //puts("init finished");
    //weight_sample.print();
}
	
void modify_weight(int i, int ww){
    //printf("modifying %dth weight %d\n",i,ww);
    int x=weight_sample.remove(w[i]);
    abs_weight_linear.remove(Weight{w[i]});
    w[i]=ww;
    //weight_sample.print();
    //printf("inserting %d:%d\n",ww,x);
    weight_sample.insert(w[i], x);
    abs_weight_linear.insert(Weight{w[i]}, Linear{x,1ll*w[i]*x});
    //puts("modify finished");
    //weight_sample.print();
}
	
void modify_value(int k, int x){
    //printf("modifying %dth large weight's value to %d\n",k,x);
    //weight_sample.print();
    Pair<int,int> w_x=weight_sample.remove_kth(n-k+1);
    abs_weight_linear.remove(Weight{w_x.a});
    //printf("find node %d:%d\n",w_x.a,w_x.b);
    //weight_sample.print();
    weight_sample.insert(w_x.a, x);
    abs_weight_linear.insert(Weight{w_x.a}, Linear{x,1ll*w_x.a*x});
    //puts("modify finished");
    //weight_sample.print();
}

long long calculate(int k){
    //printf("calculating %d\n",k);
    return abs_weight_linear.prefix_kth(k).sum;
}
