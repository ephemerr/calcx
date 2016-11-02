#ifndef CALCX_H
#define CALCX_H

#include <QMainWindow>
#include <QtCore>

namespace Ui {
class Calcx;
}

typedef QPair<QString, bool> Lexem;

class Calcx : public QMainWindow
{
    Q_OBJECT
    QMap<QObject*, QString> button_prints;
    void create_button(QString str, QLayout *parent);
public:
    explicit Calcx(QWidget *parent = 0);
    ~Calcx();

private slots:
    void on_print_clicked();
    double parse();
    void keyPressEvent(QKeyEvent * event);

private:
    Ui::Calcx *ui;
};

#endif // CALCX_H
