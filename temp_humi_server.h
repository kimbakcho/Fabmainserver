#ifndef TEMP_HUMI_SERVER_H
#define TEMP_HUMI_SERVER_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts/QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDir>
#include <QDateTime>
#include  <QThread>
#include <smtp/SmtpMime>
#define RoundOff(x, dig) (floor((x) * pow(10,dig) + 0.5) / pow(10,dig))
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class temp_humi_server;
}

class temp_humi_server : public QWidget
{
    Q_OBJECT

public:
    explicit temp_humi_server(QWidget *parent = 0);
    QSqlDatabase db;

    QByteArray join_data;
    QByteArrayList datalist;
    QTimer temp_humi_timer;
    QTimer send_message_timer;
    QTcpSocket conn_socket;
    QLineSeries *line_ASSML1_2_temp;
    QLineSeries *line_ASSML1_2_humi;
    QLineSeries *line_track_temp;
    QLineSeries *line_track_humi;
    QLineSeries *line_krf_temp;
    QLineSeries *line_krf_humi;

    QLineSeries *line_deposition_temp;
    QLineSeries *line_deposition_humi;
    QLineSeries *line_etching1_temp;
    QLineSeries *line_etching1_humi;
    QLineSeries *line_etching2_temp;
    QLineSeries *line_etching2_humi;
    QLineSeries *line_probe1_temp;
    QLineSeries *line_probe1_humi;
    QLineSeries *line_probe2_temp;
    QLineSeries *line_probe2_humi;

    QLineSeries *line_DI_resistance;
    QLineSeries *line_DI_temp;



    QStandardItemModel time_plan_modle;
    QStandardItemModel email_plan_modle;
    QStandardItemModel send_time_plan;
    bool roomtemp_humi_data_flag;
    bool roomdi_resistance_flag;
    bool roomdi_temp_flag;

    bool specout_check();
    ~temp_humi_server();
private slots:
    void temp_geter_timeout();
    void connect_socket();
    void readready_socket();
    void send_message_timeout();
    void create_chart(int specout_check_time);
    void closeEvent(QCloseEvent *event);
    void send_email(QString file_path,int spec_check_time);
    void on_sendmail_btn_clicked();

private:

    Ui::temp_humi_server *ui;
};

#endif // TEMP_HUMI_SERVER_H
