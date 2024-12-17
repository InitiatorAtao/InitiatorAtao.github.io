#include <cstdio>
#include <cstdlib>
#include <cstring>
const int N=1e5+10,M=1e5+10,T=1e4+10,mod=1e9+7;

template<typename T>
class vector {
public:
    vector() {
        value=new T[MIN_SIZE];
        capacity=MIN_SIZE;
        tail=0;
    }
    vector operator=(const vector<T> other) {
        capacity=other.capacity;
        tail=other.tail;
        value=new T[capacity];
        for(int i=0; i<tail; ++i) {
            value[i]=other.value[i];
        }
        return (*this);
    }
    vector(const vector<T> &other) {
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
    void push_back(T x) {
        if (tail>=capacity) expand();
        value[tail++]=x;
    }
    T pop_back() {
        if (!tail) exit(1);
        T result=value[--tail];
        shrink();
        return result;
    }
    T& back() {
        return value[tail-1];
    }
    T& operator[](int i) {
        if(i>=tail) exit(1);
        return value[i];
    }
    T operator[](int i)const{
        if(i>=tail) exit(1);
        return value[i];
    }
    inline void clear() {
        delete [] value;
        value=new T[MIN_SIZE];
        capacity=MIN_SIZE;
        tail=0;
    }
    inline int size()const{return tail;}
    ~vector() {delete [] value;}
    bool operator==(const vector<T>& other)const {
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

template<typename T>
inline void swap(T &a,T &b) {
    T c=a; a=b; b=c;
}

template<typename T>
inline void max(const T &a,const T &b) {
    return a<b? b:a;
}

template<typename T>
class priority_queue {
public:
    int size() {return v.size()-1;}
    const T& top() {
        return v[1];
    }
    void push(const T &a) {
        v.push_back(a);
        int u=v.size()-1;
        while((u>>1) && v[u>>1]<v[u])
            swap(v[u],v[u>>1]),u>>=1;
    }
    T pop() {
        T result=v[1];
        swap(v[1], v.back());
        v.pop_back();
        int u=1;
        while(((u<<1)<v.size() && v[u]<v[u<<1]) || 
              ((u<<1|1)<v.size() && v[u]<v[u<<1|1])) {
            int m=((u<<1|1)<v.size() && v[u<<1]<v[u<<1|1])?
                (u<<1|1):(u<<1);
            swap(v[u], v[m]);
            u=m;
        }
        return result;
    }
    inline void clear() {
        v.clear();
        v.push_back(T());
    }
    priority_queue() {
        v.push_back(T());
    }
private:
    vector<T> v;
};

template<typename X,typename Y>
class pair {
public:
    X x;    Y y;
    pair(X xx=X(),Y yy=Y()):x(xx),y(yy) {}
    inline bool operator<(const pair<X,Y> &other) {
        return ((x<other.x)^(other.x<x))?
            x<other.x:y<other.y;
    }
};

vector<pair<int,int> > e[N];
priority_queue<pair<int,int> > q;
int dist[N],count[N];
bool vis[N];

inline void dijkstra(int s) {
    q.clear();
    memset(dist,0x3f,sizeof dist);
    memset(count,0,sizeof count);
    memset(vis, 0, sizeof vis);
    dist[s]=0;  count[s]=1;
    q.push(pair<int,int>(0,s));
    while(q.size()) {
        int u=q.pop().y;
        if(vis[u]) continue;
        vis[u]=1;
        for(int i=0;i<e[u].size();++i) {
            int v=e[u][i].x,w=e[u][i].y;
            if(dist[u]+w<dist[v]) {
                dist[v]=dist[u]+w;
                count[v]=count[u];
                q.push(pair<int,int>(-dist[v],v));
            }
            else if(dist[u]+w==dist[v])
                count[v]=(1ll*count[v]+count[u])%mod;
        }
    }
}

int n,m,t[N];

int main() {
    scanf("%d%d",&n,&m);
    /*
    while(m--) {
        int u=0,v=0;
        scanf("%d%d",&u,&v);
        e[u].push_back(pair<int,int>(v,1));
        e[v].push_back(pair<int,int>(u,1));
    }
    dijkstra(1);
    for(int i=1;i<=n;++i) {
        printf("%d\n",count[i]);
    }
    */
    for(int i=1;i<=n;++i) {
        scanf("%d",&t[i]);
    }
    while(m--) {
        int u=0,v=0;
        scanf("%d%d",&u,&v);
        e[u].push_back(pair<int,int>(v,t[u]));
        e[v].push_back(pair<int,int>(u,t[v]));
    }
    dijkstra(1);
    printf("%d\n%d",dist[n]+t[n],count[n]);
    return 0;
}
