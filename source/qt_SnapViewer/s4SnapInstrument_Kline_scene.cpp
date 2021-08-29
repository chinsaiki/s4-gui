﻿#include "qt_SnapViewer/s4SnapInstrument_Kline_scene.h"
#include "common/s4time.h"
#include "qt_Kviewer/s4KlogicBar.h"
#include "qt_Kviewer/s4KlogicCurve.h"
#include "qt_Kviewer/s4KlogicTrade.h"
#include "qt_Kviewer/s4KlogicRect.h"

namespace S4{
namespace QT{

#define BAR_Z (51)
#define MA_Z (52)
#define TRADE_Z (60)
    
snapInstrument_Kline_scene::snapInstrument_Kline_scene(QWidget* parent):
    Kinstrument_scene(parent)
{
}

//void snapInstrument_Kline_scene::setInfoKQ(const std::shared_ptr<infKQ_t>& pInfoKQ){
//    _pInfoKQ = pInfoKQ;
//}
//
//void snapInstrument_Kline_scene::setMAmap(const std::shared_ptr<std::map<int, std::shared_ptr<maQ_t>>>& pMAmap){
//    _pMAmap = pMAmap;
//}


void snapInstrument_Kline_scene::paint(const KCtx_t& ctx, std::shared_ptr<data_panel_t> data_panel)
{
	_KCtx = ctx;
	_data_panel = data_panel;

    calcCtx();
    initSceneCanvas();
    paint_infoKQ();
    paint_MAmap();
	paint_trade();
}


//datetime_t or time_t -> date_seq
qreal snapInstrument_Kline_scene::label_w_to_val_w(uint64_t l) const 
{
    uint64_t _l = l;
    if (_KCtx.timeMode == timeMode_t::tDAY) {
        _l = date_to_utc(utc_to_date(l));
    }
    else if (_KCtx.timeMode == timeMode_t::tMINU) {
        time_date_t date;
        time_minuSec_t minu;
        date = utc_to_date(l, &minu);
        minu = minu / 100;
        minu = minu * 100;
        _l = date_to_utc(date, minu);
    }
    else {
        // throw TimerError("unsupport timeMode: not tDAY nor tMINU");
        _l = l;
    }

    if (!_label_map_w.size())
        return 0;

    if (_label_map_w.count(_l))
        return _label_map_w.at(_l);

    if (_l <_label_map_w.begin()->first)
        return _label_map_w.begin()->second;

    if (_l >_label_map_w.rbegin()->first)
        return _label_map_w.rbegin()->second;

    for (auto& m : _label_map_w){
        if (m.first > _l)
            return m.second;
    }
    return _label_map_w.end()->second;
}

qreal snapInstrument_Kline_scene::label_w_to_best_val_h(uint64_t l) const
{
	const std::shared_ptr<infKQ_t> _pInfoKQ = check_data();
	if (!_pInfoKQ)
		return (_ctx.val_h_max() + _ctx.val_h_min()) / 2;

    return _pInfoKQ->getLastInfo(l)->close_fq();
}

QString snapInstrument_Kline_scene::x_to_label_w(qreal x) const 
{
    int val_w = int(x_to_val_w(x) + 0.5);
    QString txt;
    if (_w_map_label.count(val_w)){
        if (_KCtx.timeMode == timeMode_t::tDAY){
            txt.sprintf("%s", date_to_str(utc_to_date(_w_map_label.at(val_w))).c_str());
        }else if (_KCtx.timeMode == timeMode_t::tMINU) {
            txt.sprintf("%s", utc_to_str(_w_map_label.at(val_w)).c_str());
		}
		else if (_KCtx.timeMode == timeMode_t::tSnap) {
			txt.sprintf("%s", utc_to_strMinu(_w_map_label.at(val_w)).c_str());
		}
    }else{
		if (_label_map_w.size() && _label_map_w.rbegin()->second + 1 == val_w) {
			txt.sprintf("Dummy Next");
        }
		else {
			txt.sprintf("N/A");
        }
    }
    return txt;
}

QString snapInstrument_Kline_scene::y_to_label_h(qreal y) const
{
    qreal val_h = y_to_val_h(y);
    QString txt;
    txt.sprintf("%0.2f", val_h / 100.0);    //TODO: tdx stock price precision = 1% of Yuan 精度=分
    return txt;
}

qreal snapInstrument_Kline_scene::val_w_to_near_x(qreal val_w) const
{
    if (_w_map_label.count(val_w)){
        return val_w_to_x(val_w);
    }else if (val_w < _w_map_label.begin()->first){
        return val_w_to_x(_w_map_label.begin()->first);
    }else if (val_w > _w_map_label.rbegin()->first){
        return val_w_to_x(_w_map_label.rbegin()->first);
    }else{
		auto itr = _w_map_label.lower_bound(val_w);
        qreal x = (val_w_to_x(itr->first));
		itr--;
		if (abs(x - x_to_val_w(val_w)) > abs(val_w_to_x(itr->first) - x_to_val_w(val_w))) {
			x = (val_w_to_x(itr->first));
		}
        return x;
    }
}

QPointF snapInstrument_Kline_scene::get_label_near(const QPointF& scene_pos, QPointF& scene_label_pos) const
{
    int val_w = int(x_to_val_w(scene_pos.x()));
    int label_w;
    if (_w_map_label.count(val_w)){
        label_w = _w_map_label.at(val_w);
        scene_label_pos.setX(val_w_to_x(val_w));
    }else if (val_w < _w_map_label.begin()->first){
        label_w = _w_map_label.begin()->second;
        scene_label_pos.setX(val_w_to_x(_w_map_label.begin()->first));
    }else if (val_w > _w_map_label.rbegin()->first){
        label_w = _w_map_label.rbegin()->second;
        scene_label_pos.setX(val_w_to_x(_w_map_label.rbegin()->first));
    }else{
		auto itr = _w_map_label.lower_bound(val_w);
        label_w = itr->second;  //第一个大于或等于
        scene_label_pos.setX(val_w_to_x(itr->first));
		itr--;
		if (abs(scene_label_pos.x() - scene_pos.x()) > abs(val_w_to_x(itr->first) - scene_pos.x())) {
			label_w = itr->second;
			scene_label_pos.setX(val_w_to_x(itr->first));
		}
    }
    
    qreal val_h = y_to_val_h(scene_pos.y());
	scene_label_pos.setY(scene_pos.y());
	return QPointF(label_w, val_h);
}

std::shared_ptr<infKQ_t> snapInstrument_Kline_scene::check_data(void) const
{
    if (!_data_panel){
        return nullptr;
    }

	std::shared_ptr<infKQ_t> pInfoKQ;
	if (_KCtx.timeMode == timeMode_t::tDAY) {
		pInfoKQ = _data_panel->info->getDayKQ();
	}
	else if (_KCtx.timeMode == timeMode_t::tMINU) {
		pInfoKQ = _data_panel->info->getMinuKQ();
	}
	else {  //TODO
		return pInfoKQ;
	}

	if (!pInfoKQ || !pInfoKQ->size())
		return nullptr;
	if (!pInfoKQ->isNewAtBack())
		return nullptr;

	return pInfoKQ;
}

void snapInstrument_Kline_scene::calcCtx(void)
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data();
    if (!_pInfoKQ)
        return;

