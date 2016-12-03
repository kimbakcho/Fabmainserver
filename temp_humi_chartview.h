#ifndef TEMP_HUMI_CHARTVIEW_H
#define TEMP_HUMI_CHARTVIEW_H

#include <QObject>
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QDebug>
#include <QPointF>
QT_CHARTS_USE_NAMESPACE

class temp_humi_chartview : public QChartView
{
public:
    temp_humi_chartview(QChart *chart, QWidget *parent = 0);
protected:
    bool viewportEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    bool m_isTouching;
};

#endif // TEMP_HUMI_CHARTVIEW_H
