#include "temp_humi_server.h"
#include "ui_temp_humi_server.h"

temp_humi_server::temp_humi_server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temp_humi_server)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL","temp_db");
    db.setHostName("fabsv.wisol.co.kr");
    db.setUserName("EIS");
    db.setPassword("wisolfab!");
    db.setDatabaseName("FAB");
    if(!db.open()){
        qDebug()<<"open false";
    }

    conn_socket.connectToHost("10.22.10.60",2004);

    connect(&temp_humi_timer,SIGNAL(timeout()),this,SLOT(temp_geter_timeout()));
    connect(&conn_socket,SIGNAL(readyRead()),this,SLOT(readready_socket()));
    connect(&send_message_timer,SIGNAL(timeout()),this,SLOT(send_message_timeout()));

    ui->time_list->setModel(&time_plan_modle);
    ui->email_list->setModel(&email_plan_modle);
    ui->send_timelist->setModel(&send_time_plan);

    QSqlQuery query(db);
    query.exec("select * from romm_temp_send_check_time");
    while(query.next()){
        QString time = query.value("check_time").toTime().toString("hh:mm");
        time_plan_modle.appendRow(new QStandardItem(time));
    }

    query.exec("select * from room_temp_send_plan");
    while(query.next()){
        QString time = query.value("send_time").toTime().toString("hh:mm");
        email_plan_modle.appendRow(new QStandardItem(time));
    }

    query.exec("select * from room_temp_send_emaillist");
    while(query.next()){
        QString time = query.value("email").toString();
        send_time_plan.appendRow(new QStandardItem(time));
    }

    create_chart();

    temp_humi_timer.setInterval(5000);
    temp_humi_timer.start();

    send_message_timer.setInterval(5000);
    send_message_timer.start();
}

bool temp_humi_server::spacout_check()
{
    bool result = false;
    QDateTime search_start_time = QDateTime::currentDateTime().addDays(-1);
    search_start_time.setTime(QTime(0,0,0));
    QDateTime search_end_time = QDateTime::currentDateTime();

    QSqlQuery query1(db);
    QString query1txt = QString("select * from room_temp_humi where check_time BETWEEN '%1' AND '%2' order by check_time desc;").arg(search_start_time.toString("yyyy-MM-dd hh:mm")).arg(search_end_time.toString("yyyy-MM-dd hh:mm"));
    query1.exec(query1txt);
    QSqlQuery query2(db);
    QString query2txt = QString("select * from room_temp_humi_spec");
    query2.exec(query2txt);

    query1.next();
    double light1_temp = query1.value("light1_temp").toDouble();
    query2.first();
    double lightroomtemp_high_spce = query2.value("lightroomtemp_spec").toDouble();
    query2.last();
    double lightroomtemp_low_spce = query2.value("lightroomtemp_spec").toDouble();

    if(light1_temp>=lightroomtemp_high_spce){
        result = true;
    }else if(light1_temp<=lightroomtemp_low_spce) {
        result = true;
    }
    double light2_temp = query1.value("light2_temp").toDouble();
    if(light2_temp>=lightroomtemp_high_spce){
        result = true;
     }else if(light2_temp<=lightroomtemp_low_spce) {
        result = true;
     }
     double light3_temp = query1.value("light3_temp").toDouble();

     if(light3_temp>=lightroomtemp_high_spce){
         result = true;
     }else if(light3_temp<=lightroomtemp_low_spce) {
         result = true;
     }

     double light1_humi = query1.value("light1_humi").toDouble();
     query2.first();
     double lightroomhumi_high_spce = query2.value("lightroomhumi_spec").toDouble();
     query2.last();
     double lightroomhumi_low_spce = query2.value("lightroomhumi_spec").toDouble();

     if(light1_humi>=lightroomhumi_high_spce){
         result = true;
     }else if(light1_humi<=lightroomhumi_low_spce) {
         result = true;
     }
     double light2_humi = query1.value("light2_humi").toDouble();
     if(light2_humi>=lightroomhumi_high_spce){
         result = true;
     }else if(light2_humi<=lightroomhumi_low_spce) {
         result = true;
     }
     double light3_humi = query1.value("light3_humi").toDouble();

     if(light3_humi>=lightroomhumi_high_spce){
         result = true;
     }else if(light3_humi<=lightroomhumi_low_spce) {
         result = true;
     }


     query2.first();
     double normaltemp_high_spce = query2.value("normalroomtemp_spec").toDouble();
     query2.last();
     double normaltemp_low_spce = query2.value("normalroomtemp_spec").toDouble();
      double deposition_temp = query1.value("deposition_temp").toDouble();


     if(deposition_temp>=normaltemp_high_spce){
         result = true;
     }else if(deposition_temp<=normaltemp_low_spce) {
         result = true;
     }
     double etching1_temp = query1.value("etching1_temp").toDouble();

     if(etching1_temp>=normaltemp_high_spce){
         result = true;
     }else if(etching1_temp<=normaltemp_low_spce) {
         result = true;
     }
     double etching2_temp = query1.value("etching2_temp").toDouble();


     if(etching2_temp>=normaltemp_high_spce){
         result = true;
     }else if(etching2_temp<=normaltemp_low_spce) {
         result = true;
     }
     double probe1_temp = query1.value("probe1_temp").toDouble();


     if(probe1_temp>=normaltemp_high_spce){
         result = true;
     }else if(probe1_temp<=normaltemp_low_spce) {
         result = true;
     }
     double probe2_temp = query1.value("probe2_temp").toDouble();

     if(probe2_temp>=normaltemp_high_spce){
         result = true;
     }else if(probe2_temp<=normaltemp_low_spce) {
         result = true;
     }
     query2.first();
     double normalroomhumi_high_spce = query2.value("normalroomhumi_spec").toDouble();
     query2.last();
     double normalroomhumi_low_spce = query2.value("normalroomhumi_spec").toDouble();

     double deposition_humi = query1.value("deposition_humi").toDouble();

     if(deposition_humi>=normalroomhumi_high_spce){
         result = true;
     }else if(deposition_humi<=normalroomhumi_low_spce) {
         result = true;
     }
     double etching1_humi = query1.value("etching1_humi").toDouble();

     if(etching1_humi>=normalroomhumi_high_spce){
        result = true;
     }else if(etching1_humi<=normalroomhumi_low_spce) {
         result = true;
     }
     double etching2_humi = query1.value("etching2_humi").toDouble();

     if(etching2_humi>=normalroomhumi_high_spce){
        result = true;
     }else if(etching2_humi<=normalroomhumi_low_spce) {
        result = true;
     }
     double probe1_humi = query1.value("probe1_humi").toDouble();

     if(probe1_humi>=normalroomhumi_high_spce){
         result = true;
     }else if(probe1_humi<=normalroomhumi_low_spce) {
        result = true;
     }
     double probe2_humi = query1.value("probe2_humi").toDouble();

     if(probe2_humi>=normalroomhumi_high_spce){
         result = true;
      }else if(probe2_humi<=normalroomhumi_low_spce) {
         result = true;
     }
     return result;

}

