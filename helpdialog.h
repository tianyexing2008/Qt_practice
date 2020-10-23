#ifndef HELPDIALOG
#define HELPDIALOG

#include <QDialog>

//class QPlainTextEdit;
class QLabel;

class CHelpDialog: public QDialog
{
    Q_OBJECT
public:
    CHelpDialog(QWidget *parent = 0);
    ~CHelpDialog();

private:
    QLabel  *labHelp;
    //QPlainTextEdit *helpInfo;

};
#endif // HELPDIALOG

