#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QNetworkAccessManager>
#include <QScreen>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCreateImage_clicked();
    void Finsihed(QNetworkReply *reply);
    void PreviewIsHere(QNetworkReply *reply);
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_actionAbout_Qt_triggered();
    void on_actionSave_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    About Ab;
    QPixmap Image;
    bool Searched;
    void CreatePreviewFromLink(QString url);
    void UpdateEveryThing();
    void ClearAsNew();
    bool Connection();
    void SetDisableForAll(bool status);
    QJsonArray LocalData;
    bool CanGoBack, CanGoNext;
    int back, next, total, current;
};
#endif // MAINWINDOW_H
