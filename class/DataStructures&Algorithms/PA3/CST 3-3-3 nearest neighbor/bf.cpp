#include <climits>
#include <cstdio>
#include <cstdlib>
typedef long long LL;
const int N=1e5+10;

template<typename T>
T min(const T& a,const T& b) {
    return a<b? a:b;
}

int d,n;
struct Point {
    int x[5];
    inline void read() {
        for (int i=0; i<d; ++i) {
            scanf("%d",&x[i]);
        }
    }
}p[N];

LL dist(const Point &a,const Point &b,int dimension) {
    LL result=0;
    for (int i=0; i<dimension; ++i) {
        result+=1ll*(a.x[i]-b.x[i])*(a.x[i]-b.x[i]);
    }
    return result;
}

int main() {
    scanf("%d%d",&d,&n);
    for (int i=0; i<n; ++i) {
        p[i].read();
    }
    int q=0;
    scanf("%d",&q);
    while (q--) {
        Point query;
        query.read();
        LL result=LONG_LONG_MAX;
        for (int i=0; i<n; ++i) {
            result=min(result,dist(p[i],query,d));
        }
        printf("%lld\n",result);
    }
    return 0;
}
