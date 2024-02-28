#include "book.h"
#include <string.h>

void Book_initialize(Book *b) {
  b->id = 0;
  strcpy(b->name, "");
  strcpy(b->author, "");
  strcpy(b->publisher, "");
  strcpy(b->releaseDate, "00/00/0000");
  b->price = 0;
}

void Book_setName(Book *b, const char *name) {
  if (strlen(name) > MAX_NAME_SIZE) {
    return;
  }

  strcpy(b->name, name);
}

void Book_setAuthor(Book *b, const char *author) {
  if (strlen(author) > MAX_NAME_SIZE) {
    return;
  }

  strcpy(b->author, author);
}

void Book_setPublisher(Book *b, const char *publisher) {
  if (strlen(publisher) > MAX_NAME_SIZE) {
    return;
  }

  strcpy(b->publisher, publisher);
}

void Book_setReleaseDate(Book *b, const char *releaseDate) {
  if (strlen(releaseDate) > MAX_NAME_SIZE) {
    return;
  }

  strcpy(b->releaseDate, releaseDate);
}
