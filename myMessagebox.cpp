#include "myMessagebox.h"

myMessagebox::myMessagebox(QString title, QString content)
{
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox -> setWindowTitle(title);
    msgBox -> setText(content);
    msgBox -> setStandardButtons(QMessageBox::Yes);
    msgBox -> setDefaultButton(QMessageBox::Yes);
    msgBox -> exec();
}
