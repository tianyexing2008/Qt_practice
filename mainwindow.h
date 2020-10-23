#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

class QLabel;
class QStandardItemModel;
class QItemSelectionModel;
class CHelpDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *);
private:
    void createAction();
    void initStatusBar();
    void initModelFromStringList(QStringList &list);
    void initTableViewHeader();
    void onAlign(Qt::AlignmentFlag align);
private:
    QAction *openAction;
    QAction *saveAction;
    QAction *addAction;
    QAction *delAction;
    QAction *alignLeftAction;
    QAction *alignRightAction;
    QAction *alignCentralAction;
    QAction *boldFontAction;
    QAction *helpAction;

    QLabel *labCurFile;     //当前文件
    QLabel *labCurCellPos;  //当前单元格行、列
    QLabel *labCurCellText; //当前单元格内容

    QStandardItemModel *tableItemModel;   //数据模型
    QItemSelectionModel *tableSelectionModel;//选择模型

private slots:
    void on_currentItemChanged(const QModelIndex &current, const QModelIndex &previous);
    void onOpenFile();
    void onSaveFile();
    void addRow();
    void delRow();
    void onAlignLeft();
    void onAlignRight();
    void onAlignCentral();
    void onBoldFont();
    void onHelp();
private:
    Ui::MainWindow *ui;

    CHelpDialog *helpDialog;
};

#endif // MAINWINDOW_H
