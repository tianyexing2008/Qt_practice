#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"

#include <QLabel>
#include <QStandardItemModel>
#include <QDebug>
#include <QObject>
#include <QFileDialog>
#include <QTextCodec>
#include <QScrollBar>

#define FIX_COLUMN_COUNT 6 //固定6列
#define END_COLUMN_INDEX 5 //最后一列index

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createAction();
    initStatusBar();

    tableItemModel = new QStandardItemModel(0, FIX_COLUMN_COUNT, this);

    tableSelectionModel = new QItemSelectionModel(tableItemModel);
    connect(tableSelectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(on_currentItemChanged(const QModelIndex &, const QModelIndex &)));
    ui->tableView->setModel(tableItemModel);    //设置数据模型
    ui->tableView->setSelectionModel(tableSelectionModel); //设置选择模型
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems); //设置选择行为
    ui->tableView->horizontalHeader()->setFrameShape(QFrame::StyledPanel);
    ui->plainTextEdit->setReadOnly(true);
    //ui->tableView->setFixedWidth(630);
    //ui->plainTextEdit->setFixedWidth(470);
    //ui->tableView->verticalHeader()->setFrameShape(QFrame::Panel);
    //setFixedSize(1100, 500);
    setWindowTitle("QTableView与QStandardItemModel");

    helpDialog = new CHelpDialog(this);
    initTableViewHeader();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createAction()
{
    openAction = new QAction("打开", this);
    openAction->setIcon(QIcon(":/icon/open.gif"));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(onOpenFile()));

    saveAction = new QAction("保存", this);
    saveAction->setIcon(QIcon(":/icon/save.png"));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(onSaveFile()));

    addAction = new QAction("添加", this);
    addAction->setIcon(QIcon(":/icon/add.jpg"));
    connect(addAction, SIGNAL(triggered(bool)), this, SLOT(addRow()));

    delAction = new QAction("删除", this);
    delAction->setIcon(QIcon(":/icon/del.png"));
    connect(delAction, SIGNAL(triggered(bool)), this, SLOT(delRow()));

    addAction->setEnabled(false);
    delAction->setEnabled(false);

    alignLeftAction = new QAction("左对齐", this);
    alignLeftAction->setIcon(QIcon(":/icon/alignLeft.png"));
    connect(alignLeftAction, SIGNAL(triggered(bool)), this, SLOT(onAlignLeft()));

    alignRightAction = new QAction("右对齐", this);
    alignRightAction->setIcon(QIcon(":/icon/alignRight.png"));
    connect(alignRightAction, SIGNAL(triggered(bool)), this, SLOT(onAlignRight()));

    alignCentralAction = new QAction("居中", this);
    alignCentralAction->setIcon(QIcon(":/icon/alignCentral.png"));
    connect(alignCentralAction, SIGNAL(triggered(bool)), this, SLOT(onAlignCentral()));

    boldFontAction = new QAction("粗体", this);
    boldFontAction->setIcon(QIcon(":/icon/font_bold.png"));
    connect(boldFontAction, SIGNAL(triggered(bool)), this, SLOT(onBoldFont()));

    helpAction = new QAction("帮助", this);
    helpAction->setIcon(QIcon(":/icon/help.png"));
    connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(onHelp()));

    ui->mainToolBar->addAction(openAction);
    ui->mainToolBar->addAction(saveAction);
    ui->mainToolBar->addAction(addAction);
    ui->mainToolBar->addAction(delAction);
    ui->mainToolBar->addAction(alignLeftAction);
    ui->mainToolBar->addAction(alignRightAction);
    ui->mainToolBar->addAction(alignCentralAction);
    ui->mainToolBar->addAction(boldFontAction);
    ui->mainToolBar->addAction(helpAction);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void MainWindow::initStatusBar()
{
    labCurFile = new QLabel("当前文件：", this);
    labCurCellPos = new QLabel("当前单元格: ", this);
    labCurCellText = new QLabel("单元格内容：", this);

    ui->statusBar->addWidget(labCurFile);
    ui->statusBar->addWidget(labCurCellPos);
    ui->statusBar->addWidget(labCurCellText);
}