    ctx_t ctx;
    ctx.set_val_h_max((*_pInfoKQ)[0]->high_fq());
    ctx.set_val_h_min((*_pInfoKQ)[0]->low_fq());
    ctx.set_val_w_max(0);
	ctx.set_val_w_min(0);

    int n = 0;
    _label_map_w.clear();
    _w_map_label.clear();
    for(const auto& d : *_pInfoKQ)
    {
        if (d->high_fq() > ctx.val_h_max()){
            ctx.set_val_h_max(d->high_fq());
        }
        if (d->low_fq() < ctx.val_h_min()){
            ctx.set_val_h_min(d->low_fq());
        }
        ctx.set_val_w_max(n);
        _label_map_w[d->_time] = n;
        _w_map_label[n] = d->_time;
        n++;
    }

    setCtx(ctx);
}

//TODO: save for _view to make label
void snapInstrument_Kline_scene::paint_infoKQ(void)
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data();

    if (!_pInfoKQ)
        return;

    QList<logicBarData_t> bars;
    
    for(const auto& d : *_pInfoKQ)
    {
        logicBarData_t bar;
        bar.seq = label_w_to_val_w(d->_time);
        bar.O = d->open_fq();
        bar.C = d->close_fq();
        bar.H = d->high_fq();
        bar.L = d->low_fq();
        bar.Avg = d->prcAvg_fq();

        if (!bars.size()){
            bar.lastC = bar.O;
        }else{
            bar.lastC = bars.back().C;
        }
        bars.push_back(std::move(bar));
    }
    
