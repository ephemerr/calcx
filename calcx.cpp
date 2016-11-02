#include "calcx.h"
#include "ui_calcx.h"

#include <QPushButton>
#include <QKeyEvent>

Calcx::Calcx(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calcx)
{
    ui->setupUi(this);
    create_button(QString("7"),static_cast<QLayout*>(ui->horizontalLayout_1));
    create_button(QString("8"),static_cast<QLayout*>(ui->horizontalLayout_1));
    create_button(QString("9"),static_cast<QLayout*>(ui->horizontalLayout_1));
    create_button(QString("/"),static_cast<QLayout*>(ui->horizontalLayout_1));

    create_button(QString("4"),static_cast<QLayout*>(ui->horizontalLayout_2));
    create_button(QString("5"),static_cast<QLayout*>(ui->horizontalLayout_2));
    create_button(QString("6"),static_cast<QLayout*>(ui->horizontalLayout_2));
    create_button(QString("*"),static_cast<QLayout*>(ui->horizontalLayout_2));

    create_button(QString("1"),static_cast<QLayout*>(ui->horizontalLayout_3));
    create_button(QString("2"),static_cast<QLayout*>(ui->horizontalLayout_3));
    create_button(QString("3"),static_cast<QLayout*>(ui->horizontalLayout_3));
    create_button(QString("-"),static_cast<QLayout*>(ui->horizontalLayout_3));

    create_button(QString("0"),static_cast<QLayout*>(ui->horizontalLayout_4));
    create_button(QString("."),static_cast<QLayout*>(ui->horizontalLayout_4));
    create_button(QString("="),static_cast<QLayout*>(ui->horizontalLayout_4));
    create_button(QString("+"),static_cast<QLayout*>(ui->horizontalLayout_4));
}

Calcx::~Calcx()
{
    foreach (QObject *obj, button_prints.keys())
    {
        obj->deleteLater();
    }
    delete ui;
}

void Calcx::create_button(QString str, QLayout *parent)
{
    QPushButton *butt = new QPushButton();
    parent->addWidget(butt);
    butt->setText(str);
    button_prints.insert(butt,str);
    connect (butt, SIGNAL(clicked()), this, SLOT(on_print_clicked()));
}

void Calcx::on_print_clicked()
{
    QString str = ui->lineEdit->text() + button_prints[sender()];
    ui->lineEdit->setText(str);
}

int test_char(const QString &str, int i) {
    char c = str[i].toLatin1();
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
        return 1 + test_char(str,i+1);
    }
    return 0; // 0 at operator and >0(size) at operand
}

int op_priority(char op){
    int res=0;
    switch (op) {
        case 0: break;
        case '+':
        case '-':
            res = 1;
            break;
        case '*':
        case '/':
            res = 2;
            break;
    }
    return res;
}

QList<Lexem> polish(const QString& str) {
    int len = str.length();
    QList<Lexem> res;
    QStack<Lexem> station;
    for (int i=0; i < len; i++)
    {
        int operand = test_char(str,i);
        char operator_ = operand ? 0 : str[i].toLatin1();
        int len = operand ? operand : 1;
        Lexem lexem = Lexem(str.mid(i,len),operator_);
        if (operator_) {
            while (station.size()) {
                Lexem &top = station.top();
                if (op_priority( operator_) <= op_priority(top.second)) {
                    res.append(station.pop());
                } else {
                    station.push(lexem);
                    break;
                }
            }
        } else {
            res.append(lexem);
        }
    }
}

QString lexem_print(QList<Lexem> to_print) {
    QString res = "";
    foreach (Lexem lexem, to_print) {
        res += lexem.first;
    }
    return res;
}

double Calcx::parse()
{
    double res = 0;
    return res;
//    bool sign = false;
//    do {
//       char c = str[i];
//       switch (c) {
//       case '-':
//           if (!i) {
//               sign = true;
//               continue;
//           } else {
//               return  sign*res - parse(str+i+1, len-i-1);
//           }
//       case '+':
//           if (!i) {
//               continue;
//           } else {
//               return  sign*res + parse(str+i+1, len-i-1);
//           }
//       case '0':
//       case '1':
//       case '2':
//       case '3':
//       case '4':
//       case '5':
//       case '6':
//       case '7':
//       case '8':
//       case '9':
//           res += c - '0';
//       }
//       if (c <= '9' && c >=0) {
//           double res
//       }
//    }
}

void Calcx::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
        case Qt::Key_Enter:

            break;
        default:
          QWidget::keyPressEvent(event);
    }
}
