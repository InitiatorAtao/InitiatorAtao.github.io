#include<bits/stdc++.h>
typedef unsigned long long ULL;
typedef long long LL;
typedef long double LD;
using namespace std;
const int N=1e6+10,M=4e6+10,INF=0x3f3f3f3f,mod=100003;
const ULL ULLINF=0x3f3f3f3f3f3f3f3f;
inline int Max(int x,int y){return x>y?x:y;}
inline int Min(int x,int y){return x<y?x:y;}
inline void Swap(int &x,int &y){x^=y^=x^=y;}
inline int read(){
	char c=getchar();int x=0,f=1;
	while(c<'0'||'9'<c){if(c=='-')f=-1;c=getchar();}
	while('0'<=c&&c<='9'){x=x*10+c-'0';c=getchar();}
	return x*f;
}

struct edge
{
	int from,to,next;
}e[M];
int n,m,cnt=1,head[N],dist[N],ans[N];

bool vis[N];

void add(int f,int t)
{
	e[++cnt]={f,t,head[f]};
	head[f]=cnt;
}

void dijkstra(int u)
{
	priority_queue<pair<int,int> > q;
	q.push(make_pair(0,u));
	memset(dist,0x3f,sizeof dist);
	dist[u]=0;	ans[u]=1;
	while(q.size())
	{
		int now=q.top().second;	q.pop();
		if(vis[now])	continue;
//		printf("dijkstra in %d\n",now);
		vis[now]=1;
		for(int i=head[now];i;i=e[i].next)
		{
			int v=e[i].to;
			if(dist[now]+1<dist[v])
			{
				dist[v]=dist[now]+1;
				ans[v]=ans[now];
				q.push(make_pair(-dist[v],v));
			}
			else if(dist[now]+1==dist[v])
				ans[v]=(ans[v]+ans[now])%mod;
		}
	}
}

int main()
{
	n=read();	m=read();
	for(int i=1;i<=m;++i)
	{
		int x=read(),y=read();
		add(x,y);
		add(y,x);
	}
	dijkstra(1);
	for(int i=1;i<=n;++i)
		printf("%lld\n",ans[i]);
	return 0;
}
/*
检查文件读写
检查空间大小
暴力优先
不要头铁
适当换题
*/
