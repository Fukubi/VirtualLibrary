#include "main_window.h"
#include "book.h"
#include "book_manager.h"

enum {
  ID_COLUMN,
  NAME_COLUMN,
  AUTHOR_COLUMN,
  PUBLISHER_COLUMN,
  RELEASE_DATE_COLUMN,
  PRICE_COLUMN,
  N_COLUMNS
};

typedef struct NewBookWindow {
  GtkWidget *window;

  GtkWidget *gridContainer;

  GtkWidget *labelName;
  GtkWidget *entryName;

  GtkWidget *labelAuthor;
  GtkWidget *entryAuthor;

  GtkWidget *labelPublisher;
  GtkWidget *entryPublisher;

  GtkWidget *labelReleaseDate;
  GtkWidget *entryReleaseDate;

  GtkWidget *labelPrice;
  GtkWidget *entryPrice;

  GtkWidget *saveButton;
} NewBookWindow;

typedef struct EditBookWindow {
  GtkWidget *window;

  GtkWidget *gridContainer;

  GtkWidget *labelName;
  GtkWidget *entryName;

  GtkWidget *labelAuthor;
  GtkWidget *entryAuthor;

  GtkWidget *labelPublisher;
  GtkWidget *entryPublisher;

  GtkWidget *labelReleaseDate;
  GtkWidget *entryReleaseDate;

  GtkWidget *labelPrice;
  GtkWidget *entryPrice;

  GtkWidget *saveButton;

  long int selectedId;
} EditBookWindow;

static GtkListStore *listBooksStore;
static GtkTreeIter iter;
static Book selectedBook;

static void onSaveBookClick(GtkWidget *widget, gpointer data) {
  Book book;
  BookManager bookManager;
  ListBookElement *books;
  NewBookWindow *newBookWindow = (NewBookWindow *)data;

  BookManager_initialize(&bookManager);
  ListBookElement_initialize(&books);

  Book_initialize(&book);
  Book_setName(&book, gtk_entry_get_text(GTK_ENTRY(newBookWindow->entryName)));
  Book_setAuthor(&book,
                 gtk_entry_get_text(GTK_ENTRY(newBookWindow->entryAuthor)));
  Book_setPublisher(
      &book, gtk_entry_get_text(GTK_ENTRY(newBookWindow->entryPublisher)));
  Book_setReleaseDate(
      &book, gtk_entry_get_text(GTK_ENTRY(newBookWindow->entryReleaseDate)));
  book.price = atof(gtk_entry_get_text(GTK_ENTRY(newBookWindow->entryPrice)));

  BookManager_addBook(&bookManager, &book);

  books = BookManager_listBooks(&bookManager);

  gtk_list_store_clear(listBooksStore);
  for (ListBookElement *el = books; el != NULL; el = el->next) {
    gtk_list_store_append(listBooksStore, &iter);

    gtk_list_store_set(listBooksStore, &iter, ID_COLUMN, el->book->id,
                       NAME_COLUMN, el->book->name, AUTHOR_COLUMN,
                       el->book->author, PUBLISHER_COLUMN, el->book->publisher,
                       RELEASE_DATE_COLUMN, el->book->releaseDate, PRICE_COLUMN,
                       el->book->price, -1);
  }

  ListBookElement_destroy(&books);
  BookManager_destroy(&bookManager);

  gtk_window_close(GTK_WINDOW(newBookWindow->window));

  free(newBookWindow);
}

