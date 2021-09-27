#pragma once

#include <QDialog>
#include "qt_SnapViewer/KsnapWidget.h"

namespace Ui {
class KsnapDialog;
}

namespace S4{
namespace QT{

class KsnapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KsnapDialog(QWidget *parent = nullptr);
    ~KsnapDialog();

signals:
    void signal_openSnapDbTable(const std::string& db_path, const std::string& table_name, const std::string& tab_title);

public slots:
	virtual void openSnapDbTable(const std::string& instrument_name, int date);

private:
    Ui::KsnapDialog *ui;
	KsnapWidget* _KsnapViewer;
    QWidget *widget;
};

}
}
