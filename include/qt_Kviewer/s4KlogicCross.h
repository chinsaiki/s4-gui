#pragma once

#include "qt_Kviewer/s4KlogicItem.h"


namespace S4{
namespace QT{

//signal Bar for one day or one week and so on...
class KlogicCross_t : public KlogicItem_t
{
public:
public:
    explicit KlogicCross_t(Kinstrument_scene *scene):
        KlogicItem_t(scene)
    {
    }

    ~KlogicCross_t(){
    }

    //NOTE: only support angle=45 or angle=0
    //TODO: more angle
    //w_rate: mulitiple of label step.
    inline void setValue(qreal center_w, qreal center_h, qreal w_rate = 1.0, qreal h_rate = 1.0, qreal angle=45.0)
    {
        _center_w = center_w;
        _center_h = center_h;
        _w_rate = w_rate;
        _h_rate = h_rate;
        _angle = angle;
    }

    inline void setLineWidth(int width) { _line_width = width; }

    inline void setColor(const QColor& c)
    {
        _color = c;
    }

    inline void setLineStyle(Qt::PenStyle s){
        _line_style = s;
    }

    virtual void mkGroupItems() override
    {
        qreal x = _scene->val_w_to_x(_center_w);
        qreal y = _scene->val_h_to_y(_center_h);

        qreal dlt_x = _scene->getCtx().val_w_pxl(); //label step
        qreal dlt_y = _scene->getCtx().val_w_pxl();
        if (_angle == 45.0) {
            dlt_x /= 2;
        }
        dlt_x *= _w_rate;
        dlt_y *= _h_rate;

        qreal x0;
        qreal y0;
        qreal x1;
        qreal y1;

        if (_angle == 45.0) {
            x0 = x - dlt_x;
            y0 = y - dlt_y;
            x1 = x + dlt_x;
            y1 = y + dlt_y;
			if (x0 < 0) x0 = 0;
			if (y0 < 0) y0 = 0;
        }
		else {
			x0 = x - dlt_x;
			y0 = y;
			x1 = x + dlt_x;
			y1 = y;
			if (x0 < 0) x0 = 0;
		}



        QGraphicsLineItem* pLine0 = new QGraphicsLineItem(x0, y0, x1, y1);
        if (_line_width > 0){
            QPen pen(_color, _line_width, _line_style, Qt::RoundCap, Qt::MiterJoin);
            pLine0->setPen(pen);
        }else{
            QPen pen(_color, 1, _line_style, Qt::RoundCap, Qt::MiterJoin);
            pen.setCosmetic(true);
            pLine0->setPen(pen);
        }

        if (_angle == 45.0) {
			x0 = x - dlt_x;
			y0 = y + dlt_y;
			x1 = x + dlt_x;
			y1 = y - dlt_y;
			if (x0 < 0) x0 = 0;
			if (y1 < 0) y1 = 0;
		}
		else {
			x0 = x;
			y0 = y - dlt_y;
			x1 = x;
			y1 = y + dlt_y;
			if (y0 < 0) y0 = 0;
		}
        QGraphicsLineItem* pLine1 = new QGraphicsLineItem(x0, y0, x1, y1);
        if (_line_width > 0){
            QPen pen(_color, _line_width, _line_style, Qt::RoundCap, Qt::MiterJoin);
            pLine1->setPen(pen);
        }else{
            QPen pen(_color, 1, _line_style, Qt::RoundCap, Qt::MiterJoin);
            pen.setCosmetic(true);
            pLine1->setPen(pen);
        }
        
		addToGroup(pLine0);
		addToGroup(pLine1);
    }
private:
    qreal _center_w;
    qreal _center_h;
    qreal _w_rate = 1.0;
    qreal _h_rate = 1.0;
    qreal _angle = 45.0;

    QColor _color;
    int _line_width = 1;
    Qt::PenStyle _line_style = Qt::PenStyle::SolidLine;

    qreal _x_max, _x_min;   //in scene
	qreal _y_top, _y_btm;
private:

};


    
} // namespace QT
} // namespace S4