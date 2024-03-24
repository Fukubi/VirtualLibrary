#ifndef BOOK_MANAGER_H
#define BOOK_MANAGER_H

#include "book.h"
#include "listbook.h"
#include "log.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE_FILENAME "database.db"

typedef struct BookManager {
  sqlite3 *database;
} BookManager;

void BookManager_initialize(BookManager *bm);
void BookManager_destroy(BookManager *bm);
void BookManager_openDb(BookManager *bm);
void BookManager_initializeDb(BookManager *bm);
void BookManager_addBook(BookManager *bm, Book *book);
void BookManager_updateBook(BookManager *bm, Book *book);
void BookManager_removeBook(BookManager *bm, long int id);
ListBookElement *BookManager_listBooks(BookManager *bm);
ListBookElement *BookManager_findByName(BookManager *bm, const char *name);
ListBookElement *BookManager_findByAuthor(BookManager *bm, const char *name);
Book BookManager_findById(BookManager *bm, long int id);

#endif