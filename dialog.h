#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "music.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setData(Music * musicToEdit);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    Music * musicToEdit;
};

#endif // DIALOG_H
