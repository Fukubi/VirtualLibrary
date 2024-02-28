#include "listbook.h"

void ListBookElement_initialize(ListBookElement **head) { *head = NULL; }

void ListBookElement_push(ListBookElement **head, Book *book) {
  ListBookElement *newEl;
  ListBookElement **it;

  it = head;

  newEl = (ListBookElement *)malloc(sizeof(ListBookElement));
  newEl->book = book;
  newEl->next = NULL;

  while (*it != NULL) {
    it = &((*it)->next);
  }

  *it = newEl;
}

void ListBookElement_popBegin(ListBookElement **head) {
  ListBookElement *oldHead;

  if (ListBookElement_isEmpty(*head)) {
    return;
  }

  oldHead = *head;
  *head = (*head)->next;

  free(oldHead);
}

void ListBookElement_popEnd(ListBookElement **head) {
  ListBookElement **it;

  if (ListBookElement_isEmpty(*head)) {
    return;
  }

  it = head;

  while ((*it)->next->next != NULL) {
    it = &((*it)->next);
  }

  free((*it)->next);
  (*it)->next = NULL;
}

int ListBookElement_isEmpty(ListBookElement *head) { return head == NULL; }

void ListBookElement_destroy(ListBookElement **head) {
  while (!ListBookElement_isEmpty(*head)) {
    ListBookElement_popBegin(head);
  }
}
