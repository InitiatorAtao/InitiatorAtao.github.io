#include <algorithm>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

int insert,query,query_range;   //query_range is number of items that can be query, if query_range<=insert the query cannot be failed
std::vector<std::pair<std::string,std::pair<int,int>>> items;

int main(int argc,char * argv[]) {
    assert(argc>2);
    sscanf(argv[1], "%d", &insert);
    sscanf(argv[2], "%d", &query);
    if(argc<=3 || !sscanf(argv[3], "%d", &query_range))
        query_range=insert;
    freopen("poj.txt", "r", stdin);
    freopen("data.in", "w", stdout);
    std::FILE *ans=fopen("data.ans", "w");
    std::string n;
    int id,r,s;
    while (std::cin>>id>>n>>r>>s) {
        items.push_back(std::make_pair(n, std::make_pair(r, s)));
    }
    std::default_random_engine rng(time(0));
    std::mt19937 mt(rng());
    std::uniform_int_distribution<> randint(0,query_range-1);
    std::shuffle(items.begin(),items.end(),rng);
    if(argc>4) {//use mix query and insert
        std::vector<bool> is_insert;
        for(int i=0;i<insert;++i)   is_insert.push_back(true);
        for(int i=0;i<query;++i)    is_insert.push_back(false);
        std::shuffle(is_insert.begin(),is_insert.end(),rng);
        int p=0;
        for(bool i:is_insert)
            if(i) {
                std::cout<<"0 "<<items[p].first<<" "<<items[p].second.first<<std::endl;
                ++p;
            }
            else {
                id=randint(mt);
                std::cout<<"1 "<<items[id].first<<std::endl;
                fprintf(ans, "%d\n",id<p? items[id].second.first:-1);
            }
        std::cout<<"2 "<<std::endl;
    }
    else {//all queries are after insert
        for (int i=0; i<insert; ++i) {
            std::cout<<"0 "<<items[i].first<<" "<<items[i].second.first<<std::endl;
        }
        for (int i=0; i<query; ++i) {
            id=randint(mt);
            std::cout<<"1 "<<items[id].first<<std::endl;
            fprintf(ans, "%d\n",id<insert? items[id].second.first:-1);
        }
        std::cout<<"2 "<<std::endl;
    }
    return 0;
}