temp_humi_server::~temp_humi_server()
{
    delete ui;
}

void temp_humi_server::temp_geter_timeout()
{
    QSqlQuery query(db);
    query.exec("select * from romm_temp_send_check_time");
    bool check_time = false;
    while(query.next()){
        QTime time = query.value("check_time").toTime();
        QTime currenttime = QTime::currentTime();
        int statue = query.value("checked").toInt();
        if((time.hour() == currenttime.hour()) && (time.minute() == currenttime.minute()) && (statue == 0)){
            if(conn_socket.state()==QTcpSocket::ConnectedState){
                QByteArray write_data;
                unsigned char send_data[]={0x4c,0x53,0x49,0x53,0x2d,0x58,0x47,0x54,0x00,0x00,0x00,0x00,0xa0,
                             0x33,0x07,0x00,0x13,0x00,0x00,0x48,0x54,0x00,0x14,0x00,0x00,0x00,
                             0x01,0x00,0x07,0x00,0x25,0x44,0x42,0x32,0x30,0x30,0x30,0x64,0x00};
                write_data.append((char *)send_data,sizeof(send_data));
                conn_socket.write((char *)send_data,sizeof(send_data));
                conn_socket.flush();
                check_time = true;
                QSqlQuery query2(db);
                query2.exec("UPDATE `romm_temp_send_check_time` SET `checked`='0'");
                QString Time_str = query.value("check_time").toTime().toString("hh:mm:ss");
                QString query_txt1 = QString("UPDATE `romm_temp_send_check_time` SET `checked`='1' WHERE `check_time`='%1'").arg(Time_str);
                query2.exec(query_txt1);
            }else if(conn_socket.state() == QTcpSocket::UnconnectedState){
                conn_socket.connectToHost("10.22.10.60",2004);
            }
        }
    }
    if(!check_time){
        return;
    }
}

void temp_humi_server::connect_socket()
{




}

void temp_humi_server::readready_socket()
{
    QByteArray data = conn_socket.readAll();
    QByteArray find_word;
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);

    for(int i=0;i<data.count();i++){
        join_data.append(data.at(i));
        if(join_data.count()==1 && data.at(i) != 0x4c){
            join_data.clear();
        }
        if(join_data.count()>=8){
             if(QString(join_data) == "LSIS-XGT"){
                if(join_data.indexOf(find_word)>=0){
                    datalist.append(join_data);
                    join_data.clear();
                }
             }
        }
    }
    for(int i=0;i<datalist.count();i++){
        QByteArray reslut_data = datalist.at(i);
        //애칭#1 data
        short temp_data = (unsigned char)reslut_data.at(35);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(34);
        short humi_data = (unsigned char)reslut_data.at(37);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(36);
        double etching1_temp_data_real = (double)temp_data/(double)10.0;
        double etching1_humi_data_real = (double)humi_data/(double)10.0;

        //노광#1 data
        temp_data = (unsigned char)reslut_data.at(39);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(38);
        humi_data = (unsigned char)reslut_data.at(41);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(40);
        double light1_temp_data_real = (double)temp_data/(double)10.0;
        double light1_humi_data_real = (double)humi_data/(double)10.0;

        //성막 온습도
        temp_data = (unsigned char)reslut_data.at(43);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(42);
        humi_data = (unsigned char)reslut_data.at(45);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(44);
        double deposition_temp_data_real = (double)temp_data/(double)10.0;
        double deposition_humi_data_real = (double)humi_data/(double)10.0;

        //노광#3 온습도
        temp_data = (unsigned char)reslut_data.at(47);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(46);
        humi_data = (unsigned char)reslut_data.at(49);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(48);
        double light3_temp_data_real = (double)temp_data/(double)10.0;
        double light3_humi_data_real = (double)humi_data/(double)10.0;

        //프로브1 온습도
        temp_data = (unsigned char)reslut_data.at(51);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(50);
        humi_data = (unsigned char)reslut_data.at(53);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(52);
        double probe1_temp_data_real = (double)temp_data/(double)10.0;
        double probe1_humi_data_real = (double)humi_data/(double)10.0;

        //에칭#2 온습도
        temp_data = (unsigned char)reslut_data.at(55);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(54);
        humi_data = (unsigned char)reslut_data.at(57);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(56);
        double etching2_temp_data_real = (double)temp_data/(double)10.0;
        double etching2_humi_data_real = (double)humi_data/(double)10.0;

        //노광2 온습도
        temp_data = (unsigned char)reslut_data.at(59);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(58);
        humi_data = (unsigned char)reslut_data.at(61);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(60);
        double light2_temp_data_real = (double)temp_data/(double)10.0;
        double light2_humi_data_real = (double)humi_data/(double)10.0;

        //프로브2 온습도
        temp_data = (unsigned char)reslut_data.at(63);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(62);
        humi_data = (unsigned char)reslut_data.at(65);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(64);
        double probe2_temp_data_real = (double)temp_data/(double)10.0;
        double probe2_humi_data_real = (double)humi_data/(double)10.0;

        //약품 창고 온도
        temp_data = (unsigned char)reslut_data.at(67);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(66);
        double drug_temp_data_real = (double)temp_data/(double)10.0;

        //리프트오프룸 온습도
        temp_data = (unsigned char)reslut_data.at(75);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(74);
        humi_data = (unsigned char)reslut_data.at(77);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(76);
        double liftoff_temp_data_real = (double)temp_data/(double)10.0;
        double liftoff_humi_data_real = (double)humi_data/(double)10.0;

        //krf룸 온습도
        temp_data = (unsigned char)reslut_data.at(71);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(70);
        humi_data = (unsigned char)reslut_data.at(73);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(72);
        double krf_temp_data_real = (double)temp_data/(double)10.0;
        double krf_humi_data_real = (double)humi_data/(double)10.0;


        QSqlQuery query(db);
        QString quert_txt = QString("INSERT INTO `room_temp_humi` (`check_time`, "
                            "`etching1_temp`, `etching1_humi`, "
                            "`light1_temp`, `light1_humi`, "
                            "`deposition_temp`, `deposition_humi`, "
                            "`KRF_temp`, `KRF_humi`, "
                            "`probe1_temp`,`probe1_humi`,"
                            "`light2_temp`,`light2_humi`,"
                            "`light3_temp`,`light3_humi`,"
                            "`liftoff_temp`,`liftoff_humi`,"
                            "`etching2_temp`,`etching2_humi`,"
                            "`probe2_temp`,`probe2_humi`,"
                            "`drug_temp`"
                            ") "
                            "VALUES ("
                            "'"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"', "
                            "'%1','%2',"
                            "'%3','%4',"
                            "'%5','%6',"
                            "'%7','%8',"
                            "'%9','%10',"
                            "'%11','%12',"
                            "'%13','%14',"
                            "'%15','%16',"
                            "'%17','%18',"
                            "'%19','%20',"
                            "'%21');")
                            .arg(etching1_temp_data_real).arg(etching1_humi_data_real)
                            .arg(light1_temp_data_real).arg(light1_humi_data_real)
                            .arg(deposition_temp_data_real).arg(deposition_humi_data_real)
                            .arg(krf_temp_data_real).arg(krf_humi_data_real)
                            .arg(probe1_temp_data_real).arg(probe1_humi_data_real)
                            .arg(light2_temp_data_real).arg(light2_humi_data_real)
                            .arg(light3_temp_data_real).arg(light3_humi_data_real)
                            .arg(liftoff_temp_data_real).arg(liftoff_humi_data_real)
                            .arg(etching2_temp_data_real).arg(etching2_humi_data_real)
                            .arg(probe2_temp_data_real).arg(probe2_humi_data_real)
                            .arg(drug_temp_data_real);
        query.exec(quert_txt);
//        qDebug()<<query.lastQuery();
//        qDebug()<<query.lastError();
        create_chart();
        if (spacout_check()){
            QDir savedir = QDir::home();
           savedir.cd("temp_humi_mail");
            QDateTime current_datetime = QDateTime::currentDateTime();
            if(!savedir.exists(current_datetime.toString("yyyy_HH_dd_hh_mm"))){
                  savedir.mkdir(current_datetime.toString("yyyy_HH_dd_hh_mm"));
            }
            savedir.cd(current_datetime.toString("yyyy_HH_dd_hh_mm"));
            QPixmap lightroom_temp_picture(ui->lightroom_temp->size());
            ui->lightroom_temp->render(&lightroom_temp_picture);
            lightroom_temp_picture.save(savedir.absolutePath()+"/lightroom_temp.png","PNG");
            QPixmap lightroom_humi_picture(ui->lightroom_humi->size());
            ui->lightroom_humi->render(&lightroom_humi_picture);
            lightroom_humi_picture.save(savedir.absolutePath()+"/lightroom_humi.png","PNG");
            QPixmap normalroom_temp_picture(ui->normalroom_temp->size());
            ui->normalroom_temp->render(&normalroom_temp_picture);
            normalroom_temp_picture.save(savedir.absolutePath()+"/normalroom_temp.png","PNG");
            QPixmap normalroom_humi_picture(ui->normalroom_humi->size());
            ui->normalroom_humi->render(&normalroom_humi_picture);
            normalroom_humi_picture.save(savedir.absolutePath()+"/normalroom_humi.png","PNG");
            send_email(current_datetime.toString("yyyy_HH_dd_hh_mm"));
        }

    }
    datalist.clear();

}



