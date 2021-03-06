#include "qt_console/s4console.h"
#include "ui_s4console.h"
#include <QSplitter>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>

s4console::s4console(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::s4console)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

	_cli = new cliparser(this);
    connect(_cli, SIGNAL(signal_load(const std::string &, const std::string &, const std::string &)),
        this, SLOT(emit_load(const std::string &, const std::string &, const std::string &)));
    connect(_cli, &cliparser::signal_centerOn_day, this, &s4console::emit_centerOn_day);
    connect(_cli, &cliparser::signal_crossOn_day, this, &s4console::emit_crossOn_day);

    QWidget * widget = new QWidget(this);

    QVBoxLayout * layout = new QVBoxLayout(widget);//铺满布局

    _cli->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);//铺满布局

    layout->addWidget(_cli);

    ui->verticalLayout->addWidget(widget);

}

s4console::~s4console()
{
    delete ui;
}
