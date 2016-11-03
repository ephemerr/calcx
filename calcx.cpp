#include "calcx.h"
#include "ui_calcx.h"
#include "assert.h"

#include <QKeyEvent>
#include <QWidget>

Calcx::Calcx(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calcx)
{
    ui->setupUi(this);
    create_button(QString("7"),ui->horizontalLayout_1);
    create_button(QString("8"),ui->horizontalLayout_1);
    create_button(QString("9"),ui->horizontalLayout_1);
    create_button(QString("/"),ui->horizontalLayout_1);

    create_button(QString("4"),ui->horizontalLayout_2);
    create_button(QString("5"),ui->horizontalLayout_2);
    create_button(QString("6"),ui->horizontalLayout_2);
    create_button(QString("*"),ui->horizontalLayout_2);

    create_button(QString("1"),ui->horizontalLayout_3);
    create_button(QString("2"),ui->horizontalLayout_3);
    create_button(QString("3"),ui->horizontalLayout_3);
    create_button(QString("-"),ui->horizontalLayout_3);

    create_button(QString("0"),ui->horizontalLayout_4);
    create_button(QString("."),ui->horizontalLayout_4);
    QPushButton * butt_res =
    create_button(QString("="),ui->horizontalLayout_4);
    create_button(QString("+"),ui->horizontalLayout_4);

    connect( butt_res, SIGNAL(clicked()), this, SLOT(on_lineEdit_returnPressed()) );
}

Calcx::~Calcx()
{
    foreach (QObject *obj, button_prints.keys())
    {
        obj->deleteLater();
    }
    delete ui;
}

QPushButton * Calcx::create_button(QString str, QLayout *parent)
{
    QPushButton *butt = new QPushButton();
    parent->addWidget(butt);
    butt->setText(str);
    butt->setObjectName(QString("butt_") + str);
    button_prints.insert(butt,str);
    connect (butt, SIGNAL(clicked()), this, SLOT(on_print_clicked()));
    return butt;
}

void Calcx::on_print_clicked()
{
    QString str = ui->lineEdit->text() + button_prints[sender()];
    ui->lineEdit->setText(str);
}

int test_char(const QString &str, int i) {
    if (i >= str.length()) return 0;
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

inline double mult (double a, double b) {
    return a*b;
}
inline double div (double a, double b) {
    return a/b;
}
inline double sum (double a, double b) {
    return a+b;
}
inline double min (double a, double b) {
    return a-b;
}
typedef double (*Arithmetic)(double, double);

enum {
    CALC_OP_NONE,
    CALC_OP_SUM,
    CALC_OP_MIN,
    CALC_OP_MULT,
    CALC_OP_DIV,
    CALC_OP_UNKNOWN
};

struct Operation {
    int priority;
    Arithmetic calc;
} operations[CALC_OP_UNKNOWN] = {
    {0,NULL},
    {1, sum},
    {1, min},
    {2, mult},
    {2, div}
};

int op_bind(char op){
    switch (op) {
        case '+': return CALC_OP_SUM;
        case '-': return CALC_OP_MIN;
        case '*': return CALC_OP_MULT;
        case '/': return CALC_OP_DIV;
    }
    return CALC_OP_NONE;
}

/// Convert string to reverse polish notation (RPN) list of lexems
QList<Lexem> polish(const QString& str) {
    int len = str.length();
    QList<Lexem> res;
    QStack<Lexem> station;
    for (int i=0; i < len; i++)
    {
        int operand = test_char(str,i);
        char operator_id = operand ? 0 : op_bind(str[i].toLatin1());
        int len = operand ? operand : 1;
        Lexem lexem = Lexem(str.mid(i,len),operator_id);
        i += len-1;
        if (operator_id) {
            while (station.size()) {
                Lexem &top = station.top();
                if (operations[operator_id].priority <=
                        operations[top.second].priority) {
                    res.append(station.pop());
                } else {
                    break;
                }
            }
            station.push(lexem);
        } else {
            res.append(lexem);
        }
    }
    while (station.size()) {
        res.append(station.pop());
    }
    return res;
}

QString lexem_print(QList<Lexem> to_print) {
    QString res = "";
    foreach (Lexem lexem, to_print) {
        res += lexem.first;
        if (!lexem.second) {
            res += ",";
        }
    }
    return res;
}

double Calcx::calc(QList<Lexem> &rpn, int &i)
{
    Lexem &current = rpn[i];
    i--;
    if (current.second == CALC_OP_NONE) {
        double res = current.first.toDouble();
        return res;
    } else {
        double a = calc(rpn,i);
        double b = calc(rpn,i);
        Arithmetic func = operations[current.second].calc;
        return func(b,a);
    }
}

void Calcx::on_lineEdit_returnPressed()
{
    QString exp = QString("[0-9-+*/.]+");
    QValidator *valid = new QRegExpValidator(QRegExp(exp), this);
    QString formula = ui->lineEdit->text();
    formula.replace(" ","");
    formula.replace("=","");
    int pos = 0;
    if (valid->validate(formula,pos) !=  QValidator::Acceptable)
        return;
    QList<Lexem> pol = polish(formula);
    QString rpn = lexem_print(pol);
    int i = pol.size()-1;
    double res = calc(pol, i);
    ui->lineEdit->setText(QString::number(res));
}
