#include<bits/stdc++.h>
using namespace std;

int n=500930,m=501000,k=100;

int main() {
    freopen("filename.in","w",stdout);
    cout<<n<<" "<<m<<" "<<k<<endl;
    for(int i=0;i<n;++i) {
        cout<<'a';
    }
    cout<<endl;
    for(int i=0;i<70;++i) {
        cout<<'b';
    }
    for(int i=70;i<m;++i) {
        cout<<'a';
    }
    cout<<endl;
}