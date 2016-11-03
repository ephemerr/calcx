#ifndef CALCX_H
#define CALCX_H

#include <QMainWindow>
#include <QtCore>
#include <QPushButton>

namespace Ui {
class Calcx;
}

typedef QPair<QString, char> Lexem;
typedef QList<Lexem>::iterator Iterator;

class Calcx : public QMainWindow
{
    Q_OBJECT
    QMap<QObject*, QString> button_prints;
    QPushButton* create_button(QString str, QLayout *parent);
    double calc(QList<Lexem> &rpn, int &i);
public:
    explicit Calcx(QWidget *parent = 0);
    ~Calcx();

private slots:
    void on_print_clicked();
    void on_lineEdit_returnPressed();

private:
    Ui::Calcx *ui;
};

#endif // CALCX_H
