#include "about.h"
#include "ui_about.h"

About::About(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::About){
    ui->setupUi(this);
    //this->setWindowFlags(Qt::Dialog | Qt::SplashScreen);
}

About::~About()
{delete ui;}

void About::on_pushButton_clicked()
{QDesktopServices::openUrl(QUrl("https://github.com/Lil-Nickel"));}

void About::on_pushButton_2_clicked()
{QDesktopServices::openUrl(QUrl("https://t.me/Lil_Nickel"));}

void About::on_pushButton_3_clicked()
{QDesktopServices::openUrl(QUrl("https://discordapp.com/channels/@me/847046024480161792"));}

