#ifndef TEMP_HUMI_BASEVIEW_H
#define TEMP_HUMI_BASEVIEW_H

#include <QWidget>
#include <temp_humi_chart.h>
#include <temp_humi_chartview.h>
#include <QDateTimeAxis>
#include <QValueAxis>

namespace Ui {
class temp_humi_baseview;
}

class temp_humi_baseview : public QWidget
{
    Q_OBJECT

public:
    explicit temp_humi_baseview(QWidget *parent = 0);
    temp_humi_chartview *chartview;
    temp_humi_chart *mainchart;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    ~temp_humi_baseview();
    temp_humi_chartview *getChartview() const;
    void setChartview(temp_humi_chartview *value);

    QDateTimeAxis *getAxisX() const;
    void setAxisX(QDateTimeAxis *value);

    QValueAxis *getAxisY() const;
    void setAxisY(QValueAxis *value);

    temp_humi_chart *getMainchart() const;
    void setMainchart(temp_humi_chart *value);

private slots:

private:
    Ui::temp_humi_baseview *ui;
};

#endif // TEMP_HUMI_BASEVIEW_H
