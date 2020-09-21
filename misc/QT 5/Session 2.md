## 布局管理器

- void valueChanged(int)
- void valueChanged(const QString &)

可以通过创建函数指针的方式显式地指定某一个函数`void (QSpinBox:: *spinBoxSignal)(int) = &QSpinBox::valueChanged;`

另外可以使用 Qt 4 的`SIGNAL`和`SLOT`宏，因为这两个宏已经指定了参数信息，所以不存在这个问题。

`QObject::connect(spinBox, SIGNAL(valueChanged(int)), slider,  SLOT(setValue(int)));`



Qt 提供了几种布局管理器供我们选择：

- `QHBoxLayout`：按照水平方向从左到右布局；
- `QVBoxLayout`：按照竖直方向从上到下布局；
- `QGridLayout`：在一个网格中进行布局，类似于 HTML 的 table；
- `QFormLayout`：按照表格布局，每一行前面是一段文本，文本后面跟随一个组件（通常是输入框），类似 HTML 的 form；
- `QStackedLayout`：层叠的布局，允许我们将几个组件按照 Z 轴方向堆叠，可以形成向导那种一页一页的效果。

## [对话框简介](https://www.devbean.net/2012/09/qt-study-road-2-dialogs-intro/)

`QDialog`（及其子类，以及所有`Qt::Dialog`类型的类）的对于其 parent 指针都有额外的解释：如果 parent 为 NULL，则该对话框会作为一个顶层窗口，否则则作为其父组件的子对话框（此时，其默认出现的位置是 parent 的中心）。顶层窗口与非顶层窗口的区别在于，顶层窗口在任务栏会有自己的位置，而非顶层窗口则会共享其父组件的位置。

```c++
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Main Window"));

    openAction = new QAction(QIcon(":/images/doc-open"), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
}

MainWindow::~MainWindow()
{
}

void MainWindow::open()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
}
```

对话框分为**模态对话框**和**非模态对话框**。所谓模态对话框，就是会阻塞同一应用程序中其它窗口的输入。

Qt 有两种级别的模态对话框：应用程序级别的模态和窗口级别的模态，默认是应用程序级别的模态。应用程序级别的模态是指，当该种模态的对话框出现时，用户必须首先对对话框进行交互，直到关闭对话框，然后才能访问程序中其他的窗口。窗口级别的模态是指，该模态仅仅阻塞与对话框关联的窗口，但是依然允许用户与程序中其它窗口交互。窗口级别的模态尤其适用于多窗口模式。

Qt 使用`QDialog::exec()`实现应用程序级别的模态对话框，使用`QDialog::open()`实现窗口级别的模态对话框，使用`QDialog::show()`实现非模态对话框。

```c++
void MainWindow::open()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.show();
}
```

对话框一闪而过，这是因为，`show()`函数不会阻塞当前线程，对话框会显示出来，然后函数立即返回，代码继续执行。



在堆上分配空间可以解决。不过，这样做有一个问题：如果我们的对话框不是在一个界面类中出现呢？由于`QWidget`的 parent 必须是`QWidget`指针，那就限制了我们不能将一个普通的 C++ 类指针传给 Qt 对话框。另外，如果对内存占用有严格限制的话，当我们将主窗口作为 parent 时，**主窗口不关闭，对话框就不会被销毁**，所以会一直占用内存。在这种情景下，我们可以设置 dialog 的`WindowAttribute`：

```c++
void MainWindow::open()
{
    QDialog *dialog = new QDialog;
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Hello, dialog!"));
    dialog->show();
}
```

`setAttribute()`函数设置对话框关闭时，自动销毁对话框。另外，`QObject`还有一个`deleteLater()`函数，该函数会在当前事件循环结束时销毁该对话框（具体到这里，需要使用`exec()`开始一个新的事件循环）。

*如果是 show() 函数的话，只能使用 dialog->setAttribute(Qt::WA_DeleteOnClose)；如果要使用 dialog->deleteLater()，则必须使用 exec() 函数。这是因为 deleteLater() 会在事件循环结束时执行，show() 不开始新的事件循环，所以会一闪而过。*

