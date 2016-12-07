#-------------------------------------------------
#
# Project created by QtCreator 2016-12-03T13:09:42
#
#-------------------------------------------------

QT       += core gui network sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mainserver
TEMPLATE = app

TRANSLATIONS += lang_ko.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    temp_humi_baseview.cpp \
    temp_humi_chart.cpp \
    temp_humi_chartview.cpp \
    temp_humi_server.cpp \
    smtp/emailaddress.cpp \
    smtp/mimeattachment.cpp \
    smtp/mimecontentformatter.cpp \
    smtp/mimefile.cpp \
    smtp/mimehtml.cpp \
    smtp/mimeinlinefile.cpp \
    smtp/mimemessage.cpp \
    smtp/mimemultipart.cpp \
    smtp/mimepart.cpp \
    smtp/mimetext.cpp \
    smtp/quotedprintable.cpp \
    smtp/smtpclient.cpp

HEADERS  += mainwindow.h \
    smtp/emailaddress.h \
    smtp/mimeattachment.h \
    smtp/mimecontentformatter.h \
    smtp/mimefile.h \
    smtp/mimehtml.h \
    smtp/mimeinlinefile.h \
    smtp/mimemessage.h \
    smtp/mimemultipart.h \
    smtp/mimepart.h \
    smtp/mimetext.h \
    smtp/quotedprintable.h \
    smtp/smtpclient.h \
    smtp/smtpexports.h \
    smtp/SmtpMime \
    temp_humi_baseview.h \
    temp_humi_chart.h \
    temp_humi_chartview.h \
    temp_humi_server.h

FORMS    += mainwindow.ui \
    temp_humi_baseview.ui \
    temp_humi_server.ui

RESOURCES += \
    resources.qrc



