#include "book_manager.h"

void BookManager_initialize(BookManager *bm) {
  bm->database = NULL;
  BookManager_openDb(bm);
  BookManager_initializeDb(bm);
}

void BookManager_openDb(BookManager *bm) {
  int resCode;
  resCode = sqlite3_open(DATABASE_FILENAME, &bm->database);

  if (resCode != SQLITE_OK) {
    log_error("Error while opening database: %s", sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
}

void BookManager_destroy(BookManager *bm) {
  int resCode;
  resCode = sqlite3_open(DATABASE_FILENAME, &bm->database);

  if (resCode != SQLITE_OK) {
    log_error("Error while opening database: %s", sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
}

void BookManager_initializeDb(BookManager *bm) {
  int resCode;
  sqlite3_stmt *stmt;

  resCode = sqlite3_prepare_v2(bm->database,
                               "CREATE TABLE IF NOT EXISTS books ("
                               "id INTEGER PRIMARY KEY,"
                               "name TEXT NOT NULL,"
                               "author TEXT NOT NULL,"
                               "publisher TEXT NOT NULL,"
                               "releaseDate TEXT NOT NULL,"
                               "price REAL NOT NULL"
                               ");",
                               -1, &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  resCode = sqlite3_step(stmt);

  if (resCode != SQLITE_DONE) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
}

void BookManager_addBook(BookManager *bm, Book *book) {
  int resCode;
  sqlite3_stmt *stmt;

  resCode = sqlite3_prepare_v2(bm->database,
                               "INSERT INTO books (name, author, publisher, "
                               "releaseDate, price) VALUES (?, ?, ?, ?, ?);",
                               -1, &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  sqlite3_bind_text(stmt, 1, book->name, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, book->author, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, book->publisher, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, book->releaseDate, -1, SQLITE_TRANSIENT);
  sqlite3_bind_double(stmt, 5, book->price);

  resCode = sqlite3_step(stmt);

  if (resCode != SQLITE_DONE) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
}

void BookManager_updateBook(BookManager *bm, Book *book) {
  int resCode;
  sqlite3_stmt *stmt;

  resCode = sqlite3_prepare_v2(
      bm->database,
      "UPDATE books SET name = ?, author = ?, publisher = ?, "
      "releaseDate = ?, price = ? WHERE id = ?;",
      -1, &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  sqlite3_bind_text(stmt, 1, book->name, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, book->author, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, book->publisher, -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, book->releaseDate, -1, SQLITE_TRANSIENT);
  sqlite3_bind_double(stmt, 5, book->price);
  sqlite3_bind_int64(stmt, 6, book->id);

  resCode = sqlite3_step(stmt);

  if (resCode != SQLITE_DONE) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
}

void BookManager_removeBook(BookManager *bm, long int id) {
  int resCode;
  sqlite3_stmt *stmt;

  resCode = sqlite3_prepare_v2(bm->database, "DELETE FROM books WHERE id=?;",
                               -1, &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  sqlite3_bind_int64(stmt, 1, id);

  resCode = sqlite3_step(stmt);

  if (resCode != SQLITE_DONE) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
}

ListBookElement *BookManager_listBooks(BookManager *bm) {
  int resCode;
  sqlite3_stmt *stmt;
  ListBookElement *books;

  ListBookElement_initialize(&books);

  resCode =
      sqlite3_prepare_v2(bm->database, "SELECT * FROM books;", -1, &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  while (sqlite3_step(stmt) != SQLITE_DONE) {
    Book *book = (Book *)malloc(sizeof(Book));
    book->id = sqlite3_column_int64(stmt, 0);
    Book_setName(book, sqlite3_column_text(stmt, 1));
    Book_setAuthor(book, sqlite3_column_text(stmt, 2));
    Book_setPublisher(book, sqlite3_column_text(stmt, 3));
    Book_setReleaseDate(book, sqlite3_column_text(stmt, 4));
    book->price = sqlite3_column_double(stmt, 5);

    ListBookElement_push(&books, book);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  return books;
}

ListBookElement *BookManager_findByName(BookManager *bm, const char *name) {
  int resCode;
  sqlite3_stmt *stmt;
  ListBookElement *books;

  ListBookElement_initialize(&books);

  resCode = sqlite3_prepare_v2(
      bm->database, "SELECT * FROM books WHERE name LIKE ? COLLATE NOCASE;", -1,
      &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  char buffer[strlen(name) + 7];
  strcpy(buffer, "%");
  strcat(buffer, name);
  strcat(buffer, "%");
  resCode = sqlite3_bind_text(stmt, 1, buffer, -1, SQLITE_TRANSIENT);

  if (resCode != SQLITE_OK) {
    log_error("Error while binding statement: %s",
              sqlite3_errmsg(bm->database));
    log_error("Statement: %s\n", sqlite3_sql(stmt));
    exit(EXIT_FAILURE);
  }

  while (sqlite3_step(stmt) != SQLITE_DONE) {
    Book *book = (Book *)malloc(sizeof(Book));
    book->id = sqlite3_column_int64(stmt, 0);
    Book_setName(book, sqlite3_column_text(stmt, 1));
    Book_setAuthor(book, sqlite3_column_text(stmt, 2));
    Book_setPublisher(book, sqlite3_column_text(stmt, 3));
    Book_setReleaseDate(book, sqlite3_column_text(stmt, 4));
    book->price = sqlite3_column_double(stmt, 5);

    ListBookElement_push(&books, book);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  return books;
}

ListBookElement *BookManager_findByAuthor(BookManager *bm, const char *author) {
  int resCode;
  sqlite3_stmt *stmt;
  ListBookElement *books;

  ListBookElement_initialize(&books);

  resCode = sqlite3_prepare_v2(
      bm->database,
      "SELECT * FROM books WHERE author LIKE ? COLLATE NOCASE;", -1,
      &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }
  char buffer[strlen(author) + 7];
  strcpy(buffer, "%");
  strcat(buffer, author);
  strcat(buffer, "%");
  
  resCode =
      sqlite3_bind_text(stmt, 1, buffer, -1, SQLITE_TRANSIENT);

  if (resCode != SQLITE_OK) {
    log_error("Error while binding statement: %s",
              sqlite3_errmsg(bm->database));
    log_error("Statement: %s\n", sqlite3_sql(stmt));
    exit(EXIT_FAILURE);
  }

  while (sqlite3_step(stmt) != SQLITE_DONE) {
    Book *book = (Book *)malloc(sizeof(Book));
    book->id = sqlite3_column_int64(stmt, 0);
    Book_setName(book, sqlite3_column_text(stmt, 1));
    Book_setAuthor(book, sqlite3_column_text(stmt, 2));
    Book_setPublisher(book, sqlite3_column_text(stmt, 3));
    Book_setReleaseDate(book, sqlite3_column_text(stmt, 4));
    book->price = sqlite3_column_double(stmt, 5);

    ListBookElement_push(&books, book);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  return books;
}

Book BookManager_findById(BookManager *bm, long int id) {
  int resCode;
  sqlite3_stmt *stmt;
  Book book;

  resCode = sqlite3_prepare_v2(
      bm->database, "SELECT * FROM books WHERE id = ?;", -1, &stmt, NULL);

  if (resCode != SQLITE_OK) {
    log_error("Error while initializing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  sqlite3_bind_int64(stmt, 1, id);

  while (sqlite3_step(stmt) != SQLITE_DONE) {
    book.id = sqlite3_column_int64(stmt, 0);
    Book_setName(&book, sqlite3_column_text(stmt, 1));
    Book_setAuthor(&book, sqlite3_column_text(stmt, 2));
    Book_setPublisher(&book, sqlite3_column_text(stmt, 3));
    Book_setReleaseDate(&book, sqlite3_column_text(stmt, 4));
    book.price = sqlite3_column_double(stmt, 5);
  }

  resCode = sqlite3_finalize(stmt);

  if (resCode != SQLITE_OK) {
    log_error("Error while executing statement: %s",
              sqlite3_errmsg(bm->database));
    exit(EXIT_FAILURE);
  }

  return book;
}