static void onNewBookClick(GtkWidget *widget, gpointer data) {
  NewBookWindow *newBookWindow = (NewBookWindow *)malloc(sizeof(NewBookWindow));

  newBookWindow->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(newBookWindow->window), "New Book");
  gtk_window_set_resizable(GTK_WINDOW(newBookWindow->window), FALSE);

  newBookWindow->gridContainer = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(newBookWindow->gridContainer), 7);
  gtk_grid_set_column_spacing(GTK_GRID(newBookWindow->gridContainer), 7);
  gtk_widget_set_margin_start(newBookWindow->gridContainer, 5);
  gtk_widget_set_margin_end(newBookWindow->gridContainer, 5);
  gtk_widget_set_margin_top(newBookWindow->gridContainer, 5);
  gtk_widget_set_margin_bottom(newBookWindow->gridContainer, 5);
  gtk_container_add(GTK_CONTAINER(newBookWindow->window),
                    newBookWindow->gridContainer);

  newBookWindow->labelName = gtk_label_new("Name: ");
  newBookWindow->entryName = gtk_entry_new();

  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->labelName, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->entryName, 1, 0, 1, 1);

  newBookWindow->labelAuthor = gtk_label_new("Author: ");
  newBookWindow->entryAuthor = gtk_entry_new();

  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->labelAuthor, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->entryAuthor, 1, 1, 1, 1);

  newBookWindow->labelPublisher = gtk_label_new("Publisher: ");
  newBookWindow->entryPublisher = gtk_entry_new();

  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->labelPublisher, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->entryPublisher, 1, 2, 1, 1);

  newBookWindow->labelReleaseDate = gtk_label_new("Release Date: ");
  newBookWindow->entryReleaseDate = gtk_entry_new();

  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->labelReleaseDate, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->entryReleaseDate, 1, 3, 1, 1);

  newBookWindow->labelPrice = gtk_label_new("Price: ");
  newBookWindow->entryPrice = gtk_entry_new();

  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->labelPrice, 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->entryPrice, 1, 4, 1, 1);

  newBookWindow->saveButton = gtk_button_new_with_label("New Book");
  g_signal_connect(newBookWindow->saveButton, "clicked",
                   G_CALLBACK(onSaveBookClick), newBookWindow);

  gtk_grid_attach(GTK_GRID(newBookWindow->gridContainer),
                  newBookWindow->saveButton, 0, 5, 2, 2);

  gtk_widget_show_all(newBookWindow->window);
}

static void onSaveChangesClick(GtkWidget *widget, gpointer data) {
  Book book;
  BookManager bookManager;
  ListBookElement *books;
  EditBookWindow *editBookWindow = (EditBookWindow *)data;

  BookManager_initialize(&bookManager);
  ListBookElement_initialize(&books);

  Book_initialize(&book);
  book.id = editBookWindow->selectedId;
  Book_setName(&book, gtk_entry_get_text(GTK_ENTRY(editBookWindow->entryName)));
  Book_setAuthor(&book,
                 gtk_entry_get_text(GTK_ENTRY(editBookWindow->entryAuthor)));
  Book_setPublisher(
      &book, gtk_entry_get_text(GTK_ENTRY(editBookWindow->entryPublisher)));
  Book_setReleaseDate(
      &book, gtk_entry_get_text(GTK_ENTRY(editBookWindow->entryReleaseDate)));
  book.price = atof(gtk_entry_get_text(GTK_ENTRY(editBookWindow->entryPrice)));

  BookManager_updateBook(&bookManager, &book);

  books = BookManager_listBooks(&bookManager);

  gtk_list_store_clear(listBooksStore);
  for (ListBookElement *el = books; el != NULL; el = el->next) {
    gtk_list_store_append(listBooksStore, &iter);

    gtk_list_store_set(listBooksStore, &iter, ID_COLUMN, el->book->id,
                       NAME_COLUMN, el->book->name, AUTHOR_COLUMN,
                       el->book->author, PUBLISHER_COLUMN, el->book->publisher,
                       RELEASE_DATE_COLUMN, el->book->releaseDate, PRICE_COLUMN,
                       el->book->price, -1);
  }

  ListBookElement_destroy(&books);
  BookManager_destroy(&bookManager);

  gtk_window_close(GTK_WINDOW(editBookWindow->window));

  free(editBookWindow);
}