void temp_humi_server::send_message_timeout()
{
    QSqlQuery query_2(db);
    QSqlQuery query_3(db);
    query_2.exec("select * from room_temp_send_plan");
    if((QTime::currentTime().hour() == 0) &&(QTime::currentTime().minute() == 0)){
        query_3.exec("UPDATE `room_temp_send_plan` SET `checked`='0'");
    }
    bool check = false;
    while(query_2.next()){
         QTime plan_time = query_2.value("send_time").toTime();
         QTime current_time = QTime::currentTime();
         int checked = query_2.value("checked").toInt();
         if((current_time.hour() == plan_time.hour()) && (current_time.minute() == plan_time.minute()) && (checked == 0)){
             query_3.exec("UPDATE `room_temp_send_plan` SET `checked`='0'");
             QString query_txt = QString("UPDATE `room_temp_send_plan` SET `checked`='1' WHERE  `send_time`='%1'").arg(plan_time.toString("hh:mm:ss"));
             query_3.exec(query_txt);
             QDir savedir = QDir::home();
            savedir.cd("temp_humi_mail");
             QDateTime current_datetime = QDateTime::currentDateTime();
             if(!savedir.exists(current_datetime.toString("yyyy_HH_dd_hh_mm"))){
                   savedir.mkdir(current_datetime.toString("yyyy_HH_dd_hh_mm"));     
             }
             savedir.cd(current_datetime.toString("yyyy_HH_dd_hh_mm"));
             QPixmap lightroom_temp_picture(ui->lightroom_temp->size());
             ui->lightroom_temp->render(&lightroom_temp_picture);
             lightroom_temp_picture.save(savedir.absolutePath()+"/lightroom_temp.png","PNG");
             QPixmap lightroom_humi_picture(ui->lightroom_humi->size());
             ui->lightroom_humi->render(&lightroom_humi_picture);
             lightroom_humi_picture.save(savedir.absolutePath()+"/lightroom_humi.png","PNG");
             QPixmap normalroom_temp_picture(ui->normalroom_temp->size());
             ui->normalroom_temp->render(&normalroom_temp_picture);
             normalroom_temp_picture.save(savedir.absolutePath()+"/normalroom_temp.png","PNG");
             QPixmap normalroom_humi_picture(ui->normalroom_humi->size());
             ui->normalroom_humi->render(&normalroom_humi_picture);
             normalroom_humi_picture.save(savedir.absolutePath()+"/normalroom_humi.png","PNG");
             send_email(current_datetime.toString("yyyy_HH_dd_hh_mm"));
             check = true;
         }
    }
    if(!check){
        return ;
    }

}

