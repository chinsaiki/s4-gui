#ifndef DBCONNECTDIALOG_H
#define DBCONNECTDIALOG_H

#include <QPushButton>
#include <QDialog>
#include <QFile>

#include "DBHandler.h"
#include "ddwidget.h"
#include "qt_common/Utils.h"

namespace Ui {
class DbConnectDialog;
}

namespace S4{
namespace QT{
    

class DbConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DbConnectDialog(QWidget *parent = nullptr);
    ~DbConnectDialog();

     DBHandler* getDBHandler() const;

signals:
    void connectionReady(DBHandler *connection);

private slots:
    void onOpenFileDialog(const QString &path);

private:
    Ui::DbConnectDialog *ui;
    DBHandler *connection;
};

} // namespace QT
} // namespace S4

#endif // DBCONNECTDIALOG_H