static void onEditBookClick(GtkWidget *widget, gpointer data) {
  EditBookWindow *editBookWindow =
      (EditBookWindow *)malloc(sizeof(EditBookWindow));

  editBookWindow->selectedId = selectedBook.id;

  editBookWindow->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(editBookWindow->window), "New Book");
  gtk_window_set_resizable(GTK_WINDOW(editBookWindow->window), FALSE);

  editBookWindow->gridContainer = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(editBookWindow->gridContainer), 7);
  gtk_grid_set_column_spacing(GTK_GRID(editBookWindow->gridContainer), 7);
  gtk_widget_set_margin_start(editBookWindow->gridContainer, 5);
  gtk_widget_set_margin_end(editBookWindow->gridContainer, 5);
  gtk_widget_set_margin_top(editBookWindow->gridContainer, 5);
  gtk_widget_set_margin_bottom(editBookWindow->gridContainer, 5);
  gtk_container_add(GTK_CONTAINER(editBookWindow->window),
                    editBookWindow->gridContainer);

  editBookWindow->labelName = gtk_label_new("Name: ");
  editBookWindow->entryName = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(editBookWindow->entryName), selectedBook.name);

  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->labelName, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->entryName, 1, 0, 1, 1);

  editBookWindow->labelAuthor = gtk_label_new("Author: ");
  editBookWindow->entryAuthor = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(editBookWindow->entryAuthor),
                     selectedBook.author);

  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->labelAuthor, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->entryAuthor, 1, 1, 1, 1);

  editBookWindow->labelPublisher = gtk_label_new("Publisher: ");
  editBookWindow->entryPublisher = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(editBookWindow->entryPublisher),
                     selectedBook.publisher);

  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->labelPublisher, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->entryPublisher, 1, 2, 1, 1);

  editBookWindow->labelReleaseDate = gtk_label_new("Release Date: ");
  editBookWindow->entryReleaseDate = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(editBookWindow->entryReleaseDate),
                     selectedBook.releaseDate);

  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->labelReleaseDate, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->entryReleaseDate, 1, 3, 1, 1);

  editBookWindow->labelPrice = gtk_label_new("Price: ");
  editBookWindow->entryPrice = gtk_entry_new();
  char priceStr[40];
  sprintf(priceStr, "%f", selectedBook.price);
  gtk_entry_set_text(GTK_ENTRY(editBookWindow->entryPrice), priceStr);

  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->labelPrice, 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->entryPrice, 1, 4, 1, 1);

  editBookWindow->saveButton = gtk_button_new_with_label("Save Changes");
  g_signal_connect(editBookWindow->saveButton, "clicked",
                   G_CALLBACK(onSaveChangesClick), editBookWindow);

  gtk_grid_attach(GTK_GRID(editBookWindow->gridContainer),
                  editBookWindow->saveButton, 0, 5, 2, 2);

  gtk_widget_show_all(editBookWindow->window);
}

static void onTreeSelectionChange(GtkTreeSelection *selection, gpointer data) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gint32 id;
  gchar *name;
  gchar *author;
  gchar *publisher;
  gchar *releaseDate;
  gfloat price;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, ID_COLUMN, &id, NAME_COLUMN, &name,
                       AUTHOR_COLUMN, &author, PUBLISHER_COLUMN, &publisher,
                       RELEASE_DATE_COLUMN, &releaseDate, PRICE_COLUMN, &price,
                       -1);

    selectedBook.id = id;
    Book_setName(&selectedBook, name);
    Book_setAuthor(&selectedBook, author);
    Book_setPublisher(&selectedBook, publisher);
    Book_setReleaseDate(&selectedBook, releaseDate);
    selectedBook.price = price;

    g_free(name);
    g_free(author);
    g_free(publisher);
    g_free(releaseDate);
  }
}

