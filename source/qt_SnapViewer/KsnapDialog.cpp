#include "common/s4conf.h"
#include "common/s4filesystem.h"
#include "qt_SnapViewer/KsnapDialog.h"
#include "ui_KsnapDialog.h"
#include <QDebug>

#define TIME_BEFORE_OTF (90000)
#define TIME_AFTER_OTF (151500)

namespace S4 {
namespace QT {
    
KsnapDialog::KsnapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KsnapDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

	_KsnapViewer = new KsnapWidget(this);
    connect(this, &KsnapDialog::signal_openSnapDbTable, _KsnapViewer, &KsnapWidget::openDsxSnapTab);

    QWidget * widget = new QWidget(this);

    QVBoxLayout * layout = new QVBoxLayout(widget);//铺满布局

    _KsnapViewer->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);//铺满布局

    layout->addWidget(_KsnapViewer);

    ui->verticalLayout->addWidget(widget);

}

KsnapDialog::~KsnapDialog()
{
    delete ui;
}

void KsnapDialog::openSnapDbTable(const std::string& instrument_name, int date)
{
    qDebug() << "openSnapDbTable " << instrument_name.c_str() << " "  << date;
    
    std::vector<std::string> snap_db_root = glb_conf::pInstance()->db_snap().snap5x.roots;
    if (glb_conf::pInstance()->db_snap().snap5x.otf_root.size()) {
        if ((nowWeekDay() > 5 || nowWeekDay() == 0 || nowMinuSec() < TIME_BEFORE_OTF || nowMinuSec() > TIME_AFTER_OTF)) {
            qDebug() << "Include snap5x otf_root=" << glb_conf::pInstance()->db_snap().snap5x.otf_root.c_str();
                snap_db_root.push_back(glb_conf::pInstance()->db_snap().snap5x.otf_root);
        }
    }

    std::string table_name = "d" + std::to_string(date);

    for (auto& rt : snap_db_root){
        auto fs = search_file(rt, instrument_name + ".db", true);

        for (auto& db : fs) {
            sqlite::DB_t snap_db(db.string());
            std::set<std::string> dates = snap_db.get_table_list();
            if (dates.count(table_name)) {
                emit signal_openSnapDbTable(db.string(), table_name, instrument_name + "-" + std::to_string(date));
                return;
            }
        }
    }

    qDebug() << "not found " << instrument_name.c_str() << " "  << date;
}

}
}