    KlogicBarGroup_t* barGroup = new KlogicBarGroup_t(this);
    barGroup->setColor(_colorpalette->positive_boxes[0], _colorpalette->negtive_boxes[0]);
    barGroup->setType(KlogicBar_t::barType_t::BAR_JPN);
    barGroup->setLineWidth(4);
    barGroup->setValue(bars);
    barGroup->mkGroupItems();
    barGroup->setZValue(BAR_Z);
	this->addItem(barGroup);

	/* paint next day */
	logicBarData_t nextDay;
	nextDay.seq = _label_map_w.rbegin()->second + 1;
    
	//up
    nextDay.lastC = nextDay.L = nextDay.O = (*_pInfoKQ->rbegin())->close_fq();
    nextDay.H = nextDay.C = UP_10p((*_pInfoKQ->rbegin())->close_fq());
	KlogicBar_t* barU = new KlogicBar_t(this);
	barU->setColor(_colorpalette->positive_boxes[2], _colorpalette->negtive_boxes[2]);
	barU->setType(KlogicBar_t::barType_t::BAR_USA);
	barU->setLineWidth(0);
	barU->setAlpha(100);
	barU->setValue(nextDay);
	barU->mkGroupItems();
    this->addItem(barU);

	//dn
	nextDay.H = nextDay.O = (*_pInfoKQ->rbegin())->close_fq();
	nextDay.L = nextDay.C = DN_10p((*_pInfoKQ->rbegin())->close_fq());
	KlogicBar_t* barD = new KlogicBar_t(this);
	barD->setColor(_colorpalette->positive_boxes[2], _colorpalette->negtive_boxes[2]);
	barD->setType(KlogicBar_t::barType_t::BAR_USA);
	barD->setLineWidth(0);
	barD->setAlpha(100);
	barD->setValue(nextDay);
	barD->mkGroupItems();
	this->addItem(barD);

}

void snapInstrument_Kline_scene::paint_MAmap(void){
    int n = 0;
    for(auto& m : *_data_panel->info->getMAlib()){
        paint_MA(n++, m.second);
    }
}

//TODO: save ind&scope&value&colors for _view to make label
void snapInstrument_Kline_scene::paint_MA(int ind, const std::shared_ptr<maQ_t>& maQ)
{
    if( !maQ || !maQ->size())
        return;

    QList<QPointF> dots;
    for (auto& ma : *maQ){
        QPointF dot;
        dot.setX(label_w_to_val_w(ma->_time));
        dot.setY(ma->C());
        dots.push_back(std::move(dot));
    }
	KlogicCurve_t* curve = new KlogicCurve_t(this);
	curve->setLineStyle(Qt::PenStyle::SolidLine);
	curve->setColor(_colorpalette->curve[ind % _colorpalette->curve.size()]);
	curve->setLineWidth(1);
	curve->setValue(dots);
	curve->mkGroupItems();
	curve->setZValue(MA_Z);
	this->addItem(curve);
}

void snapInstrument_Kline_scene::paint_trade()
{
    KlogicTrade_t* trade_group = new KlogicTrade_t(this);
	trade_group->setValue(_data_panel->history);
    trade_group->setColor(_colorpalette->positive_boxes[1], _colorpalette->negtive_boxes[1]);
    trade_group->mkGroupItems();
    trade_group->setZValue(TRADE_Z);
	this->addItem(trade_group);

}


bool snapInstrument_Kline_scene::get_valPos(int w_seq, QPointF& val) const
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data();

    if (!_pInfoKQ)
        return false;

    size_t nb;
    if (w_seq >= 0) {
        nb = w_seq % _pInfoKQ->size();
    }
    else {
        nb = (-w_seq) % _pInfoKQ->size();
        if (nb != 0)
            nb = _pInfoKQ->size() - nb;
    }
    
    val.setX(label_w_to_val_w((*_pInfoKQ)[nb]->_time));
    val.setY((*_pInfoKQ)[nb]->open_fq());

    return true;
}

void snapInstrument_Kline_scene::paint(const infSnap5xQ_ptr& pSnaps)
{
    _KCtx.timeMode = timeMode_t::tSnap;
    calcCtx(pSnaps);
    initSceneCanvas();
    paint_Snap_price(pSnaps);
    if (pSnaps->size()) {
        paint_last_close(pSnaps->back()->last_close);
    }
}

