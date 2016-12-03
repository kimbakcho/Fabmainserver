#include "temp_humi_chart.h"

temp_humi_chart::temp_humi_chart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
{
    setTitle("");
    setAcceptHoverEvents(true);

}

void temp_humi_chart::movetopoint()
{
    qreal x = plotArea().width() / ((QValueAxis *)axisX())->tickCount();
    scroll(x,0);
}