void temp_humi_server::create_chart()
{
    QSqlQuery query(db);
    ui->lightroom_temp->getMainchart()->removeAxis(ui->lightroom_temp->getAxisX());
    ui->lightroom_temp->getMainchart()->removeAxis(ui->lightroom_temp->getAxisY());
    ui->lightroom_temp->getMainchart()->removeAllSeries();
    ui->lightroom_temp->getMainchart()->legend()->setFont(QFont("Sans Serif",11));

    ui->lightroom_humi->getMainchart()->removeAxis(ui->lightroom_humi->getAxisX());
    ui->lightroom_humi->getMainchart()->removeAxis(ui->lightroom_humi->getAxisY());
    ui->lightroom_humi->getMainchart()->removeAllSeries();
    ui->lightroom_humi->getMainchart()->legend()->setFont(QFont("Sans Serif",11));

    ui->normalroom_temp->getMainchart()->removeAxis(ui->normalroom_temp->getAxisX());
    ui->normalroom_temp->getMainchart()->removeAxis(ui->normalroom_temp->getAxisY());
    ui->normalroom_temp->getMainchart()->removeAllSeries();
    ui->normalroom_temp->getMainchart()->legend()->setFont(QFont("Sans Serif",11));

    ui->normalroom_humi->getMainchart()->removeAxis(ui->normalroom_humi->getAxisX());
    ui->normalroom_humi->getMainchart()->removeAxis(ui->normalroom_humi->getAxisY());
    ui->normalroom_humi->getMainchart()->removeAllSeries();
    ui->normalroom_humi->getMainchart()->legend()->setFont(QFont("Sans Serif",11));

    line_ASSML1_2_temp = new QLineSeries();
    line_ASSML1_2_temp->setPointsVisible(true);
    line_ASSML1_2_temp->setName("ASML1,2");
    line_track_temp = new QLineSeries();
    line_track_temp->setPointsVisible(true);
    line_track_temp->setName("track");
    line_krf_temp = new QLineSeries();
    line_krf_temp->setPointsVisible(true);
    line_krf_temp->setName("krf");

    line_ASSML1_2_humi = new QLineSeries();
    line_ASSML1_2_humi->setPointsVisible(true);
    line_ASSML1_2_humi->setName("ASML1,2");
    line_track_humi = new QLineSeries();
    line_track_humi->setPointsVisible(true);
    line_track_humi->setName("track");
    line_krf_humi = new QLineSeries();
    line_krf_humi->setPointsVisible(true);
    line_krf_humi->setName("krf");

    line_deposition_temp = new QLineSeries();
    line_deposition_temp->setPointsVisible(true);
    line_deposition_temp->setName(tr("deposition"));
    line_deposition_humi = new QLineSeries();
    line_deposition_humi->setPointsVisible(true);
    line_deposition_humi->setName(tr("deposition"));
    line_etching1_temp = new QLineSeries();
    line_etching1_temp->setPointsVisible(true);
    line_etching1_temp->setName(tr("etching1"));
    line_etching1_humi = new QLineSeries();
    line_etching1_humi->setPointsVisible(true);
    line_etching1_humi->setName(tr("etching1"));
    line_etching2_temp = new QLineSeries();
    line_etching2_temp->setPointsVisible(true);
    line_etching2_temp->setName(tr("etching2"));
    line_etching2_humi = new QLineSeries();
    line_etching2_humi->setPointsVisible(true);
    line_etching2_humi->setName(tr("etching2"));
    line_probe1_temp = new QLineSeries();
    line_probe1_temp->setPointsVisible(true);
    line_probe1_temp->setName(tr("probe1"));
    line_probe1_humi = new QLineSeries();
    line_probe1_humi->setPointsVisible(true);
    line_probe1_humi->setName(tr("probe1"));
    line_probe2_temp = new QLineSeries();
    line_probe2_temp->setPointsVisible(true);
    line_probe2_temp->setName(tr("probe2"));
    line_probe2_humi = new QLineSeries();
    line_probe2_humi->setPointsVisible(true);
    line_probe2_humi->setName(tr("probe2"));

    QDateTime search_start_time = QDateTime::currentDateTime().addDays(-15);
    search_start_time.setTime(QTime(0,0,0));
    QDateTime search_end_time = QDateTime::currentDateTime();

    query.exec("select * from room_temp_humi where check_time BETWEEN "
               "\'"+search_start_time.toString("yyyy-MM-dd hh:mm:ss")+"\' "
               "AND \'"+search_end_time.toString("yyyy-MM-dd hh:mm:ss")+"\' order by check_time asc");
    while(query.next()){
          qint64 history_time_value = query.value("check_time").toDateTime().toMSecsSinceEpoch();
          double asml1_2_temp_value = query.value("light1_temp").toDouble();
          double track_temp_value = query.value("light2_temp").toDouble();
          double krf_temp = query.value("light3_temp").toDouble();
          line_ASSML1_2_temp->append(history_time_value,asml1_2_temp_value);
          line_track_temp->append(history_time_value,track_temp_value);
          line_krf_temp->append(history_time_value,krf_temp);

          double asml1_2_humi_value = query.value("light1_humi").toDouble();
          double track_humi_value = query.value("light1_humi").toDouble();
          double krf_humi = query.value("light3_humi").toDouble();
          line_ASSML1_2_humi->append(history_time_value,asml1_2_humi_value);
          line_track_humi->append(history_time_value,track_humi_value);
          line_krf_humi->append(history_time_value,krf_humi);

          double deposition_temp_value = query.value("deposition_temp").toDouble();
          double etching1_temp_value = query.value("etching1_temp").toDouble();
          double etching2_temp_value = query.value("etching2_temp").toDouble();
          double probe1_temp_value = query.value("probe1_temp").toDouble();
          double probe2_temp_value = query.value("probe2_temp").toDouble();
          line_deposition_temp->append(history_time_value,deposition_temp_value);
          line_etching1_temp->append(history_time_value,etching2_temp_value);
          line_etching2_temp->append(history_time_value,etching2_temp_value);
          line_probe1_temp->append(history_time_value,probe1_temp_value);
          line_probe2_temp->append(history_time_value,probe2_temp_value);

          double deposition_humi_value = query.value("deposition_humi").toDouble();
          double etching1_humi_value = query.value("etching1_humi").toDouble();
          double etching2_humi_value = query.value("etching2_humi").toDouble();
          double probe1_humi_value = query.value("probe1_humi").toDouble();
          double probe2_humi_value = query.value("probe2_humi").toDouble();
          line_deposition_humi->append(history_time_value,deposition_humi_value);
          line_etching1_humi->append(history_time_value,etching1_humi_value);
          line_etching2_humi->append(history_time_value,etching2_humi_value);
          line_probe1_humi->append(history_time_value,probe1_humi_value);
          line_probe2_humi->append(history_time_value,probe2_humi_value);
    }

    QSqlQuery query_spec(db);
    query_spec.exec("select lightroomtemp_spec from room_temp_humi_spec where lightroomtemp_spec!=\"\" order by lightroomtemp_spec asc;");
    QVector<QLineSeries *> lightroomtemp_list;
    while(query_spec.next()){
        QLineSeries *temp_series = new QLineSeries();
        qint64 xdata_first = line_ASSML1_2_temp->points().first().x();
        qint64 xdata_last = line_ASSML1_2_temp->points().last().x();
        temp_series->append(xdata_first,query_spec.value("lightroomtemp_spec").toDouble());
        temp_series->append(xdata_last,query_spec.value("lightroomtemp_spec").toDouble());
        temp_series->setName("spec");
        QPen temppen;
        temppen.setWidth(3);
        temppen.setColor(QColor("Red"));
        temp_series->setPen(temppen);
        lightroomtemp_list.append(temp_series);
        ui->lightroom_temp->getMainchart()->addSeries(temp_series);
    }
    query_spec.exec("select lightroomhumi_spec from room_temp_humi_spec where lightroomhumi_spec!=\"\" order by lightroomhumi_spec asc;");
    QVector<QLineSeries *> lightroomhumi_list;
    while(query_spec.next()){
        QLineSeries *temp_series = new QLineSeries();
        qint64 xdata_first = line_ASSML1_2_temp->points().first().x();
        qint64 xdata_last = line_ASSML1_2_temp->points().last().x();
        temp_series->append(xdata_first,query_spec.value("lightroomhumi_spec").toDouble());
        temp_series->append(xdata_last,query_spec.value("lightroomhumi_spec").toDouble());
        temp_series->setName("spec");
        QPen temppen;
        temppen.setWidth(3);
        temppen.setColor(QColor("Red"));
        temp_series->setPen(temppen);
        lightroomhumi_list.append(temp_series);
        ui->lightroom_humi->getMainchart()->addSeries(temp_series);
    }
    query_spec.exec("select normalroomtemp_spec from room_temp_humi_spec where normalroomtemp_spec!=\"\" order by normalroomtemp_spec asc;");
    QVector<QLineSeries *> normalroomtemp_list;
    while(query_spec.next()){
        QLineSeries *temp_series = new QLineSeries();
        qint64 xdata_first = line_deposition_temp->points().first().x();
        qint64 xdata_last = line_deposition_temp->points().last().x();
        temp_series->append(xdata_first,query_spec.value("normalroomtemp_spec").toDouble());
        temp_series->append(xdata_last,query_spec.value("normalroomtemp_spec").toDouble());
        temp_series->setName("spec");
        QPen temppen;
        temppen.setWidth(3);
        temppen.setColor(QColor("Red"));
        temp_series->setPen(temppen);
        normalroomtemp_list.append(temp_series);
        ui->normalroom_temp->getMainchart()->addSeries(temp_series);
    }
    query_spec.exec("select normalroomhumi_spec from room_temp_humi_spec where normalroomhumi_spec!=\"\" order by normalroomhumi_spec asc;");
    QVector<QLineSeries *> normalroomhumi_list;
    while(query_spec.next()){
        QLineSeries *temp_series = new QLineSeries();
        qint64 xdata_first = line_deposition_temp->points().first().x();
        qint64 xdata_last = line_deposition_temp->points().last().x();
        temp_series->append(xdata_first,query_spec.value("normalroomhumi_spec").toDouble());
        temp_series->append(xdata_last,query_spec.value("normalroomhumi_spec").toDouble());
        temp_series->setName("spec");
        QPen temppen;
        temppen.setWidth(3);
        temppen.setColor(QColor("Red"));
        temp_series->setPen(temppen);
        normalroomhumi_list.append(temp_series);
        ui->normalroom_humi->getMainchart()->addSeries(temp_series);
    }

    ui->lightroom_temp->getMainchart()->addSeries(line_ASSML1_2_temp);
    ui->lightroom_temp->getMainchart()->addSeries(line_track_temp);
    ui->lightroom_temp->getMainchart()->addSeries(line_krf_temp);

    ui->lightroom_humi->getMainchart()->addSeries(line_ASSML1_2_humi);
    ui->lightroom_humi->getMainchart()->addSeries(line_track_humi);
    ui->lightroom_humi->getMainchart()->addSeries(line_krf_humi);

    ui->normalroom_temp->getMainchart()->addSeries(line_deposition_temp);
    ui->normalroom_temp->getMainchart()->addSeries(line_etching1_temp);
    ui->normalroom_temp->getMainchart()->addSeries(line_etching2_temp);
    ui->normalroom_temp->getMainchart()->addSeries(line_probe1_temp);
    ui->normalroom_temp->getMainchart()->addSeries(line_probe2_temp);

    ui->normalroom_humi->getMainchart()->addSeries(line_deposition_humi);
    ui->normalroom_humi->getMainchart()->addSeries(line_etching1_humi);
    ui->normalroom_humi->getMainchart()->addSeries(line_etching2_humi);
    ui->normalroom_humi->getMainchart()->addSeries(line_probe1_humi);
    ui->normalroom_humi->getMainchart()->addSeries(line_probe2_humi);

    QDateTimeAxis *axisX_lightroom_temp = new QDateTimeAxis();
    QDateTime startpoint_time;
    startpoint_time.setMSecsSinceEpoch((qint64)line_ASSML1_2_temp->points().first().x());
    QDateTime endpoint_time;
    endpoint_time.setMSecsSinceEpoch((qint64)line_ASSML1_2_temp->points().last().x());
    int diffday = startpoint_time.daysTo(endpoint_time);
    int tickcount = diffday+1;
    axisX_lightroom_temp->setTickCount(tickcount);
    axisX_lightroom_temp->setFormat("MM-dd");
    axisX_lightroom_temp->setTitleText(tr("Date"));
    axisX_lightroom_temp->setLabelsFont(QFont("Sans Serif",11));
    ui->lightroom_temp->setAxisX(axisX_lightroom_temp);
    ui->lightroom_temp->getMainchart()->addAxis(axisX_lightroom_temp, Qt::AlignBottom);
    for(int i=0;i<lightroomtemp_list.count();i++){
        lightroomtemp_list.at(i)->attachAxis(axisX_lightroom_temp);
    }
    line_ASSML1_2_temp->attachAxis(axisX_lightroom_temp);
    line_track_temp->attachAxis(axisX_lightroom_temp);
    line_krf_temp->attachAxis(axisX_lightroom_temp);


    QValueAxis *axisY_lightroom_temp = new QValueAxis();
    axisY_lightroom_temp->setTitleText(tr("temp"));
    double lightroom_temp_spec_low = lightroomtemp_list.first()->points().first().y();
    double lightroom_temp_spec_high = lightroomtemp_list.last()->points().first().y();
    axisY_lightroom_temp->setRange(lightroom_temp_spec_low-1,lightroom_temp_spec_high+1);
    axisY_lightroom_temp->setLabelsFont(QFont("Sans Serif",15));
    ui->lightroom_temp->setAxisY(axisY_lightroom_temp);
    ui->lightroom_temp->getMainchart()->addAxis(axisY_lightroom_temp, Qt::AlignLeft);
    for(int i=0;i<lightroomtemp_list.count();i++){
        lightroomtemp_list.at(i)->attachAxis(axisY_lightroom_temp);
    }
    line_ASSML1_2_temp->attachAxis(axisY_lightroom_temp);
    line_track_temp->attachAxis(axisY_lightroom_temp);
    line_krf_temp->attachAxis(axisY_lightroom_temp);


    QDateTimeAxis *axisX_lightroom_humi = new QDateTimeAxis();

    axisX_lightroom_humi->setTickCount(tickcount);
    axisX_lightroom_humi->setFormat("MM-dd");
    axisX_lightroom_humi->setTitleText(tr("Date"));
    axisX_lightroom_humi->setLabelsFont(QFont("Sans Serif",11));
    ui->lightroom_humi->setAxisX(axisX_lightroom_humi);
    ui->lightroom_humi->getMainchart()->addAxis(axisX_lightroom_humi, Qt::AlignBottom);
    for(int i=0;i<lightroomhumi_list.count();i++){
        lightroomhumi_list.at(i)->attachAxis(axisX_lightroom_humi);
    }
    line_ASSML1_2_humi->attachAxis(axisX_lightroom_humi);
    line_track_humi->attachAxis(axisX_lightroom_humi);
    line_krf_humi->attachAxis(axisX_lightroom_humi);

    QValueAxis *axisY_lightroom_humi = new QValueAxis();
    axisY_lightroom_humi->setTitleText(tr("humi"));
    double lightroom_humi_spec_low = lightroomhumi_list.first()->points().first().y();
    double lightroom_humi_spec_high = lightroomhumi_list.last()->points().first().y();
    axisY_lightroom_humi->setRange(lightroom_humi_spec_low-1,lightroom_humi_spec_high+1);
    axisY_lightroom_humi->setLabelsFont(QFont("Sans Serif",15));
    ui->lightroom_humi->setAxisY(axisY_lightroom_humi);
    ui->lightroom_humi->getMainchart()->addAxis(axisY_lightroom_humi, Qt::AlignLeft);
    for(int i=0;i<lightroomtemp_list.count();i++){
        lightroomhumi_list.at(i)->attachAxis(axisY_lightroom_humi);
    }
    line_ASSML1_2_humi->attachAxis(axisY_lightroom_humi);
    line_track_humi->attachAxis(axisY_lightroom_humi);
    line_krf_humi->attachAxis(axisY_lightroom_humi);

    QDateTimeAxis *axisX_normalroom_temp = new QDateTimeAxis();

    axisX_normalroom_temp->setTickCount(tickcount);
    axisX_normalroom_temp->setFormat("MM-dd");
    axisX_normalroom_temp->setTitleText(tr("Date"));
    axisX_normalroom_temp->setLabelsFont(QFont("Sans Serif",11));
    ui->normalroom_temp->setAxisX(axisX_normalroom_temp);
    ui->normalroom_temp->getMainchart()->addAxis(axisX_normalroom_temp, Qt::AlignBottom);
    for(int i=0;i<normalroomtemp_list.count();i++){
        normalroomtemp_list.at(i)->attachAxis(axisX_normalroom_temp);
    }
    line_deposition_temp->attachAxis(axisX_normalroom_temp);
    line_etching1_temp->attachAxis(axisX_normalroom_temp);
    line_etching2_temp->attachAxis(axisX_normalroom_temp);
    line_probe1_temp->attachAxis(axisX_normalroom_temp);
    line_probe2_temp->attachAxis(axisX_normalroom_temp);

    QValueAxis *axisY_normalroom_temp = new QValueAxis();
    axisY_normalroom_temp->setTitleText(tr("humi"));
    axisY_normalroom_temp->setTickCount(6);
    double normalroom_temp_spec_low = normalroomtemp_list.first()->points().first().y();
    double normalroom_temp_spec_high = normalroomtemp_list.last()->points().first().y();
    axisY_normalroom_temp->setRange(normalroom_temp_spec_low-1,normalroom_temp_spec_high+1);
    axisY_normalroom_temp->setLabelsFont(QFont("Sans Serif",15));
    ui->normalroom_temp->setAxisY(axisY_normalroom_temp);
    ui->normalroom_temp->getMainchart()->addAxis(axisY_normalroom_temp, Qt::AlignLeft);
    for(int i=0;i<normalroomtemp_list.count();i++){
        normalroomtemp_list.at(i)->attachAxis(axisY_normalroom_temp);
    }
    line_deposition_temp->attachAxis(axisY_normalroom_temp);
    line_etching1_temp->attachAxis(axisY_normalroom_temp);
    line_etching2_temp->attachAxis(axisY_normalroom_temp);
    line_probe1_temp->attachAxis(axisY_normalroom_temp);
    line_probe2_temp->attachAxis(axisY_normalroom_temp);

    QDateTimeAxis *axisX_normalroom_humi = new QDateTimeAxis();

    axisX_normalroom_humi->setTickCount(tickcount);
    axisX_normalroom_humi->setFormat("MM-dd");
    axisX_normalroom_humi->setTitleText(tr("Date"));
    axisX_normalroom_humi->setLabelsFont(QFont("Sans Serif",11));
    ui->normalroom_humi->setAxisX(axisX_normalroom_humi);
    ui->normalroom_humi->getMainchart()->addAxis(axisX_normalroom_humi, Qt::AlignBottom);
    for(int i=0;i<normalroomhumi_list.count();i++){
        normalroomhumi_list.at(i)->attachAxis(axisX_normalroom_humi);
    }
    line_deposition_humi->attachAxis(axisX_normalroom_humi);
    line_etching1_humi->attachAxis(axisX_normalroom_humi);
    line_etching2_humi->attachAxis(axisX_normalroom_humi);
    line_probe1_humi->attachAxis(axisX_normalroom_humi);
    line_probe2_humi->attachAxis(axisX_normalroom_humi);

    QValueAxis *axisY_normalroom_humi = new QValueAxis();
    axisY_normalroom_humi->setTitleText(tr("humi"));
    double normalroom_humi_spec_low = normalroomhumi_list.first()->points().first().y();
    double normalroom_humi_spec_high = normalroomhumi_list.last()->points().first().y();
    axisY_normalroom_humi->setRange(normalroom_humi_spec_low-1,normalroom_humi_spec_high+1);
    axisY_normalroom_humi->setLabelsFont(QFont("Sans Serif",15));
    ui->normalroom_humi->setAxisY(axisY_normalroom_humi);
    ui->normalroom_humi->getMainchart()->addAxis(axisY_normalroom_humi, Qt::AlignLeft);
    for(int i=0;i<normalroomhumi_list.count();i++){
        normalroomhumi_list.at(i)->attachAxis(axisY_normalroom_humi);
    }
    line_deposition_humi->attachAxis(axisY_normalroom_humi);
    line_etching1_humi->attachAxis(axisY_normalroom_humi);
    line_etching2_humi->attachAxis(axisY_normalroom_humi);
    line_probe1_humi->attachAxis(axisY_normalroom_humi);
    line_probe2_humi->attachAxis(axisY_normalroom_humi);

}

