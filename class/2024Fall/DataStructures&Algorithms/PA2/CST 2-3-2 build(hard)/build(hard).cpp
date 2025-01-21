#include <cassert>
#include <cstdio>
#include <cstdlib>
const int N=1e6+10;

struct node {
    int from,to,prev,next;
}e[N];

int n,m,head[N],tail[N],weight[N],height[N],suffix_height[N],tot=1;
int vec[N],top,k;

inline int max(int a,int b) {
    return a<b? b:a;
}

inline void add(int f,int t) {
    e[++tot]={f,t,0,0};
    if (!head[f]) {
        head[f]=tail[f]=tot;
    }
    else {
        e[tail[f]].next=tot;
        e[tot].prev=tail[f];
        tail[f]=tot;
    }
}

inline void update_suffix(int i) {
    suffix_height[e[i].to]=max(height[e[i].to],suffix_height[e[e[i].next].to]);
}

inline void update_height(int u) {
    height[u]=suffix_height[e[head[u]].to]+1;
}

void dfs(int u) {
    weight[u]=1;
    for (int i=tail[u]; i; i=e[i].prev) {
        int v=e[i].to;
        dfs(v);
        weight[u]+=weight[v];
        update_suffix(i);
    }
    update_height(u);
    //printf("node %d has height %d, weight %d\n",u,height[u],weight[u]);
}

int delete_tree(int u,int l) {
    //printf("delete tree in %d with l=%d\n",u,l);
    int i=head[u];
    assert(vec[l]>=0);
    while(vec[l]--) i=e[i].next;
    if(!i) return 0;
    int root=0;
    if (l>=top-1 || !(root=delete_tree(e[i].to, l+1))) {
        root=i;
        if(e[i].prev) e[e[i].prev].next=e[i].next;
        else    head[u]=e[i].next;
        if(e[i].next) e[e[i].next].prev=e[i].prev;
        else    tail[u]=e[i].prev;
    }
    else update_suffix(i);
    for (i=e[i].prev; i; i=e[i].prev) update_suffix(i);
    update_height(u);
    weight[u]-=weight[e[root].to];
    return root;
}

void add_tree(int u,int l,int root) {
    //printf("add tree in u=%d,l=%d,vec[l]=%d\n",u,l,vec[l]);
    int i=head[u];
    if(l<top) while(vec[l]--) i=e[i].next;
    if (!i || l>=top) {
        i=head[u];
        //printf("end add tree with read i=%d\n",k);
        assert(k>=0);
        while(k--) assert(e[i].next || !k),i=e[i].next;
        e[root].from=u;
        if(i) {
            e[root].prev=e[i].prev;
            if(e[i].prev)   e[e[i].prev].next=root;
            else            head[u]=root;
            e[root].next=i;
            e[i].prev=root;
        }
        else {
            e[root].next=0;
            e[root].prev=tail[u];
            if(tail[u]) e[tail[u]].next=root;
            else        head[u]=root;
            tail[u]=root;
        }
        i=root;
    }
    else add_tree(e[i].to, l+1, root);
    for (; i; i=e[i].prev) update_suffix(i);
    update_height(u);
    weight[u]+=weight[e[root].to];
    //printf("now height [%d]=%d, weight [%d]=%d\n",u,height[u],u,weight[u]);
}

inline void move() {
    //puts("start move");
    int l=0;

    scanf("%d",&l);
    assert(l>0);  top=0;
    while(l--) scanf("%d",&vec[top++]);
    int root=delete_tree(1,0);

    top=l=0;
    scanf("%d",&l);
    while(l--) scanf("%d",&vec[top++]);
    scanf("%d",&k);
    add_tree(1,0,root);
}

inline void check_height() {
    //puts("start check height");
    int l=0,u=1;    top=0;
    scanf("%d",&l);
    while(l--) scanf("%d",&vec[top++]);
    for (int p=0; p<top; ++p) {
        int i=head[u];
        while(vec[p]--) i=e[i].next;
        if(i)   u=e[i].to;
        else    break;
    }
    printf("%d\n",height[u]-1);
}

inline void check_weight() {
    //puts("start check weight");
    int l=0,u=1;    top=0;
    scanf("%d",&l);
    while(l--) scanf("%d",&vec[top++]);
    for (int p=0; p<top; ++p) {
        int i=head[u];
        while(vec[p]--) i=e[i].next;
        if(i)   u=e[i].to;
        else    break;
    }
    printf("%d\n",weight[u]);
}

int main() {
    scanf("%d%d",&n,&m);
    for (int i=1; i<=n; ++i) {
        int s=0;
        scanf("%d",&s);
        for (int j=0; j<s; ++j) {
            int son=0;
            scanf("%d",&son);
            add(i, son);
        }
    }
    dfs(1);
    while (m--) {
        int op=0;
        scanf("%d",&op);
        switch (op) {
        case 0:move();break;
        case 1:check_height();break;
        case 2:check_weight();break;
        default:exit(1);
        }
    }
    return 0;
}
