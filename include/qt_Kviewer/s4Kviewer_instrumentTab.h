#pragma once

#include "qt_Kviewer/s4Kinstrument.h"
#include "qt_common/s4qt_data_if.h"

#include <QTabWidget>
#include <QKeyEvent>

namespace S4{
namespace QT{
    
class Kviewer_instrumentTab: public QTabWidget
{
    Q_OBJECT
public:
    explicit Kviewer_instrumentTab(QWidget *parent = nullptr):
        QTabWidget(parent)
    {
        setMouseTracking(true);
        setTabsClosable(true);
	    connect(this, &QTabWidget::tabCloseRequested, this, &Kviewer_instrumentTab::closeTab);
    }

    void addInstrument(const data_panel_t& data_panel){
        //day
        Kinstrument* K = new Kinstrument(this);
        K->setInstrument(data_panel);
        connect(K, &Kinstrument::signal_day_selected, this, &Kviewer_instrumentTab::signal_day_selected);
        int i = addTab(K, data_panel.info->mktCodeStr().c_str());
        setCurrentIndex(i);
	}
signals:
    void signal_day_selected(const std::string& instrument_name, time_date_t date);


public slots:
	//seq >=0: next; <0: last
	void slot_next_trade(int seq)
	{
		Kinstrument* K = (Kinstrument*)currentWidget();
        K->slot_next_trade(seq);
	}

	void slot_centerOn_day(int date)
	{
		Kinstrument* K = (Kinstrument*)currentWidget();
        K->slot_centerOn_day(date);
	}

	void slot_crossOn_day(int date)
	{
		Kinstrument* K = (Kinstrument*)currentWidget();
        K->slot_crossOn_day(date);
	}

    void closeTab(int index)
    {
        removeTab(index);
    }

};


} // namespace QT
} // namespace S4
