#include"book.h"
#include"library.h"

void Library::add_book(Book book)
{
    Books.push_back(book);
    cout<<"Added:"<<book.get_name()<<endl;
}
void Library::delete_book(int book_ID)
{
    for(int i=0;i<Books.size();++i)
        if(Books[i].get_ID()==book_ID)
        {
            cout<<"Deleted:"<<Books[i].get_name()<<endl;
            Books.erase(Books.begin()+i);
            return;
        }
    cout<<"Delete Failed"<<endl;
}
void Library::find_book(int book_ID)
{
    for(int i=0;i<Books.size();++i)
        if(Books[i].get_ID()==book_ID)
        {
            Books[i].print_message();
            return;
        }
    cout<<"Find Failed"<<endl;
}
void Library::borrow_book(int book_ID,string borrower)
{
    for(int i=0;i<Books.size();++i)
        if(Books[i].get_ID()==book_ID)
        {
            if(Books[i].get_state()==true)
                cout<<"Borrow Failed"<<endl;
            else
            {
                Books[i].change_state(borrower);
                cout<<"Borrowed:"<<Books[i].get_name()<<endl;
            }
            return;
        }
    cout<<"Borrow Failed"<<endl;
}
void Library::return_book(int book_ID)
{
    for(int i=0;i<Books.size();++i)
        if(Books[i].get_ID()==book_ID)
        {
            if(Books[i].get_state()==true)
            {
                Books[i].change_state();
                cout<<"Returned:"<<Books[i].get_name()<<endl;
            }
            else
                cout<<"Return Failed"<<endl;
            return;
        }
    cout<<"Return Failed"<<endl;
}