static void onDeleteButtonClick(GtkWidget *widget, gpointer data) {
  BookManager bookManager;
  ListBookElement *books;

  if (strlen(selectedBook.name) > 0) {
    BookManager_initialize(&bookManager);
    BookManager_removeBook(&bookManager, selectedBook.id);

    ListBookElement_initialize(&books);

    books = BookManager_listBooks(&bookManager);

    gtk_list_store_clear(listBooksStore);
    for (ListBookElement *el = books; el != NULL; el = el->next) {
      gtk_list_store_append(listBooksStore, &iter);

      gtk_list_store_set(
          listBooksStore, &iter, ID_COLUMN, el->book->id, NAME_COLUMN,
          el->book->name, AUTHOR_COLUMN, el->book->author, PUBLISHER_COLUMN,
          el->book->publisher, RELEASE_DATE_COLUMN, el->book->releaseDate,
          PRICE_COLUMN, el->book->price, -1);
    }

    ListBookElement_destroy(&books);
    BookManager_destroy(&bookManager);
  }
}

static void onEntryAuthorChanged(GtkWidget *widget, gpointer data) {
  BookManager bookManager;
  ListBookElement *books;
  GtkWidget *entry = (GtkWidget *)data;
  const gchar *author = gtk_entry_get_text(GTK_ENTRY(entry));

  BookManager_initialize(&bookManager);
  ListBookElement_initialize(&books);

  if (strlen(author) > 0) {
    books = BookManager_findByAuthor(&bookManager, author);
  } else {
    books = BookManager_listBooks(&bookManager);
  }

  gtk_list_store_clear(listBooksStore);
  for (ListBookElement *el = books; el != NULL; el = el->next) {
    gtk_list_store_append(listBooksStore, &iter);

    gtk_list_store_set(listBooksStore, &iter, ID_COLUMN, el->book->id,
                       NAME_COLUMN, el->book->name, AUTHOR_COLUMN,
                       el->book->author, PUBLISHER_COLUMN, el->book->publisher,
                       RELEASE_DATE_COLUMN, el->book->releaseDate, PRICE_COLUMN,
                       el->book->price, -1);
  }
  ListBookElement_destroy(&books);
  BookManager_destroy(&bookManager);
}

static void onEntryNameChanged(GtkWidget *widget, gpointer data) {
  BookManager bookManager;
  ListBookElement *books;
  GtkWidget *entry = (GtkWidget *)data;
  const gchar *name = gtk_entry_get_text(GTK_ENTRY(entry));

  BookManager_initialize(&bookManager);
  ListBookElement_initialize(&books);

  if (strlen(name) > 0) {
    books = BookManager_findByName(&bookManager, name);
  } else {
    books = BookManager_listBooks(&bookManager);
  }

  gtk_list_store_clear(listBooksStore);
  for (ListBookElement *el = books; el != NULL; el = el->next) {
    gtk_list_store_append(listBooksStore, &iter);

    gtk_list_store_set(listBooksStore, &iter, ID_COLUMN, el->book->id,
                       NAME_COLUMN, el->book->name, AUTHOR_COLUMN,
                       el->book->author, PUBLISHER_COLUMN, el->book->publisher,
                       RELEASE_DATE_COLUMN, el->book->releaseDate, PRICE_COLUMN,
                       el->book->price, -1);
  }
  ListBookElement_destroy(&books);
  BookManager_destroy(&bookManager);
}

