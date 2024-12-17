#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
const int N=1e6+10;

class point {
public:
    int x,y,id;
    bool operator<(const point& other)const {
        return x<other.x;
    }
    point operator-(const point& other)const {
        point result;
        result.x=x-other.x;
        result.y=y-other.y;
        result.id=0;
        return result;
    }
    long long operator*(const point& other)const {
        return 1ll*x*other.y-1ll*y*other.x;
    }
};

struct triangular {
    point a,b,c;
};

int n,sta[N],top;
point line[N];
std::vector<triangular> ans;
bool line_belong_delta;

inline bool line_belong(const point& p) {
    return (line[0].id<p.id)^(line[n-1].id<p.id)^line_belong_delta;
}

int main() {
    scanf("%d",&n);
    for (int i=0; i<n; ++i) {
        scanf("%d%d",&line[i].x,&line[i].y);
        line[i].id=i;
    }
    std::sort(line,line+n);
    line_belong_delta=line[0].id<line[n-1].id;
    sta[top++]=0;
    for (int i=1;i<n-1;++i) {
        //printf("solving point %d %d %d\n",line[i].x,line[i].y,line[i].id);
        if (line_belong(line[i])^line_belong(line[sta[top-1]])) {
            //puts("not the same line, clearing stack...");
            int save=sta[top-1];
            while (top>1) {
                ans.push_back(triangular{line[i],line[sta[top-1]],line[sta[top-2]]});
                --top;
            }
            sta[top-1]=save;
            sta[top++]=i;
        }
        else {
            //puts("poping...");
            while (top>1 && (((line[i]-line[sta[top-1]])*(line[sta[top-2]]-line[sta[top-1]])>0)^line_belong(line[i]))) {
                ans.push_back(triangular{line[i],line[sta[top-1]],line[sta[top-2]]});
                --top;
            }
            sta[top++]=i;
        }
        /*
        printf("now stack: ");
        for (int j=0; j<top; ++j) {
            printf("%d(%d) ",sta[j],line[sta[j]].id);
        }
        puts("");
        */
    }
    while (top>1) {
        ans.push_back(triangular{line[n-1],line[sta[top-1]],line[sta[top-2]]});
        --top;
    }
    for (triangular &i : ans) {
        printf("%d %d %d %d %d %d\n",i.a.x,i.a.y,i.b.x,i.b.y,i.c.x,i.c.y);
    }
    return 0;
}
