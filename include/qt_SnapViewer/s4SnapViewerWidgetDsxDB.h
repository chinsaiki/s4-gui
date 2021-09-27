#pragma once


#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapViewerWidget.h"


namespace S4{
namespace QT{

//    void updateTableList();


class s4SnapViewerWidgetDsxDB : public s4SnapViewerWidget
{
    Q_OBJECT

public:
    explicit s4SnapViewerWidgetDsxDB(QWidget *parent = nullptr);
    virtual ~s4SnapViewerWidgetDsxDB();

public slots:
	virtual void onOpenDsxDB();

	virtual void closeSnapTab(int index);

    virtual void dbTree_doubleClicked(const QModelIndex &index);
	virtual void openDsxSnapTab(const std::string& db_path, const std::string& table_name, const std::string& tab_title);

    virtual void nextDsxSnap();
    // void addDsxSnaps();

signals:
	void signal_status(const QString&);

protected:

	std::map<std::string, std::string> _db_list;

	struct snap_info_t
	{
	};
	std::map<QString, snap_info_t> _instrument_info_cargo;

protected:
	void mouseMoveEvent(QMouseEvent* )
	{
		//qDebug() << "Kview " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
	}

protected:



};

}
}