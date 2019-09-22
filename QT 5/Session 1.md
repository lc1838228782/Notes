## 信号槽

在 Qt 5 中，`QObject::connect()`有五个重载：

```c++
QMetaObject::Connection connect(const QObject *, const char *,
                                const QObject *, const char *,
                                Qt::ConnectionType);

QMetaObject::Connection connect(const QObject *, const QMetaMethod &,
                                const QObject *, const QMetaMethod &,
                                Qt::ConnectionType);

QMetaObject::Connection connect(const QObject *, const char *,
                                const char *,
                                Qt::ConnectionType) const;

QMetaObject::Connection connect(const QObject *, PointerToMemberFunction,
                                const QObject *, PointerToMemberFunction,
                                Qt::ConnectionType)

QMetaObject::Connection connect(const QObject *, PointerToMemberFunction,
                                Functor);
// 最后一个参数是Functor类型。这个类型可以接受 static 函数、全局函数以及 Lambda 表达式。
```



使用了`SIGNAL`和`SLOT`这两个宏，将两个函数名转换成了字符串。



- 发送者和接收者都需要是`QObject`的子类（当然，槽函数是全局函数、Lambda 表达式等无需接收者的时候除外）；
- 使用 signals 标记信号函数，信号是一个函数声明，返回 void，不需要实现函数代码；
- 槽函数是普通的成员函数，作为成员函数，会受到 public、private、protected 的影响；
- 使用 emit 在恰当的位置发送信号；
- 使用`QObject::connect()`函数连接信号和槽。

## 状态栏，菜单栏，工具栏

```c++
	openAction = new QAction(QIcon(":/images/doc-open"), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);

    statusBar() ;
```

下面的`menuBar()`、`toolBar()`和`statusBar()`三个是`QMainWindow`的函数，用于创建并返回菜单栏、工具栏和状态栏。

## [资源文件](https://www.devbean.net/2012/08/qt-study-road-2-resource-files/)

Qt 资源系统是一个跨平台的资源机制，用于将程序运行时所需要的资源以二进制的形式存储于可执行文件内部。如果你的程序需要加载特定的资源（图标、文本翻译等），那么，将其放置在资源文件中，就再也不需要担心这些文件的丢失。也就是说，如果你将资源以资源文件形式存储，它是会**编译到可执行文件内部**。

`:/images/document-open.png`：前缀/images，文件名document-open.png。

为了避免以后更改文件名称，多次修改的问题，可以起个别名。

国际化。

## [对象模型](https://www.devbean.net/2012/09/qt-study-road-2-objects-model/)

Qt 使用 moc，为标准 C++ 增加了一些特性：

- 信号槽机制，用于解决对象之间的通讯，这个我们已经了解过了，可以认为是 Qt 最明显的特性之一；
- 可查询，并且可设计的对象属性；
- 强大的事件机制以及事件过滤器；
- 基于上下文的字符串翻译机制（国际化），也就是 tr() 函数，我们简单地介绍过；
- 复杂的定时器实现，用于在事件驱动的 GUI 中嵌入能够精确控制的任务集成；
- 层次化的可查询的对象树，提供一种自然的方式管理对象关系。
- 智能指针（QPointer），在对象析构之后自动设为 0，防止野指针；
- 能够跨越库边界的动态转换机制。

通过继承`QObject`类，我们可以很方便地获得这些特性。当然，这些特性都是由 moc 帮助我们实现的。moc 其实实现的是一个叫做元对象系统（meta-object system）的机制。

`QObject`是以对象树的形式组织起来的。

`QWidget`是能够在屏幕上显示的一切组件的父类。`QWidget`继承自`QObject`，因此也继承了这种对象树关系。一个孩子自动地成为父组件的一个子组件。

我们可以使用`QObject::dumpObjectTree()`和`QObject::dumpObjectInfo()`这两个函数进行这方面的调试。

**注意！**

```c++
{
    QPushButton quit("Quit");
    QWidget window;

    quit.setParent(&window);
}
```

局部对象的析构顺序应该按照其创建顺序的相反过程。window析构，子对象quit析构。而后，quit再次析构。崩溃退出。

由此我们看到，Qt 的对象树机制虽然帮助我们在一定程度上解决了内存问题，但是也引入了一些值得注意的事情。这些细节在今后的开发过程中很可能时不时跳出来烦扰一下，所以，我们最好从开始就养成良好习惯，在 Qt 中，尽量在构造的时候就指定 parent 对象，并且**大胆在堆上创建**。