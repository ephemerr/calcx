#ifndef PRINTBUTTON_H
#define PRINTBUTTON_H

#include <QPushButton>

class PrintButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PrintButton(QWidget *parent = 0);
    ~PrintButton();

signals:

public slots:
};

#endif // PRINTBUTTON_H