void MainWindow::initModelFromStringList(QStringList &list)
{
    addAction->setEnabled(true);
    delAction->setEnabled(true);

    int rowCnt = list.count();
    tableItemModel->setRowCount(rowCnt - 1);
    QString header = list.at(0); //第一行为标题
    QStringList headerList = header.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    tableItemModel->setHorizontalHeaderLabels(headerList); //设置表头文字
    QString lineText;
    QStringList tmpList;
    QStandardItem *item;
    QBrush brush(Qt::red);

    for(int row = 1; row < rowCnt; row++)
    {
        lineText = list.at(row);
        tmpList = lineText.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        for(int col = 0; col < FIX_COLUMN_COUNT; col++)
        {
            item = new QStandardItem(tmpList.at(col));
            //item->setTextAlignment(Qt::AlignCenter);

            //最后一列数据
            if(col == END_COLUMN_INDEX)
            {
                if(tmpList.at(col) == "0")
                {
                    item->setCheckState(Qt::Unchecked);
                }
                else
                {
                    item->setCheckState(Qt::Checked);
                }
            }

            //第0列“测深”数据大于300的字符标红
            if(col == 0 && tmpList.at(0).toInt() > 300)
            {
                item->setForeground(brush);
            }
            tableItemModel->setItem(row - 1, col, item);
        }
    }
}

void MainWindow::on_currentItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //消除编译警告
    previous.isValid();

    if(current.isValid())
    {
        labCurCellPos->setText(QString::asprintf("当前单元格: %d 行， %d 列", current.row() + 1, current.column() + 1));
        QStandardItem *curItem = tableItemModel->itemFromIndex(current);
        labCurCellText->setText("单元格内容: " + curItem->text());
    }
}

void MainWindow::onOpenFile()
{
    QString curPath = QCoreApplication::applicationDirPath();
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", curPath, "文本文件(*.txt)");
    if(fileName.isEmpty())
    {
        return;
    }

    QStringList fileContent; //文件内容字符串列表
    QFile dataFile(fileName);
    if(dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&dataFile);
        ui->plainTextEdit->clear();
        in.setCodec(QTextCodec::codecForName("UTF-8")); //解决中文乱码问题

        while(!in.atEnd())
        {
            QString data = in.readLine();
            ui->plainTextEdit->appendPlainText(data);

            fileContent.append(data);
        }
        dataFile.close();
        labCurFile->setText("当前文件: " + fileName);
        initModelFromStringList(fileContent);
    }
}

void MainWindow::onSaveFile()
{
    QString savePath = QCoreApplication::applicationDirPath();
    QString saveFile = QFileDialog::getSaveFileName(this, "保存文件", savePath, "文本文件(*.txt)");
    if(saveFile.isEmpty())
    {
        return;
    }

    QFile txtFile(saveFile);
    if(txtFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&txtFile);
        out.setCodec(QTextCodec::codecForName("UTF-8")); //防止中文乱码

        QStandardItem *item;
        int i, j;
        QString str;

        ui->plainTextEdit->clear();

        //获取表头
        for(i = 0; i < tableItemModel->columnCount(); i++)
        {
            item = tableItemModel->horizontalHeaderItem(i);
            str += item->text() + "\t";
        }
        out << str << "\n"; //每一行添加一个换行符
        ui->plainTextEdit->appendPlainText(str);

        //获取数据
        for(i = 0; i < tableItemModel->rowCount(); i++)
        {
            str = "";
            for(j = 0; j < tableItemModel->columnCount() - 1; j++)
            {
                item = tableItemModel->item(i, j);
                str = str + item->text() + "\t";
            }
            item = tableItemModel->item(i, j);
            if(item->checkState())
            {
                str += "1";
            }
            else
            {
                str += "0";
            }
            ui->plainTextEdit->appendPlainText(str);
            out << str << "\n";
        }
    }
}

