#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QDialog *parent = nullptr);
    ~About();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::About *ui;
};

#endif // ABOUT_H
