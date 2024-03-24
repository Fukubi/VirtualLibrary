# Virtual Library

A Virtual Library created in C to manage your books! This project was created to be used in university to learn about Databases.

# Preview

![list preview](https://github.com/Fukubi/VirtualLibrary/blob/main/docs/PreviewGUIV1.1.png?raw=true)

# To-Do
- [x] Connect with Database
- [x] Execute default operations (Create, Read, Update, Delete)
- [x] Execute a search ByName and ById
- [x] Create a Initial GUI using terminal
- [x] Create the Class Diagram
- [x] Create a GUI outside of terminal
- [ ] Create a Report Interface

# Build Process

Install the required libraries for the project (SQLite and GTK3.0) using the available method on your OS, the create a build folder in the root directory of the project and go into it

```bash
mkdir build
cd build
```

Use cmake to generate build files and check if the dependencies are installed properly

```bash
cmake ..
```

Compile with your default compiler tool (i.e. make)

```bash
make .
```

The executable is generated at the src folder

# Libraries Used

- SQLite (https://www.sqlite.org/)
- Log.c (https://github.com/rxi/log.c)
- GTK3.0 (https://www.gtk.org/)
