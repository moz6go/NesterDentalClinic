#ifndef COPYRESTOREDBDIALOG_H
#define COPYRESTOREDBDIALOG_H

#include "general.h"

namespace Ui {
class CopyRestoreDbDialog;
}

class CopyRestoreDbDialog : public QDialog
{
    Q_OBJECT
    Ui::CopyRestoreDbDialog *ui;
public:
    explicit CopyRestoreDbDialog(QWidget *parent = nullptr);
    QString GetOption();
    ~CopyRestoreDbDialog();
};

#endif // COPYRESTOREDBDIALOG_H
