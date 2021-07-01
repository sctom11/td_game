#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QMessageBox>

class myMessagebox : public QMessageBox
{
public:
    myMessagebox(QString Title, QString content);
};

#endif // MYMESSAGEBOX_H
