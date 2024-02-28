#include "book.h"
#include "book_manager.h"
#include "listbook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  BookManager bm;
  BookManager_initialize(&bm);
  printf("\t=====================\t\n");
  printf("\tLivraria Digital V0.1\t\n");
  printf("\t=====================\t\n");

  while (1) {
    short choice;

    printf("Menu Principal:\n");
    printf("0. Sair\n");
    printf("1. Novo Livro\n");
    printf("2. Listar Livros\n");
    printf("3. Procurar por nome\n");
    printf("4. Procurar por identificador\n");
    printf("5. Atualizar livro\n");
    printf("6. Remover livro\n");
    printf("> ");
    scanf("%hd", &choice);
    getchar();

    if (choice == 0) {
      break;
    } else if (choice == 1) {
      Book book;
      double doubleBuffer;
      char buffer[255];

      printf("Digite o nome do livro: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';

      Book_setName(&book, buffer);

      printf("Digite o autor do livro: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';
      Book_setAuthor(&book, buffer);

      printf("Digite o nome da editora do livro: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';
      Book_setPublisher(&book, buffer);

      printf("Digite a data de lancamento do livro (dd/MM/YYYY): ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';
      Book_setReleaseDate(&book, buffer);

      printf("Digite o preco do livro: ");
      scanf("%lf", &doubleBuffer);
      getchar();
      book.price = doubleBuffer;

      BookManager_addBook(&bm, &book);
    } else if (choice == 2) {
      ListBookElement *books;

      books = BookManager_listBooks(&bm);

      for (ListBookElement *it = books; it != NULL; it = it->next) {
        printf("========================\n");
        printf("ID: %d\n", it->book->id);
        printf("Nome: %s\n", it->book->name);
        printf("Autor: %s\n", it->book->author);
        printf("Editora: %s\n", it->book->publisher);
        printf("Data de lancamento: %s\n", it->book->releaseDate);
        printf("Preco: %.2lf\n", it->book->price);
      }

      ListBookElement_destroy(&books);
    } else if (choice == 3) {
      ListBookElement *books;
      char buffer[255];

      printf("Digite o nome do livro: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';

      books = BookManager_findByName(&bm, buffer);

      for (ListBookElement *it = books; it != NULL; it = it->next) {
        printf("========================\n");
        printf("ID: %d\n", it->book->id);
        printf("Nome: %s\n", it->book->name);
        printf("Autor: %s\n", it->book->author);
        printf("Editora: %s\n", it->book->publisher);
        printf("Data de lancamento: %s\n", it->book->releaseDate);
        printf("Preco: %.2lf\n", it->book->price);
      }

      ListBookElement_destroy(&books);
    } else if (choice == 4) {
      Book book;
      long int searchId;

      printf("Digite o identificador do livro: ");
      scanf("%ld", &searchId);
      getchar();

      book = BookManager_findById(&bm, searchId);

      printf("========================\n");
      printf("ID: %d\n", book.id);
      printf("Nome: %s\n", book.name);
      printf("Autor: %s\n", book.author);
      printf("Editora: %s\n", book.publisher);
      printf("Data de lancamento: %s\n", book.releaseDate);
      printf("Preco: %.2lf\n", book.price);
    } else if (choice == 5) {
      Book book;
      long int searchId;
      double doubleBuffer;
      char buffer[255];

      printf("Digite o identificador do livro: ");
      scanf("%ld", &searchId);
      getchar();

      book = BookManager_findById(&bm, searchId);

      printf("Digite o novo nome do livro [vazio para nao alterar]: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';

      if (strlen(buffer) != 0) {
        Book_setName(&book, buffer);
      }

      printf("Digite o autor do livro [vazio para nao alterar]: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';

      if (strlen(buffer) != 0) {
        Book_setAuthor(&book, buffer);
      }

      printf("Digite o nome da editora do livro [vazio para nao alterar]: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';

      if (strlen(buffer) != 0) {
        Book_setPublisher(&book, buffer);
      }

      printf("Digite a data de lancamento do livro (dd/MM/YYYY) [vazio para "
             "nao alterar]: ");
      fgets(buffer, 255, stdin);
      buffer[strlen(buffer) - 1] = '\0';

      if (strlen(buffer) != 0) {
        Book_setReleaseDate(&book, buffer);
      }

      printf("Digite o preco do livro [-1 para nao alterar]: ");
      scanf("%lf", &doubleBuffer);
      getchar();
      if (doubleBuffer >= 0) {
        book.price = doubleBuffer;
      }

      BookManager_updateBook(&bm, &book);
    } else if (choice == 6) {
      long int idRemove;

      printf("Digite o ID a ser removido: ");
      scanf("%ld", &choice);
      getchar();

      BookManager_removeBook(&bm, idRemove);
    }
  }

  BookManager_destroy(&bm);

  return 0;
}