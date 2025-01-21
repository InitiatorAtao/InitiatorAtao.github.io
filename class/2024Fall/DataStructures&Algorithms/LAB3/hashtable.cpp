#include "hashtable.h"
#include<cmath>
int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}

int bad_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    int result=0;
    for(char* i=str;*i;++i) {
        result=(result+(*i))%N;
    }
    return result;
}

int good_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    int result=0;
    for(char* i=str;*i;++i) {
        result=(result*31+(*i))%N;
    }
    return result;
}

int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}
void linear_probe::init(){
    // do nothing
}

int two_way_square_trial::operator()(hash_entry* Table, int table_size, int last_choice){
    if(hash_key==-1) return hash_key=last_choice;
    if(last_choice-hash_key>0) return ((2*hash_key-last_choice)%table_size+table_size)%table_size;
    int now_square=sqrt(hash_key-last_choice)+1.5;
    return ((hash_key+now_square*now_square)%table_size+table_size)%table_size;
}
void two_way_square_trial::init(){
    hash_key=-1;
}

int common_overflow_area::operator()(hash_entry* Table, int table_size, int last_choice){
    if(start) return start=0;
    return (last_choice+1)%table_size;
}
void common_overflow_area::init() {
    start=true;
}
