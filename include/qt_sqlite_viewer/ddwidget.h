#ifndef DDWIDGET_H
#define DDWIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QLayout>

#include "qt_common/Utils.h"

namespace Ui {
class DDwidget;
}


namespace S4{
namespace QT{
    
class DDwidget : public QWidget
{
    Q_OBJECT

public:
    explicit DDwidget(QWidget *parent = nullptr);
    ~DDwidget();

signals:
    void filePathRecived(const QString& path);

public slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::DDwidget *ui;
    QString dbPath;
};

} // namespace QT
} // namespace S4

#endif // DDWIDGET_H
