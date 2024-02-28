#ifndef BOOK_H
#define BOOK_H

#define MAX_NAME_SIZE 255
#define RELEASE_DATE_SIZE 12

typedef struct Book {
  long int id;
  char name[MAX_NAME_SIZE];
  char author[MAX_NAME_SIZE];
  char publisher[MAX_NAME_SIZE];
  char releaseDate[RELEASE_DATE_SIZE];
  float price;
} Book;

void Book_initialize(Book *b);
void Book_setName(Book *b, const char *name);
void Book_setAuthor(Book *b, const char *author);
void Book_setPublisher(Book *b, const char *publisher);
void Book_setReleaseDate(Book *b, const char *releaseDate);

#endif