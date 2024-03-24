#ifndef LIST_BOOK_H
#define LIST_BOOK_H

#include "book.h"
#include <stdlib.h>

typedef struct ListBookElement {
  Book *book;
  struct ListBookElement *next;
} ListBookElement;

void ListBookElement_initialize(ListBookElement **head);
void ListBookElement_push(ListBookElement **head, Book *book);
void ListBookElement_popBegin(ListBookElement **head);
void ListBookElement_popEnd(ListBookElement **head);
int ListBookElement_isEmpty(ListBookElement *head);
void ListBookElement_destroy(ListBookElement **head);

#endif