void temp_humi_server::closeEvent(QCloseEvent *event)
{
    this->deleteLater();

}

void temp_humi_server::send_email(QString file_path)
{
    QSqlQuery query(db);
    SmtpClient smtp("mx.info.wisol.co.kr", 25, SmtpClient::TcpConnection);

    MimeMessage message;

    message.setHeaderEncoding(MimePart::QuotedPrintable);

    EmailAddress sender("automail@wisol.co.kr", "automail");
    message.setSender(&sender);
    query.exec("select * from room_temp_send_emaillist");
    while(query.next()){
        EmailAddress *to = new EmailAddress(query.value("email").toString(), query.value("name").toString());
        message.addRecipient(to);
    }
    message.setSubject("FAB 자동 온습도 메일 입니다.");

    MimeHtml html;

    query.exec("select * from room_temp_humi_sendmail_content");
    query.next();

    QDateTime search_start_time = QDateTime::currentDateTime().addDays(-1);
    search_start_time.setTime(QTime(0,0,0));
    QDateTime search_end_time = QDateTime::currentDateTime();

    QSqlQuery query1(db);
    QString query1txt = QString("select * from room_temp_humi where check_time BETWEEN '%1' AND '%2';").arg(search_start_time.toString("yyyy-MM-dd hh:mm")).arg(search_end_time.toString("yyyy-MM-dd hh:mm"));
    query1.exec(query1txt);
    QSqlQuery query2(db);
    QString query2txt = QString("select * from room_temp_humi_spec");
    query2.exec(query2txt);
    QString lightroom_temp_comment ="";
    QString lightroom_humi_comment="";
    QString nomalroom_temp_comment = "";
    QString nomalroom_humi_comment = "";
    QString comment_temp_time;
    QString comment_temp_room;
    QString high_temp = tr("high temp");
    QString low_temp = tr("low temp");
    QString comment_humi_time;
    QString comment_humi_room;
    QString high_humi = tr("high humi");
    QString low_humi = tr("low humi");
    while(query1.next()){
        double light1_temp = query1.value("light1_temp").toDouble();
        query2.first();
        double lightroomtemp_high_spce = query2.value("lightroomtemp_spec").toDouble();
        query2.last();
        double lightroomtemp_low_spce = query2.value("lightroomtemp_spec").toDouble();
        comment_temp_time = query1.value("check_time").toDateTime().toString("MM/dd hh:mm");
        comment_temp_room = tr("ASML1,2");
        if(light1_temp>=lightroomtemp_high_spce){
            lightroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(light1_temp).arg(lightroomtemp_high_spce).arg(high_temp));
        }else if(light1_temp<=lightroomtemp_low_spce) {
            lightroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(light1_temp).arg(lightroomtemp_low_spce).arg(low_temp));
        }
        double light2_temp = query1.value("light2_temp").toDouble();
        comment_temp_room = tr("track");
        if(light2_temp>=lightroomtemp_high_spce){
            lightroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(light2_temp).arg(lightroomtemp_high_spce).arg(high_temp));
        }else if(light2_temp<=lightroomtemp_low_spce) {
            lightroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(light2_temp).arg(lightroomtemp_low_spce).arg(low_temp));
        }
        double light3_temp = query1.value("light3_temp").toDouble();
        comment_temp_room = tr("krf");
        if(light3_temp>=lightroomtemp_high_spce){
            lightroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(light3_temp).arg(lightroomtemp_high_spce).arg(high_temp));
        }else if(light3_temp<=lightroomtemp_low_spce) {
            lightroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(light3_temp).arg(lightroomtemp_low_spce).arg(low_temp));
        }

        double light1_humi = query1.value("light1_humi").toDouble();
        query2.first();
        double lightroomhumi_high_spce = query2.value("lightroomhumi_spec").toDouble();
        query2.last();
        double lightroomhumi_low_spce = query2.value("lightroomhumi_spec").toDouble();
        comment_humi_time = query1.value("check_time").toDateTime().toString("MM/dd hh:mm");

        comment_humi_room = tr("ASML1,2");
        if(light1_humi>=lightroomhumi_high_spce){
            lightroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(light1_humi).arg(lightroomhumi_high_spce).arg(high_humi));
        }else if(light1_humi<=lightroomhumi_low_spce) {
            lightroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(light1_humi).arg(lightroomhumi_low_spce).arg(low_humi));
        }
        double light2_humi = query1.value("light2_humi").toDouble();
        comment_humi_room = tr("track");
        if(light2_humi>=lightroomhumi_high_spce){
            lightroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(light2_humi).arg(lightroomhumi_high_spce).arg(high_humi));
        }else if(light2_humi<=lightroomhumi_low_spce) {
            lightroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(light2_humi).arg(lightroomhumi_low_spce).arg(low_humi));
        }
        double light3_humi = query1.value("light3_humi").toDouble();
        comment_humi_room = tr("krf");
        if(light3_humi>=lightroomhumi_high_spce){
            lightroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(light3_humi).arg(lightroomhumi_high_spce).arg(high_humi));
        }else if(light3_humi<=lightroomhumi_low_spce) {
            lightroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(light3_humi).arg(lightroomhumi_low_spce).arg(low_humi));
        }


        query2.first();
        double normaltemp_high_spce = query2.value("normalroomtemp_spec").toDouble();
        query2.last();
        double normaltemp_low_spce = query2.value("normalroomtemp_spec").toDouble();

        double deposition_temp = query1.value("deposition_temp").toDouble();

        comment_temp_room = tr("deposition");
        if(deposition_temp>=normaltemp_high_spce){
            nomalroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(deposition_temp).arg(normaltemp_high_spce).arg(high_temp));
        }else if(deposition_temp<=normaltemp_low_spce) {
            nomalroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(deposition_temp).arg(normaltemp_low_spce).arg(low_temp));
        }
        double etching1_temp = query1.value("etching1_temp").toDouble();

        comment_temp_room = tr("etching1");
        if(etching1_temp>=normaltemp_high_spce){
            nomalroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(etching1_temp).arg(normaltemp_high_spce).arg(high_temp));
        }else if(etching1_temp<=normaltemp_low_spce) {
            nomalroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(etching1_temp).arg(normaltemp_low_spce).arg(low_temp));
        }
        double etching2_temp = query1.value("etching2_temp").toDouble();

        comment_temp_room = tr("etching2");
        if(etching2_temp>=normaltemp_high_spce){
            nomalroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(etching2_temp).arg(normaltemp_high_spce).arg(high_temp));
        }else if(etching2_temp<=normaltemp_low_spce) {
            nomalroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(etching2_temp).arg(normaltemp_low_spce).arg(low_temp));
        }
        double probe1_temp = query1.value("probe1_temp").toDouble();

        comment_temp_room = tr("probe1");
        if(probe1_temp>=normaltemp_high_spce){
            nomalroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(probe1_temp).arg(normaltemp_high_spce).arg(high_temp));
        }else if(probe1_temp<=normaltemp_low_spce) {
            nomalroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(probe1_temp).arg(normaltemp_low_spce).arg(low_temp));
        }
        double probe2_temp = query1.value("probe2_temp").toDouble();
        comment_temp_room = tr("probe2");
        if(probe2_temp>=normaltemp_high_spce){
            nomalroom_temp_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(probe2_temp).arg(normaltemp_high_spce).arg(high_temp));
        }else if(probe2_temp<=normaltemp_low_spce) {
            nomalroom_temp_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_temp_time).arg(comment_temp_room).arg(probe2_temp).arg(normaltemp_low_spce).arg(low_temp));
        }

        query2.first();
        double normalroomhumi_high_spce = query2.value("normalroomhumi_spec").toDouble();
        query2.last();
        double normalroomhumi_low_spce = query2.value("normalroomhumi_spec").toDouble();
        comment_humi_time = query1.value("check_time").toDateTime().toString("MM/dd hh:mm");
        double deposition_humi = query1.value("deposition_humi").toDouble();
        comment_humi_room = tr("deposition");
        if(deposition_humi>=normalroomhumi_high_spce){
            nomalroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(deposition_humi).arg(normalroomhumi_high_spce).arg(high_humi));
        }else if(deposition_humi<=normalroomhumi_low_spce) {
            nomalroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(deposition_humi).arg(normalroomhumi_low_spce).arg(low_humi));
        }
        double etching1_humi = query1.value("etching1_humi").toDouble();
        comment_humi_room = tr("etching1");
        if(etching1_humi>=normalroomhumi_high_spce){
            nomalroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(etching1_humi).arg(normalroomhumi_high_spce).arg(high_humi));
        }else if(etching1_humi<=normalroomhumi_low_spce) {
            nomalroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(etching1_humi).arg(normalroomhumi_low_spce).arg(low_humi));
        }
        double etching2_humi = query1.value("etching2_humi").toDouble();
        comment_humi_room = tr("etching2");
        if(etching2_humi>=normalroomhumi_high_spce){
            nomalroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(etching2_humi).arg(normalroomhumi_high_spce).arg(high_humi));
        }else if(etching2_humi<=normalroomhumi_low_spce) {
            nomalroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(etching2_humi).arg(normalroomhumi_low_spce).arg(low_humi));
        }
        double probe1_humi = query1.value("probe1_humi").toDouble();
        comment_humi_room = tr("probe1");
        if(probe1_humi>=normalroomhumi_high_spce){
            nomalroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(probe1_humi).arg(normalroomhumi_high_spce).arg(high_humi));
        }else if(probe1_humi<=normalroomhumi_low_spce) {
            nomalroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(probe1_humi).arg(normalroomhumi_low_spce).arg(low_humi));
        }
        double probe2_humi = query1.value("probe2_humi").toDouble();
        comment_humi_room = tr("probe2");
        if(probe2_humi>=normalroomhumi_high_spce){
            nomalroom_humi_comment.append(QString("<p style=\"color:red;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(probe2_humi).arg(normalroomhumi_high_spce).arg(high_humi));
        }else if(probe2_humi<=normalroomhumi_low_spce) {
            nomalroom_humi_comment.append(QString("<p style=\"color:blue;\">[%1] %2 = %3 spac = %4  %5</p> <br>").arg(comment_humi_time).arg(comment_humi_room).arg(probe2_humi).arg(normalroomhumi_low_spce).arg(low_humi));
        }

    }


    QString htmldata = query.value("data").toString();
    htmldata = htmldata.replace("!!lighttemp_src",file_path+"/lightroom_temp.png");
    htmldata = htmldata.replace("!!lighthumi_src",file_path+"/lightroom_humi.png");
    htmldata = htmldata.replace("!!normaltemp_src",file_path+"/normalroom_temp.png");
    htmldata = htmldata.replace("!!normalhumi_src",file_path+"/normalroom_humi.png");
    htmldata = htmldata.replace("lightroom_temp_comment",lightroom_temp_comment);
    htmldata = htmldata.replace("lightroom_humi_comment",lightroom_humi_comment);
    htmldata = htmldata.replace("normalroom_temp_comment",nomalroom_temp_comment);
    htmldata = htmldata.replace("normalroom_humi_comment",nomalroom_humi_comment);


    html.setHtml(htmldata);

    message.addPart(&html);

    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
    }
    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;

    }
    smtp.quit();
}

void temp_humi_server::on_sendmail_btn_clicked()
{
    send_email(QDateTime::currentDateTime().toString("yyyy_HH_dd_hh_mm"));
}