## 对话框数据传递

```c++
void MainWindow::open()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
    qDebug() << dialog.result();
}
```

`exec()`开启事件循环阻塞在那里，返回之后可以获得result。

注意：如果我们设置 dialog 的属性为`WA_DeleteOnClose`，那么当对话框关闭时，对象被销毁，我们就不能使用这种办法获取数据了。

实际上，`QDialog::exec()`是有返回值的，其返回值是`QDialog::Accepted`或者`QDialog::Rejected`。一般我们会使用类似下面的代码：

```c++
QDialog dialog(this);
if (dialog.exec() == QDialog::Accepted) {
    // do something
} else {
    // do something else
}
```

来判断对话框的返回值，也就是用户是点击了“确定”还是“取消”。

由于非模态对话框在关闭时可以调用`QDialog::accept()`或者`QDialog::reject()`或者更通用的`QDialog::done()`函数，所以我们可以在这里发出信号。另外，如果找不到合适的信号发出点，我们可以重写`QDialog::closeEvent()`函数，在这里发出信号。在需要接收数据的窗口（这里是主窗口）连接到这个信号即可。类似的代码片段如下所示：

```c++
//!!! Qt 5
// in dialog:
void UserAgeDialog::accept()
{
    emit userAgeChanged(newAge); // newAge is an int
    QDialog::accept();
}

// in main window:
void MainWindow::showUserAgeDialog()
{
    UserAgeDialog *dialog = new UserAgeDialog(this);
    connect(dialog, &UserAgeDialog::userAgeChanged, this, &MainWindow::setUserAge);
    dialog->show();
}

// ...

void MainWindow::setUserAge(int age)
{
    userAge = age;
}
```

## [标准对话框 QMessageBox](https://www.devbean.net/2012/09/qt-study-road-2-standard-dialogs-qmessagebox/)

Qt 的内置对话框大致分为以下几类：

- `QColorDialog`：选择颜色；
- `QFileDialog`：选择文件或者目录；
- `QFontDialog`：选择字体；
- `QInputDialog`：允许用户输入一个值，并将其值返回；
- `QMessageBox`：模态对话框，用于显示信息、询问问题等；
- `QPageSetupDialog`：为打印机提供纸张相关的选项；
- `QPrintDialog`：打印机配置；
- `QPrintPreviewDialog`：打印预览；
- `QProgressDialog`：显示操作过程。

`QMessageBox`用于显示消息提示。我们一般会使用其提供的几个 static 函数：

- `void about(QWidget * parent, const QString & title, const QString & text)`：显示关于对话框。这是一个最简单的对话框，其标题是 title，内容是 text，父窗口是 parent。对话框只有一个 OK 按钮。
- `void aboutQt(QWidget * parent, const QString & title = QString())`：显示关于 Qt 对话框。该对话框用于显示有关 Qt 的信息。
- `StandardButton critical(QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton)`：显示严重错误对话框。这个对话框将显示一个红色的错误符号。我们可以通过 buttons 参数指明其显示的按钮。默认情况下只有一个 Ok 按钮，我们可以使用`StandardButtons`类型指定多种按钮。
- `StandardButton information(QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton)`：`QMessageBox::information()`函数与`QMessageBox::critical()`类似，不同之处在于这个对话框提供一个普通信息图标。
- `StandardButton question(QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = StandardButtons( Yes | No ), StandardButton defaultButton = NoButton)`：`QMessageBox::question()`函数与`QMessageBox::critical()`类似，不同之处在于这个对话框提供一个问号图标，并且其显示的按钮是“是”和“否”两个。
- `StandardButton warning(QWidget * parent, const QString & title, const QString & text, StandardButtons buttons = Ok, StandardButton defaultButton = NoButton)`：`QMessageBox::warning()`函数与`QMessageBox::critical()`类似，不同之处在于这个对话框提供一个黄色叹号图标。

