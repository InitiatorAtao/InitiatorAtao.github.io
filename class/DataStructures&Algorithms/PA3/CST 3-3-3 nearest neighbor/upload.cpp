#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
const int N=1e5+10,Q=2e5+10;

template<typename T>
inline void swap(T& a,T &b) {
    T c=b;  b=a;    a=c;
}

template<typename T>
inline T min(const T& a,const T& b) {
    return a<b? a:b;
}

template<typename T>
T* kth(T* l,T* r,int k,std::function<bool(const T&,const T&)> cmp=std::less<T>()) {
    if(r-l<k)   return nullptr;
    T *mid=l,*left=l;
    for(T* i=l+1;i<r;++i)
        if(!cmp(*mid,*i))        //*i<=*mid
            swap(*(++left),*i);
    swap(*mid, *(left++));
    return left-l<k? kth(left,r,k-(left-l),cmp):
        (left-l==k? left-1:kth(l,left-1,k,cmp));
}

template<typename T> 
T* median(T* l,T* r,std::function<bool(const T&,const T&)> cmp=std::less<T>()) {
    return kth(l,r,((r-l)>>1)+1,cmp);
}

template<typename P>    //P means "point", needs to support cmp(const &P a,const &P b,int dim) to compare (operator<) in given 0-based dimension. Also need dist(P,P,dim) to calculate distance, single_dist(P,P,dim) to calculate distance in one special dimension
class KDTree {
private:
    static const int LEAF_POINTS_COUNT=75;
    int dim,head[N<<2],size[N],cnt;
    P p[N<<2],pool[N];
    bool used[N<<2];
    void build(int u,int d,P* l,P* r) {
        if (r<=l+LEAF_POINTS_COUNT) {
            if(l<r) {
                used[u]=1;
                head[u]=cnt+1;
                size[cnt+1]=r-l;
                for (P *i=l; i<r; ++i) {
                    pool[++cnt]=*i;
                }
            }
            return;
        }
        used[u]=1;
        P *mid=median(l,r,std::function<bool(const P&,const P&)>([&](const P& a,const P& b){
            return cmp(a,b,d%dim);}));
        p[u]=*mid;
        build(u<<1, d+1, l, mid);
        build(u<<1|1, d+1, mid+1, r);
    }
    template<typename D>            //distance, result of dist(P,P,dim) and single_dist(P,P,dim)
    D nearest_neighbor(int u,int d,const P &query) {
        D result=dist(p[u],query,dim);
        if(head[u]) {
            result=dist(pool[head[u]],query,dim);
            for(int i=1;i<size[head[u]];++i) {
                result=min(result,dist(pool[head[u]+i],query,dim));
            }
            return result;
        }
        int next=cmp(p[u],query,d%dim)?
            (u<<1|1):(u<<1);
        if(used[next]) {
            result=min(result,nearest_neighbor<D>(next,d+1,query));
        }
        if(used[next^1] && single_dist(p[u],query,d%dim)<result) {
            result=min(result,nearest_neighbor<D>(next^1,d+1,query));
        }
        return result;
    }
public:
    void print(int u=1) {
        if(!used[u]) return;
        if(!head[u]) printf("(%d,%d) ls %d(%d,%d) rs %d(%d,%d)\n",p[u].x[0],p[u].x[1],head[u<<1],p[u<<1].x[0],p[u<<1].x[1],head[u<<1|1],p[u<<1|1].x[0],p[u<<1|1].x[1]);
        else {
            for(int i=0;i<size[head[u]];++i)
                printf("(%d,%d) ",pool[head[u]+i].x[0],pool[head[u]+i].x[1]);
            puts("");
        }
        print(u<<1);
        print(u<<1|1);
    }
    void build(P* l,P* r) {build(1,0,l,r);}
    KDTree(int dimension=2):dim(dimension),cnt(0) {
        memset(used,0,sizeof used);
        memset(head,0,sizeof head);
        memset(size,0,sizeof size);
        for (int i=0,lim=N<<2; i<lim; ++i) {
            p[i]=P();
        }
        for(int i=0;i<N;++i) {
            pool[i]=P();
        }
    }
    template<typename D>        //distance
    D nearest_neighbor(const P &query) {
        return nearest_neighbor<D>(1,0,query);
    }
    int dimension() {return dim;}
    void set_dimension(int dimension) {
        dim=dimension;
    }
};
#include <cstdio>
typedef long long LL;

int d;
struct Point {
    int x[5];
    inline void read() {
        for (int i=0; i<d; ++i) {
            scanf("%d",&x[i]);
        }
    }
}p[N];

bool cmp(const Point &a,const Point &b,int dimension) {
    return a.x[dimension]<b.x[dimension];
}

inline LL dist(const Point &a,const Point &b,int dimension) {
    LL result=0;
    for (int i=0; i<dimension; ++i) {
        result+=1ll*(a.x[i]-b.x[i])*(a.x[i]-b.x[i]);
    }
    return result;
}

inline LL single_dist(const Point &a,const Point &b,int dimension) {
    LL dist=a.x[dimension]-b.x[dimension];
    return dist*dist;
}

KDTree<Point> kd_tree;
int n,q;

int main() {
    scanf("%d%d",&d,&n);
    kd_tree.set_dimension(d);
    for (int i=0; i<n; ++i) {
        p[i].read();
    }
    kd_tree.build(p, p+n);
    //kd_tree.print();
    scanf("%d",&q);
    while (q--) {
        Point query;
        query.read();
        //printf("%d %d ",result.x[0],result.x[1]);
        printf("%lld\n",kd_tree.nearest_neighbor<LL>(query));
    }
    return 0;
}
