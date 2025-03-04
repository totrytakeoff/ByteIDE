# ByteIDE

ByteIDE 是一个基于 Qt 开发的简易文本编辑器和集成开发环境 (IDE)。它使用了 QScintilla 库来实现编辑器功能，提供了基本的文本编辑功能和常用的快捷键以及查找替换功能,并且支持内嵌了C/Cpp编译器和Python解释器,支持一键运行代码。
![image](https://github.com/user-attachments/assets/1f802188-bfb0-4df5-b38f-eaa8255c7d7e)

![image](https://github.com/user-attachments/assets/e0cf7544-4010-45c0-9541-28f6de206134)


## 功能介绍

- **文件管理**：支持新建、打开、保存文件和文件夹。
- **代码编辑**：支持 C++ 和 Python 代码的语法高亮和自动补全。
- **终端集成**：内置终端，可以直接在 IDE 中执行命令。
- **代码运行**：支持运行 Python 和 C++ 代码。
- **查找替换**：提供查找和替换功能，支持正则表达式。
- **多标签页**：支持多文件同时打开和编辑。

## 实现方法

### 文件管理

通过 `QFileDialog` 实现文件和文件夹的选择和操作。使用 `QTreeView` 和 `QFileSystemModel` 实现文件资源管理器。

### 代码编辑

使用 QScintilla 库实现代码编辑器功能。通过 `QsciLexerCPP` 和 `QsciLexerPython` 实现 C++ 和 Python 代码的语法高亮和自动补全。

### 终端集成

使用 `QProcess` 实现终端功能，支持命令执行和输出显示。

### 代码运行

通过 `QProcess` 调用 Python 解释器和 C++ 编译器实现代码运行功能。支持运行结果在终端中显示。

### 查找替换

使用 `QScintilla` 的查找和替换功能，支持匹配大小写和完整单词的查找和替换。

### 多标签页

使用 `QTabWidget` 实现多文件同时打开和编辑功能。每个标签页对应一个 `EditArea` 对象。

## 项目结构

```
ByteIDE/
├── res/
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── editarea.cpp
│   ├── editarea.h
│   ├── terminal.cpp
│   ├── terminal.h
│   ├── coderunner.cpp
│   ├── coderunner.h
│   ├── newfile.cpp
│   ├── newfile.h
│   ├── resourcemanager.cpp
│   ├── resourcemanager.h
│   ├── searchwidget.cpp
│   ├── searchwidget.h
│   ├── codetabwidget.cpp
│   ├── codetabwidget.h
│   └── res.qrc
├── qscintilla/
│   ├── Qsci/
│   └── libqscintilla2_qt6d.a
├── CMakeLists.txt
└── README.md
```

## 编译和运行

### 环境要求

- Qt 6
- QScintilla
- CMake 3.16 或更高版本

### 编译步骤

1. 克隆项目到本地：
   ```sh
   git clone https://github.com/totrytakeoff/IDE-based-on-Qt.git
   cd IDE-based-on-Qt
   ```

2. 创建并进入构建目录：
   ```sh
   mkdir build
   cd build
   ```

3. 运行 CMake 配置项目：
   ```sh
   cmake ..
   ```

4. 编译项目：
   ```sh
   cmake --build .
   ```

5. 运行可执行文件：
   ```sh
   ./ByteIDE
   ```

## 贡献

欢迎提交 issue 和 pull request 来改进 ByteIDE。

## 许可证

本项目使用 MIT 许可证，详情请参阅 LICENSE 文件。