```c++
if (QMessageBox::Yes == QMessageBox::question(this,
                                              tr("Question"),
                                              tr("Are you OK?"),
                                              QMessageBox::Yes | QMessageBox::No,
                                              QMessageBox::Yes)) {
    QMessageBox::information(this, tr("Hmmm..."), tr("I'm glad to hear that!"));
} else {
    QMessageBox::information(this, tr("Hmmm..."), tr("I'm sorry!"));
}
```

`QMessageBox`类的 static 函数优点是方便使用，缺点也很明显：非常不灵活。我们只能使用简单的几种形式。为了能够定制`QMessageBox`细节，我们必须使用`QMessageBox`的属性设置 API。如果我们希望制作一个询问是否保存的对话框，我们可以使用如下的代码：

```c++
QMessageBox msgBox;
msgBox.setText(tr("The document has been modified."));
msgBox.setInformativeText(tr("Do you want to save your changes?"));
msgBox.setDetailedText(tr("Differences here..."));
msgBox.setStandardButtons(QMessageBox::Save
                          | QMessageBox::Discard
                          | QMessageBox::Cancel);
msgBox.setDefaultButton(QMessageBox::Save);
int ret = msgBox.exec();
switch (ret) {
case QMessageBox::Save:
    qDebug() << "Save document!";
    break;
case QMessageBox::Discard:
    qDebug() << "Discard changes!";
    break;
case QMessageBox::Cancel:
    qDebug() << "Close document!";
    break;
}
```

对于一个普通的`QDialog`而言，Qt 使用的是`QDialogButtonBox`这个类来实现不同平台的对话框按钮顺序的显示的。

## [深入Qt5信号槽新语法](https://www.devbean.net/2012/09/qt-study-road-2-deep-qt5-signals-slots-syntax/)

信号重载

```c++
QObject::connect(&newspaper,
                 (void (Newspaper:: *)(const QString &, const QDate &))&Newspaper::newPaper,
                 &reader,
                 &Reader::receiveNewspaper);
```

```c++
QObject::connect(&newspaper,
                 static_cast<void (Newspaper:: *)(const QString &, const QDate &)>(&Newspaper::newPaper),
                 &reader,
                 &Reader::receiveNewspaper);
```

对比上面两种写法。第一个使用的是 C 风格的强制类型转换。此时，如果你改变了信号的类型，那么你就会有一个潜在的运行时错误。例如，如果我们把`(const QString &, const QDate &)`两个参数修改成`(const QDate &, const QString &)`，C 风格的强制类型转换就会失败，并且这个错误只能在运行时发现。而**第二种则是 C++ 推荐的风格，当参数类型改变时，编译器会检测到这个错误。**

### 带有默认参数的槽函数

Qt 允许信号和槽的参数数目不一致：槽函数的参数数目可以比信号的参数少。然而，有一种情况，槽函数的参数可以比信号的多，那就是槽函数的参数带有默认值。

虽然`Reader::receiveNewspaper()`的参数数目比`Newspaper::newPaper()`多，但是由于`Reader::receiveNewspaper()`后面一个参数带有默认值，所以该参数不是必须提供的。但是，如果你按照前面的写法，比如如下的代码：

```c++
QObject::connect(&newspaper,
                 static_cast<void (Newspaper:: *)(const QString &)>(&Newspaper::newPaper),
                 &reader,
                 static_cast<void (Reader:: *)(const QString &, const QDate & =QDate::currentDate())>(&Reader::receiveNewspaper));
```

你会得到一个断言错误：`The slot requires more arguments than the signal provides.`

我们不能在函数指针中使用函数参数的默认值。这是 C++ 语言的限制：**参数默认值只能使用在直接地函数调用中。当使用函数指针取其地址的时候，默认参数是不可见的！**

