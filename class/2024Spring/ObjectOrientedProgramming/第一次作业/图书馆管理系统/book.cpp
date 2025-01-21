#include"book.h"

Book::Book(int ID,string name,string author,string publishing_house)
:ID(ID),name(name),author(author),publishing_house(publishing_house)
{
    state=make_pair(0,"");
}

int Book::get_ID(){return ID;}
string Book::get_name(){return name;}
string Book::get_author(){return author;}
string Book::get_publishing_house(){return publishing_house;}
bool Book::get_state(){return state.first;}
void Book::change_state(){state=make_pair(0,"");}
void Book::change_state(string name){state=make_pair(1,name);}
void Book::print_message()
{
    /*
    ID:123456
    name:Design_Patterns
    author:John_Smith
    publishing_house:Publisher_A
    state:loanable
    */
   cout<<"ID:"<<ID<<endl;
   cout<<"name:"<<name<<endl;
   cout<<"author:"<<author<<endl;
   cout<<"publishing_house:"<<publishing_house<<endl;
   if(state.first)
        cout<<"borrower:"<<state.second<<endl;
   cout<<"state:"<<(state.first? "borrowed":"loanable")<<endl;
}