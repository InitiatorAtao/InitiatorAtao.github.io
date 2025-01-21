#include "nearest_neighbor.cpp"
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
