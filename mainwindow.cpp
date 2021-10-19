#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(screen()->geometry().center() - frameGeometry().center());
    Searched = false;
    ClearAsNew();
}

MainWindow::~MainWindow()
{delete ui;}

void MainWindow::ClearAsNew()
{
    ui->btnBack->setDisabled(true);
    ui->btnNext->setDisabled(true);
    LocalData = QJsonArray({});
    CanGoNext = false;
    CanGoBack = false;
    current = 0;
    total = 0;
    next = 0;
    back = 0;
    UpdateEveryThing();
}

bool MainWindow::Connection()
{
    QTcpSocket* sock = new QTcpSocket(this);
    sock->connectToHost("brandcrowd.com", 80);
    bool ok = sock->waitForConnected(30000);
    if (!ok){
        sock->abort();
        return false;
    }
    sock->close();
    return true;
}

void MainWindow::SetDisableForAll(bool status)
{
    // BUTTONS :
    ui->btnBack->setDisabled(status);
    ui->btnNext->setDisabled(status);
    ui->btnCreateImage->setDisabled(status);
    // INPUTS :
    ui->BusinessName->setDisabled(status);
    ui->KeyWord->setDisabled(status);
    ui->FontStyle->setDisabled(status);
    ui->LogoStyle->setDisabled(status);
    ui->Color->setDisabled(status);
}

void MainWindow::on_btnCreateImage_clicked()
{
    if(ui->BusinessName->text() == nullptr && ui->BusinessName->text().replace(" ", "") == ""){
        ui->BusinessName->setPlaceholderText("** required **");
        ui->BusinessName->focusWidget();
        return;
    }
    if(!Connection()){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("you're offline !, check your network connection and try again");
        msgBox.setWindowTitle("Brandcrowd - offline ?");
        msgBox.exec();
        return;
    }
    LocalData = QJsonArray({});
    ClearAsNew();
    SetDisableForAll(true);
//    QUrl url("http://localhost/API/brandcrowd/main.php");
    QUrl url("http://lil-nickel.epizy.com/brandcrowd/main.php?name=Lil-Nickel.&q=tech");
    QUrlQuery param;
    param.addQueryItem("name", ui->BusinessName->text());
    param.addQueryItem("ls", ui->LogoStyle->currentText());//.replace("All", "0").replace("Icon & Text", "1").replace("Text-only", "2")
    if(ui->KeyWord->text() != nullptr)
        param.addQueryItem("q", ui->KeyWord->text());
    if(ui->Color->currentText() != "None")
        param.addQueryItem("co", ui->Color->currentText());
    if(ui->FontStyle->currentText() != "None")
        param.addQueryItem("co", ui->FontStyle->currentText());
    url.setQuery(param);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:93.0) Gecko/20100101 Firefox/93.0");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Sec-GPC", "1");
    request.setRawHeader("DNT", "1");
    request.setRawHeader("Cookie", "__test=efcd78ab299db7419014e9aee14cbc5c");
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->get(request);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::Finsihed);
}

void MainWindow::Finsihed(QNetworkReply *reply)
{
    SetDisableForAll(false);
    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject JON = doc.object();
    if(JON.value("ok").toBool()){
        Searched = true;
        LocalData = JON.value("results").toArray();
        total = LocalData.size()-1;
        if(total > current)
            CanGoNext = true;
        CreatePreviewFromLink(LocalData[0].toString());
        UpdateEveryThing();
    }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Brandcrowd - error");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QString("Api Error : %1").arg(JON.value("msg").toString()));
    }
}

void MainWindow::PreviewIsHere(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    Image.loadFromData(response, "PNG");
    ui->Preview->setPixmap(Image);
    SetDisableForAll(false);
    UpdateEveryThing();
}

void MainWindow::CreatePreviewFromLink(QString url)
{
    QUrl Url(url);
    QNetworkRequest request(Url);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->get(request);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::PreviewIsHere);
}

void MainWindow::UpdateEveryThing()
{
    SetDisableForAll(true);
    if(current < total && current == 0){
        CanGoNext = true;
        CanGoBack = false;
    }else if(current == total && current != 0){
        CanGoNext = false;
        CanGoBack = true;
    }else if(current < total && current >= 1){
        CanGoBack = true;
        CanGoNext = true;
    }else{
        CanGoBack = false;
        CanGoNext = false;
    }
    if(CanGoBack) ui->btnBack->setDisabled(false);
    if(CanGoNext) ui->btnNext->setDisabled(false);
    ui->BusinessName->setDisabled(false);
    ui->BusinessName->setPlaceholderText("your name");
    ui->KeyWord->setDisabled(false);
    ui->Color->setDisabled(false);
    ui->FontStyle->setDisabled(false);
    ui->LogoStyle->setDisabled(false);
    ui->btnCreateImage->setDisabled(false);
}

void MainWindow::on_btnNext_clicked()
{
    SetDisableForAll(true);
    QString NextUrl;
    current += 1;
    NextUrl = LocalData[current].toString();
    CreatePreviewFromLink(NextUrl);
    UpdateEveryThing();
}

void MainWindow::on_btnBack_clicked()
{
    SetDisableForAll(true);
    QString Backurl;
    current -= 1;
    Backurl = LocalData[current].toString();
    CreatePreviewFromLink(Backurl);
    UpdateEveryThing();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionSave_triggered()
{
    if(Searched){
        QString SaveTo = QFileDialog::getSaveFileName(this, "Save Image As", QDir::homePath(), "Png (*.png)");
        if(SaveTo != NULL){
            QFile file(SaveTo);
            file.open(QIODevice::WriteOnly);
            Image.save(&file, "PNG");
        }
    }else{
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Brandcrowd - tip");
        msgBox.setText("You need to search first, then save what you searched for !");
        msgBox.exec();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    Ab.exec();
}