void snapInstrument_Kline_scene::paint_last_close(price_t last_close)
{
    if (last_close == 0)
        return;
    val_h_to_y(last_close);
	logicRectData_w_t rec_scop;
	rec_scop.val_h = (last_close + getCtx().val_h_min()) / 2;//val_h_max;
	rec_scop.val_h_scope = last_close - getCtx().val_h_min();
	rec_scop.val_lf = getCtx().val_w_min();
	rec_scop.val_rt = getCtx().val_w_max();
	rec_scop.positive = false;
	KlogicRect_w_t* rect = new KlogicRect_w_t(this);
	rect->setValue(rec_scop);
	rect->setColor(_colorpalette->positive_boxes[0], _colorpalette->negtive_boxes[0]);
	rect->setAlpha(20);
	rect->setZValue(0);
	rect->mkGroupItems();
    this->addItem(rect);
}


void snapInstrument_Kline_scene::calcCtx(const infSnap5xQ_ptr& pSnaps)
{
    if (!pSnaps)
        return;

    const time_minuSec_t KTP_BGN_0 = KTP_STK_PBREAK + 100;
    const time_minuSec_t KTP_END_0 = KTP_STK_TRADE1 - 100;
    const int KTM_0 = KTM_STK_PBREAK - 2;

    const time_minuSec_t KTP_BGN_1 = KTP_STK_BREAK + 100;
    const time_minuSec_t KTP_END_1 = KTP_STK_TRADE2 - 4100;
    const int KTM_1 = KTM_STK_BREAK - 2;

    ctx_t ctx;
    // ctx.set_val_h_max(pSnaps->front()->high);
    // ctx.set_val_h_min(pSnaps->front()->low);
    ctx.set_val_h_max(pSnaps->front()->up20P());
    ctx.set_val_h_min(pSnaps->front()->dn20P());
    ctx.set_val_w_max(0);
	ctx.set_val_w_min(0);

    time_t bgn_time = pSnaps->front()->_time;

    if (pSnaps->front()->_MinuSec > KTP_END_0)
        bgn_time -= KTM_0 * 60;
    if (pSnaps->front()->_MinuSec > KTP_END_1)
        bgn_time -= KTM_1 * 60;


    _label_map_w.clear();
    _w_map_label.clear();
    time_t dlt_time = 0;
    for(const auto& d : *pSnaps)
    {
        // if (d->high > ctx.val_h_max()){
        //     ctx.set_val_h_max(d->high);
        // }
        // if (d->low < ctx.val_h_min()){
        //     ctx.set_val_h_min(d->low);
        // }
        dlt_time = d->_time - bgn_time;
        if (d->_MinuSec >= KTP_BGN_0 && d->_MinuSec <= KTP_END_0) continue;
        if (d->_MinuSec >= KTP_BGN_1 && d->_MinuSec <= KTP_END_1) continue;
        if (d->_MinuSec > KTP_END_0)
            dlt_time -= KTM_0 * 60;
        if (d->_MinuSec > KTP_END_1)
            dlt_time -= KTM_1 * 60;

        _label_map_w[d->_time] = dlt_time;
        _w_map_label[dlt_time] = d->_time;

    }


    ctx.set_val_w_max(dlt_time);    //画布中也不含中场休息, val_w = dlt-time

    ctx.set_val_h_10percent_pxl(2048);
    ctx.set_val_w_pxl(4);

    setCtx(ctx);
}


void snapInstrument_Kline_scene::paint_Snap_price(const infSnap5xQ_ptr& pSnaps)
{
    if( !pSnaps || !pSnaps->size())
        return;

    QList<QPointF> dots;
    for (auto& pSnap : *pSnaps){
        QPointF dot;
        dot.setX(label_w_to_val_w(pSnap->_time));
        if (pSnap->price != 0){
            dot.setY(pSnap->price);
        }else{
            if (pSnap->ask1){
                dot.setY(pSnap->ask1);
            }else if (pSnap->bid1){
                dot.setY(pSnap->bid1);
            }else{
                dot.setY(pSnap->last_close);
            }
        }
        dots.push_back(std::move(dot));
    }
	KlogicCurve_t* curve = new KlogicCurve_t(this);
	curve->setLineStyle(Qt::PenStyle::SolidLine);
	curve->setColor(_colorpalette->curve[0]);
	curve->setLineWidth(1);
    curve->setDotSize(1);
	curve->setValue(dots);
	curve->mkGroupItems();
	curve->setZValue(BAR_Z);
	this->addItem(curve);

}





} // namespace QT
} // namespace S4


