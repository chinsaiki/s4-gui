#pragma once

#include "types/s4type.h"
#include "qt_common/sharedCharArray_ptr.h"
#include "data/s4infSnap5x.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>


namespace S4{
namespace QT{
    
//tabs, = [market]+[basic]

class snapInstrument_table: public QTabWidget
{
    Q_OBJECT
public:
    snapInstrument_table(int snapLeves_nb, QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent* )
    {
        //qDebug() << "snapInstrument_table " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
    }
    
public slots:

    void addSnaps(const infSnap5xQ_ptr&);
    // void addOrders(const std::vector<ssz_sbe_order_t>&);
    // void addExecs(const std::vector<ssz_sbe_exec_t>&);

    //TODO
    void addBasic(){}
    
public slots:
	void onL2Data_instrument_snap(const S4::sharedCharArray_ptr&);
	void onL2Data_index_snap(const S4::sharedCharArray_ptr&);
	void onL2Data_order(const S4::sharedCharArray_ptr&);
	void onL2Data_exec(const S4::sharedCharArray_ptr&);
signals:
	void signal_L2Data_instrument_snap(const S4::sharedCharArray_ptr&);
	void signal_L2Data_index_snap(const S4::sharedCharArray_ptr&);
	void signal_L2Data_order(const S4::sharedCharArray_ptr&);
	void signal_L2Data_exec(const S4::sharedCharArray_ptr&);
    
private:
    // std::map<int, Kinstrument_indicator_scene::indCtx_t> _tab_ctx;

    QWidget* _market;
    QWidget* _basic;

};


} // namespace QT
} // namespace S4
