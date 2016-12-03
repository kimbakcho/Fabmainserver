#include "temp_humi_baseview.h"
#include "ui_temp_humi_baseview.h"

temp_humi_baseview::temp_humi_baseview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temp_humi_baseview)
{
    ui->setupUi(this);
    mainchart = new temp_humi_chart();
    chartview = new temp_humi_chartview(mainchart);
    ui->layout_chartview->addWidget(chartview);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(5);
    axisX->setFormat("MM-dd HH:mm:ss");
    axisX->setTitleText("Date");
    mainchart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;

    axisY->setTitleText("err count");
    mainchart->addAxis(axisY, Qt::AlignLeft);
}

temp_humi_chart *temp_humi_baseview::getMainchart() const
{
    return mainchart;
}

void temp_humi_baseview::setMainchart(temp_humi_chart *value)
{
    mainchart = value;
}

QValueAxis *temp_humi_baseview::getAxisY() const
{
    return axisY;
}

void temp_humi_baseview::setAxisY(QValueAxis *value)
{
    axisY = value;
}

QDateTimeAxis *temp_humi_baseview::getAxisX() const
{
    return axisX;
}

void temp_humi_baseview::setAxisX(QDateTimeAxis *value)
{
    axisX = value;
}

temp_humi_chartview *temp_humi_baseview::getChartview() const
{
    return chartview;
}

void temp_humi_baseview::setChartview(temp_humi_chartview *value)
{
    chartview = value;
}

temp_humi_baseview::~temp_humi_baseview()
{
    delete ui;
}