void activateMainWindow(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;

  GtkWidget *gridContainer;

  GtkWidget *hBoxButton;
  GtkWidget *vBoxContainer;

  GtkWidget *buttonNew;
  GtkWidget *buttonEdit;
  GtkWidget *buttonDelete;

  GtkWidget *labelFilter;
  GtkWidget *entryAuthor;
  GtkWidget *entryName;

  GtkWidget *scrolledWindow;
  GtkWidget *listBook;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  Book_initialize(&selectedBook);

  /* Setup the selection handler */
  GtkTreeSelection *selection;

  BookManager bookManager;
  ListBookElement *books;

  /**
   * Creating Components
   */
  // Window
  window = gtk_application_window_new(app);

  // Main Grid container
  gridContainer = gtk_grid_new();

  // Initializing List
  listBooksStore =
      gtk_list_store_new(N_COLUMNS, G_TYPE_INT64, G_TYPE_STRING, G_TYPE_STRING,
                         G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);

  /**
   * Updating list with database values
   */
  BookManager_initialize(&bookManager);
  ListBookElement_initialize(&books);

  books = BookManager_listBooks(&bookManager);

  for (ListBookElement *el = books; el != NULL; el = el->next) {
    gtk_list_store_append(listBooksStore, &iter);

    gtk_list_store_set(listBooksStore, &iter, ID_COLUMN, el->book->id,
                       NAME_COLUMN, el->book->name, AUTHOR_COLUMN,
                       el->book->author, PUBLISHER_COLUMN, el->book->publisher,
                       RELEASE_DATE_COLUMN, el->book->releaseDate, PRICE_COLUMN,
                       el->book->price, -1);
  }
  ListBookElement_destroy(&books);
  BookManager_destroy(&bookManager);

  listBook = gtk_tree_view_new_with_model(GTK_TREE_MODEL(listBooksStore));

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text",
                                                    ID_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listBook), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text",
                                                    NAME_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listBook), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Author", renderer, "text",
                                                    AUTHOR_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listBook), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
      "Publisher", renderer, "text", PUBLISHER_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listBook), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
      "Release Date", renderer, "text", RELEASE_DATE_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listBook), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Price", renderer, "text",
                                                    PRICE_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listBook), column);

  // Setup list selection
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listBook));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(selection), "changed",
                   G_CALLBACK(onTreeSelectionChange), NULL);

  // Box to align correctly
  hBoxButton = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  vBoxContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Buttons in screen
  buttonNew = gtk_button_new_with_label("New Book");
  buttonEdit = gtk_button_new_with_label("Edit Book");
  buttonDelete = gtk_button_new_with_label("Delete Book");

  // Filter label
  labelFilter = gtk_label_new("Filters: ");

  // Entry filter
  entryName = gtk_entry_new();
  entryAuthor = gtk_entry_new();

  /**
   * Setting Values
   */

  // Window
  gtk_window_set_title(GTK_WINDOW(window), "Library Manager");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  // Grid Container
  gtk_grid_set_row_spacing(GTK_GRID(gridContainer), 10);
  gtk_grid_set_column_spacing(GTK_GRID(gridContainer), 10);

  // Scrolled Window Container
  scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolledWindow),
                                            800);
  gtk_scrolled_window_set_min_content_height(
      GTK_SCROLLED_WINDOW(scrolledWindow), 550);

  // Buttons
  g_signal_connect(buttonNew, "clicked", G_CALLBACK(onNewBookClick), NULL);
  g_signal_connect(buttonEdit, "clicked", G_CALLBACK(onEditBookClick), NULL);
  g_signal_connect(buttonDelete, "clicked", G_CALLBACK(onDeleteButtonClick),
                   listBook);

  // Entries
  gtk_entry_set_placeholder_text(GTK_ENTRY(entryAuthor), "Author Name");
  g_signal_connect(entryAuthor, "changed", G_CALLBACK(onEntryAuthorChanged), (gpointer)entryAuthor);
  gtk_entry_set_placeholder_text(GTK_ENTRY(entryName), "Book Name");
  g_signal_connect(entryName, "changed", G_CALLBACK(onEntryNameChanged), (gpointer)entryName);

  /**
   * Setting hierarchy
   */
  gtk_container_add(GTK_CONTAINER(window), gridContainer);

  gtk_container_add(GTK_CONTAINER(scrolledWindow), listBook);

  gtk_grid_attach(GTK_GRID(gridContainer), buttonNew, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(gridContainer), buttonEdit, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(gridContainer), buttonDelete, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(gridContainer), labelFilter, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridContainer), entryName, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridContainer), entryAuthor, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridContainer), scrolledWindow, 0, 2, 3, 3);

  gtk_widget_show_all(window);
}