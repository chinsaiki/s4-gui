#include "qt_Kviewer/s4Kinstrument_indicator_tab.h"

namespace S4{
namespace QT{
    
Kinstrument_indicator_tab::Kinstrument_indicator_tab(QWidget *parent):
    QTabWidget(parent)
{
    setMouseTracking(true);
}


void Kinstrument_indicator_tab::setInstrument(std::shared_ptr<data_panel_t> data_panel){
    _data_panel = data_panel;
}


void Kinstrument_indicator_tab::paint(Kinstrument_indicator_scene::ind_type type, timeMode_t timeMode)
{
    for(auto& ctx : _tab_ctx){
        if ( ctx.second.timeMode == timeMode && ctx.second.type == type){
            setCurrentIndex(ctx.first);
            return;
        }
    }

    //need new tab
    Kinstrument_indicator_scene::indCtx_t indCtx;
    indCtx.type = type;
    indCtx.timeMode = timeMode;

    Kinstrument_indicator_scene* scene = new Kinstrument_indicator_scene(this);
    scene->setCoorType(coor_type_t::VALUE);
    scene->paint(indCtx, _data_panel);

    Kinstrument_indicator_view* view = new Kinstrument_indicator_view(scene, this);
    view->setCoorType(coor_type_t::VALUE);
	view->setTimeMode(timeMode);
    view->paint();
    int i = addTab(view, indCtx.toString());
    setCurrentIndex(i);
    _tab_ctx[i] = indCtx;

	connect(this, SIGNAL(signalViewEvent(std::shared_ptr<view_event>)), view, SLOT(slotViewEvent(std::shared_ptr<view_event>)));

}

void Kinstrument_indicator_tab::slotViewEvent(std::shared_ptr<view_event> event)
{
	emit signalViewEvent(event);
}


void Kinstrument_indicator_tab::slot_centerOn_day(int date)
{
	Kinstrument_indicator_view* _I_view = (Kinstrument_indicator_view*)currentWidget();
	_I_view->slot_centerOn_day(date);
}

void Kinstrument_indicator_tab::slot_crossOn_day(int date)
{
	Kinstrument_indicator_view* _I_view = (Kinstrument_indicator_view*)currentWidget();
	_I_view->slot_crossOn_day(date);
}


} // namespace QT
} // namespace S4