void MainWindow::addRow()
{
    QList<QStandardItem*> itemList;
    QStandardItem *item;
    for(int i = 0; i < FIX_COLUMN_COUNT - 1; i++)
    {
        item = new QStandardItem("0");
        itemList << item;
    }
    //获取最后一列的表头文字
    QString str = tableItemModel->headerData(tableItemModel->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    item = new QStandardItem(str); //创建 "测井取样"Item
    item->setCheckable(true);
    itemList << item;   //添加到容器

    //插入一行
    tableItemModel->insertRow(tableItemModel->rowCount(), itemList);

    QModelIndex index = tableItemModel->index(tableItemModel->rowCount(), 0);
    tableSelectionModel->clearSelection();
    tableSelectionModel->setCurrentIndex(index, QItemSelectionModel::Select);
    QScrollBar *verScroll = ui->tableView->verticalScrollBar();
    verScroll->setValue(verScroll->maximumHeight());
}

void MainWindow::delRow()
{
    QModelIndex curIndex = tableSelectionModel->currentIndex();
    tableItemModel->removeRow(curIndex.row());

    if(curIndex.row() != tableItemModel->rowCount())
    {   //删除一行，并重新设置当前选择行
        tableSelectionModel->setCurrentIndex(curIndex, QItemSelectionModel::Select);
    }
}

void MainWindow::onAlignLeft()
{
    onAlign(Qt::AlignLeft);
}

void MainWindow::onAlignRight()
{
    onAlign(Qt::AlignRight);
}

void MainWindow::onAlignCentral()
{
    onAlign(Qt::AlignCenter);
    qDebug() << "on align central!!" << endl;
}

void MainWindow::onBoldFont()
{
    if (!tableSelectionModel->hasSelection())
    {
        return;
    }

    //获取选择的单元格的模型索引列表，可以是多选
    QModelIndexList selectedIndexList = tableSelectionModel->selectedIndexes();
    QModelIndex Index;
    QStandardItem* Item;
    QFont font;
    QBrush brush(Qt::red);
    bool bold = false;

    for (int i = 0; i < selectedIndexList.count();i++)
    {
        Index = selectedIndexList.at(i) ; //获取一个模型索引
        Item = tableItemModel->itemFromIndex(Index);

        font = Item->font();
        bold = font.bold();
        font.setBold(!bold);
        Item->setFont(font);
        Item->setForeground(brush);
    }
}

void MainWindow::onHelp()
{
    helpDialog->exec();
}

void MainWindow::onAlign(Qt::AlignmentFlag align)
{
    if (!tableSelectionModel->hasSelection())
    {
        return;
    }

    //获取选择的单元格的模型索引列表，可以是多选
    QModelIndexList selectedIndexList = tableSelectionModel->selectedIndexes();
    QModelIndex Index;
    QStandardItem* Item;
    for (int i = 0; i < selectedIndexList.count(); i++)
    {
        Index = selectedIndexList.at(i) ; //获取一个模型索引
        Item = tableItemModel->itemFromIndex(Index);
        Item->setTextAlignment(align | Qt::AlignVCenter) ;//设置文字对齐方式
    }
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
    switch(k->key())
    {
        case Qt::Key_Escape:
            close();
        break;
    }
}

void MainWindow::initTableViewHeader()
{
    QStringList stringList;
    for(int i = 0; i < FIX_COLUMN_COUNT; i++)
    {
        stringList << "untitled";
    }
    tableItemModel->setHorizontalHeaderLabels(stringList);

    for(int col = 0; col < FIX_COLUMN_COUNT; col++)
    {
        QStandardItem *item = tableItemModel->horizontalHeaderItem(col);
        if(item != nullptr)
        {
            //QFont font = item->font();
            QBrush brush(Qt::blue);
            //font.setBold(true);
            //item->setFont(font);
            item->setForeground(brush);
        }
    }
}
