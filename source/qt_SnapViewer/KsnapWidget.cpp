#include "qt_SnapViewer/KsnapWidget.h"

#include <QGridLayout>

namespace S4 {
namespace QT {

KsnapWidget::KsnapWidget(QWidget *parent) :
    s4SnapViewerWidgetDsxDB(parent)
{   
	_treeView->setMaximumWidth(0);
}


//打开一个DSX sqlite数据库，并把table添加到dbTree中
void KsnapWidget::onOpenDsxDB()
{
    //do nothing
    return;
}


}
}