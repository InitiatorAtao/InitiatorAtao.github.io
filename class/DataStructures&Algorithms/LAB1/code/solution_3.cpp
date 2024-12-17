#include <cstdio>

int matrix[2010][2010];
long long sum[2010][2010];

int main() {
    int n, m, q;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            scanf("%d", &matrix[i][j]);
        }
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            sum[i][j] = sum[i][j - 1] + sum[i - 1][j] - sum[i-1][j-1] + matrix[i][j];
        }
    }
    scanf("%d", &q);
    for (int i = 1; i <= q; ++i) {
        int x, y, a, b;
        scanf("%d %d %d %d", &x, &y, &a, &b);
        printf("%lld\n",sum[x+a-1][y+b-1]-sum[x-1][y+b-1]-sum[x+a-1][y-1]+sum[x-1][y-1]);
    }
    return 0;
}