#ifndef TEMP_HUMI_CHART_H
#define TEMP_HUMI_CHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QLineSeries>
#include <QValueAxis>
QT_CHARTS_USE_NAMESPACE

class temp_humi_chart : public QChart
{
public:
    temp_humi_chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    void movetopoint();

};

#endif // TEMP_HUMI_CHART_H
