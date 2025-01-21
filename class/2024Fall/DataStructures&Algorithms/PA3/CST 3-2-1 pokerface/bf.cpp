#include <cstdio>
#include <cstdlib>
const int N=1e5+10,M=1e5+10;

template<typename T>
T min(const T &a,const T &b) {
    return a<b? a:b;
}

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

template<typename T>
void merge_sort(T *l,T *r) {
    static T tmp[N];
    if(r-l<=1) return;
    T *mid=l+((r-l)>>1),*t=tmp;
    merge_sort(l, mid);
    merge_sort(mid, r);
    T *i=l,*j=mid;
    while(i<mid && j<r) {
        if(*i<=*j)  *(t++)=*(i++);
        else        *(t++)=*(j++);
    }
    while(i<mid)*(t++)=*(i++);
    while(j<r)  *(t++)=*(j++);
    while(tmp<t) *(--r)=*(--t);
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

Vector<int> v[N+N+M];

int n,m,top,l[N],r[N],k[M],s[N+N+M];

int main() {
    scanf("%d%d",&n,&m);
    for (int i=1; i<=n; ++i) {
        scanf("%d%d",&l[i],&r[i]);
        s[top++]=l[i];  s[top++]=r[i];
    }
    for (int i=1; i<=m; ++i) {
        scanf("%d",&k[i]);
        s[top++]=k[i];
    }
    merge_sort(s, s+top);
    for (int i=1; i<=n; ++i) {
        l[i]=lower_bound(s, s+top, l[i])-s;
        r[i]=lower_bound(s, s+top, r[i])-s;
        for (int j=l[i]; j<=r[i]; ++j) {
            v[j].push_back(i);
        }
    }
    for (int i=1; i<=m; ++i) {
        k[i]=lower_bound(s, s+top, k[i])-s;
        if (v[k[i]].size()) {
            for (int j=0; j<v[k[i]].size(); ++j) {
                printf("%d ",v[k[i]][j]);
            }
            puts("");
        }
        else puts("-1");
    }
    return 0;
}
