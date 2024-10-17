#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char title[50];
    char author[20];
    char publisher[30];
    char date[10];
} Book;

Book books[500];
int book_count = 0;

int compare_books(Book a, Book b)
{
    return strcmp(a.title, b.title);
}

void swap_books(Book *a, Book *b)
{
    Book temp = *a;
    *a = *b;
    *b = temp;
}

// insertion sort
void sort_books()
{
    for (int i = 1; i < book_count; i++)
    {
        for (int j = i; j > 0 && compare_books(books[j], books[j - 1]) < 0; j--)
        {
            swap_books(&books[j], &books[j - 1]);
        }
    }
}

void read_books()
{
    FILE *file = fopen("books.txt", "r");
    while (fscanf(file, "%s %s %s %s", books[book_count].title, books[book_count].author, books[book_count].publisher, books[book_count].date) != EOF)
    {
        book_count++;
    }
    fclose(file);
    sort_books();
}

void write_books()
{
    FILE *file = fopen("ordered.txt", "w");
    for (int i = 0; i < book_count; i++)
    {
        fprintf(file, "%-50s%-20s%-30s%-10s\n", books[i].title, books[i].author, books[i].publisher, books[i].date);
    }
    fclose(file);
}

void insert_book()
{
    scanf("%s %s %s %s", books[book_count].title, books[book_count].author, books[book_count].publisher, books[book_count].date);
    book_count++;
    sort_books();
}

void search_books()
{
    char keyword[50];
    scanf("%s", keyword);
    for (int i = 0; i < book_count; i++)
    {
        if (strstr(books[i].title, keyword) != NULL)
        {
            printf("%-50s%-20s%-30s%-10s\n", books[i].title, books[i].author, books[i].publisher, books[i].date);
        }
    }
}

void delete_books()
{
    char keyword[50];
    scanf("%s", keyword);
    int i = 0;
    while (i < book_count)
    {
        if (strstr(books[i].title, keyword) != NULL)
        {
            for (int j = i; j < book_count - 1; j++)
            {
                books[j] = books[j + 1];
            }
            book_count--;
        }
        else
        {
            i++;
        }
    }
}

int main()
{
    read_books();

    while (1)
    {
        int operation;
        scanf("%d", &operation);

        if (operation == 0) // 0 -> write books to file and exit
        {
            write_books();
            break;
        }
        else if (operation == 1) // 1 -> insert book
        {
            insert_book();
        }
        else if (operation == 2) // 2 -> search books
        {
            search_books();
        }
        else if (operation == 3) // 3 -> delete books
        {
            delete_books();
        }
    }

    return 0;
}