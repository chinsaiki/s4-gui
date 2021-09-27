#pragma once


#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapViewerWidgetDsxDB.h"


namespace S4{
namespace QT{


class KsnapWidget : public s4SnapViewerWidgetDsxDB
{
    Q_OBJECT

public:
    explicit KsnapWidget(QWidget *parent = nullptr);
    virtual ~KsnapWidget() {}

public slots:
	virtual void onOpenDsxDB();


};

}
}