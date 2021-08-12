#ifndef S4CONSOLE_H
#define S4CONSOLE_H

#include <QDialog>
#include "qt_console/cliparser.h"

namespace Ui {
class s4console;
}

class s4console : public QDialog
{
    Q_OBJECT

public:
    explicit s4console(QWidget *parent = nullptr);
    ~s4console();


signals:
    void signal_load(const std::string&, const std::string&, const std::string&);

    void signal_centerOn_day(int date);
    void signal_crossOn_day(int date);
public slots:
    void emit_load(const std::string& mktCode, const std::string& stgName, const std::string& orderTblName) {
        emit signal_load(mktCode, stgName, orderTblName);
    }

    void emit_centerOn_day(int date) {
        emit signal_centerOn_day(date);
    }

    void emit_crossOn_day(int date) {
        emit signal_crossOn_day(date);
    }
private:
    Ui::s4console *ui;
	cliparser* _cli;
    QWidget *widget;
};

#endif // S4CONSOLE_H