当然，此时你可以选择 Qt 4 的连接语法。如果你还是想使用 Qt 5 的新语法，目前的办法只有一个：Lambda 表达式。不要担心你的编译器不支持 Lambda 表达式，因为在你使用 Qt 5 的时候，能够支持 Qt 5 的编译器都是支持 Lambda 表达式的。于是，我们的代码就变成了：

```c++
QObject::connect(&newspaper,
                 static_cast<void (Newspaper:: *)(const QString &)>(&Newspaper::newPaper),
                 [=](const QString &name) { /* Your code here. */ });
```

## [文件对话框](https://www.devbean.net/2012/09/qt-study-road-2-file-dialog/)

```c++
void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),
                                 tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        textEdit->setText(in.readAll());
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}

void MainWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                                       tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}
```

```c++
QString getOpenFileName(QWidget * parent = 0,
                        const QString & caption = QString(),
                        const QString & dir = QString(),
                        const QString & filter = QString(),
                        QString * selectedFilter = 0,
                        Options options = 0)
```

- parent：父窗口。我们前面介绍过，Qt 的标准对话框提供静态函数，用于返回一个模态对话框（在一定程度上这就是外观模式的一种体现）；
- caption：对话框标题；
- dir：对话框打开时的默认目录，“.” 代表程序运行目录，“/” 代表当前盘符的根目录（特指 Windows 平台；Linux 平台当然就是根目录），这个参数也可以是平台相关的，比如“C:\\”等；
- filter：过滤器。我们使用文件对话框可以浏览很多类型的文件，但是，很多时候我们仅希望打开特定类型的文件。比如，文本编辑器希望打开文本文件，图片浏览器希望打开图片文件。过滤器就是用于过滤特定的后缀名。如果我们使用“Image Files(*.jpg *.png)”，则只能显示后缀名是 jpg 或者 png 的文件。如果需要多个过滤器，使用“;;”分割，比如“JPEG Files(*.jpg);;PNG Files(*.png)”；
- selectedFilter：默认选择的过滤器；
- options：对话框的一些参数设定，比如只显示文件夹等等，它的取值是`enum QFileDialog::Option`，每个选项可以使用 | 运算组合起来。

## [事件](https://www.devbean.net/2012/09/qt-study-road-2-events/)

在所有组件的父类`QWidget`中，定义了很多事件处理的回调函数，如`keyPressEvent()`、`keyReleaseEvent()`、`mouseDoubleClickEvent()`、`mouseMoveEvent()`、`mousePressEvent()`、`mouseReleaseEvent()`等。这些函数都是 protected virtual 的，也就是说，我们可以在子类中重新实现这些函数。下面来看一个例子：

```c++
class EventLabel : public QLabel
{
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

void EventLabel::mouseMoveEvent(QMouseEvent *event)
{
    this->setText(QString("<center><h1>Move: (%1, %2)</h1></center>")
                  .arg(QString::number(event->x()), QString::number(event->y())));
}

void EventLabel::mousePressEvent(QMouseEvent *event)
{
    this->setText(QString("<center><h1>Press: (%1, %2)</h1></center>")
                  .arg(QString::number(event->x()), QString::number(event->y())));
}

void EventLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QString msg;
    msg.sprintf("<center><h1>Release: (%d, %d)</h1></center>",
                event->x(), event->y());
    this->setText(msg);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EventLabel *label = new EventLabel;
    label->setWindowTitle("MouseEvent Demo");
    label->resize(300, 200);
    label->show();

    return a.exec();
}
```

`QString`的`arg()`函数可以自动替换掉`QString`中出现的占位符。其占位符以 % 开始，后面是占位符的位置，例如 %1，%2 这种。`QString("[%1, %2]").arg(x, y);`

为什么要点击鼠标之后才能在`mouseMoveEvent()`函数中显示鼠标坐标值？这是因为`QWidget`中有一个`mouseTracking`属性，该属性用于设置是否追踪鼠标。只有鼠标被追踪时，`mouseMoveEvent()`才会发出。如果`mouseTracking`是 false（默认即是），组件在至少一次鼠标点击之后，才能够被追踪，也就是能够发出`mouseMoveEvent()`事件。如果`mouseTracking`为 true，则`mouseMoveEvent()`直接可以被发出。知道了这一点，我们就可以在`main()`函数中直接设置下：

