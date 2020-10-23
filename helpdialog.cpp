#include "helpdialog.h"
#include <QCoreApplication>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTextCodec>
#include <QHBoxLayout>

CHelpDialog::CHelpDialog(QWidget *parent): QDialog(parent)
{
//    helpInfo = new QPlainTextEdit(this);

//    QString info;
//    QString path = QCoreApplication::applicationDirPath();
//    QFile file(path + "/help.txt");
//    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        QTextStream out(&file);
//        out.setCodec(QTextCodec::codecForName("UTF-8"));
//        info = out.readAll();
//    }
//    if(!info.isEmpty())
//    {
//        helpInfo->appendPlainText(info);
//        helpInfo->setFixedSize(400, 230);
//    }

//    helpInfo->setReadOnly(true);

    labHelp = new QLabel();
    QString info = "1.打开一个纯文本文件，该文件是规则的二维数据文件，通过字符串处理获取表头和各行各列的数据，导入到一个 QStandardItemModel 数据模型。\n" \
            "2.编辑修改数据模型的数据，可以插入行、添加行、删除行，还可以在 QTableView 视图组件中直接修改单元格的数据内容。\n" \
            "3.可以设置数据模型中某个项的不同角色的数据，包括文字对齐方式、字体是否粗体等。\n" \
            "4.通过 QItemSelectionModel 获取视图组件上的当前单元格，以及选择单元格的范围，对选择的单元格进行操作。\n" \
            "5.将数据模型的数据内容显示到 QPlainTextEdit 组件里，显示数据模型的内容，检验视图组件上做的修改是否与数据模型同步。\n" \
            "6.将修改后的模型数据另存为一个文本文件。\n";
    labHelp->setText(info);
    labHelp->adjustSize();
    //labHelp->setStyle();

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(labHelp);

    setLayout(mainLayout);
    setWindowTitle("Help");

}

CHelpDialog::~CHelpDialog()
{

}

