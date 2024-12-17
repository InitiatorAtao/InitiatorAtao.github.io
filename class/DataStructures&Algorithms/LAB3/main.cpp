#include <cstdio>
#include "hashtable.h"

int main(int argc,char *argv[]){
    int type;
    char buffer[1000];int data;
    hashing_strategy *h=(argc>1 && argv[1][0]=='b')?
        (hashing_strategy*)
        new bad_hashing():new good_hashing();
    collision_strategy *c=(argc>2 && argv[2][0]=='l')?
        (collision_strategy*)
        new linear_probe():(argc>2 && argv[2][0]=='c')?
        (collision_strategy*)
        new common_overflow_area():
        new two_way_square_trial();
    hashtable table(TABLE_SIZE,h,c);
    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            printf("%d\n", table.query(buffer));
        }else break;
    }
    return 0;
}