```c++
EventLabel *label = new EventLabel;
label->setWindowTitle("MouseEvent Demo");
label->resize(300, 200);
label->setMouseTracking(true);
label->show();
```

## [事件的接受与忽略](https://www.devbean.net/2012/09/qt-study-road-2-events-accept-reject/)

mousePressEvent()覆盖掉了clicked()事件。暗示我们一个非常重要的细节：**当重写事件回调函数时，时刻注意是否需要通过调用父类的同名函数来确保原有实现仍能进行！**

Qt 的事件对象有两个函数：`accept()`和`ignore()`。正如它们的名字一样，前者用来告诉 Qt，这个类的事件处理函数想要处理这个事件；后者则告诉 Qt，这个类的事件处理函数不想要处理这个事件。在事件处理函数中，可以使用`isAccepted()`来查询这个事件是不是已经被接收了。具体来说：如果一个事件处理函数调用了一个事件对象的`accept()`函数，这个事件就不会被继续传播给其**父组件**；如果它调用了事件的`ignore()`函数，Qt 会从其父组件中寻找另外的接受者。

事实上，我们很少会使用`accept()`和`ignore()`函数，而是像上面的示例一样，如果希望忽略事件（所谓忽略，是指自己不想要这个事件），只要调用父类的响应函数即可。因为我们无法确认父类中的这个处理函数有没有额外的操作。

**事件的传播是在组件层次上面的，而不是依靠类继承机制。**

在一个特殊的情形下，我们必须使用`accept()`和`ignore()`函数，那就是窗口关闭的事件。对于窗口关闭`QCloseEvent`事件，调用`accept()`意味着 Qt 会停止事件的传播，窗口关闭；调用`ignore()`则意味着事件继续传播，即阻止窗口关闭。

```c++
//!!! Qt5
...
textEdit = new QTextEdit(this);
setCentralWidget(textEdit);
connect(textEdit, &QTextEdit::textChanged, [=]() {
    this->setWindowModified(true);
});

setWindowTitle("TextPad [*]");
...

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isWindowModified()) {
        bool exit = QMessageBox::question(this,
                                      tr("Quit"),
                                      tr("Are you sure to quit this application?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No) == QMessageBox::Yes;
        if (exit) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}
```

`setWindowTitle()`函数可以使用 [] 这种语法来表明，在窗口内容发生改变时（通过setWindowModified(true)函数通知），Qt 会自动在标题上面的 [] 位置替换成 * 号。我们使用 Lambda 表达式连接`QTextEdit::textChanged()`信号，将`windowModified`设置为 true。然后我们需要重写`closeEvent()`函数。在这个函数中，我们首先判断是不是有过修改，如果有，则弹出询问框，问一下是否要退出。如果用户点击了“Yes”，则接受关闭事件，这个事件所在的操作就是关闭窗口。因此，一旦接受事件，窗口就会被关闭；否则窗口继续保留。当然，如果窗口内容没有被修改，则直接接受事件，关闭窗口。

## [event()](https://www.devbean.net/2012/10/qt-study-road-2-event-func/)

`event()`函数主要用于事件的分发。所以，如果你希望在事件分发之前做一些操作，就可以重写这个`event()`函数了。

```c++
bool CustomWidget::event(QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if (keyEvent->key() == Qt::Key_Tab) {
            qDebug() << "You press tab.";
            return true;
        }
    }
    return QWidget::event(e);
}
```

注意，在`event()`函数中，调用事件对象的`accept()`和`ignore()`函数是没有作用的，不会影响到事件的传播。

如果传入的事件已被识别并且处理，则需要返回 true，否则返回 false。如果返回值是 true，那么 Qt 会认为这个事件已经处理完毕，不会再将这个事件发送给其它对象，而是会继续处理事件队列中的下一事件。

