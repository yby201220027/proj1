#include "document_previewer.h"
#include "ui_document_previewer.h"
#include<QFileDialog>
#include<QFile>
#include<QDesktopServices>
#include <QUrl>
#include "markdown.h"

document_previewer::document_previewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::document_previewer)
{
    ui->setupUi(this);
    connect(ui->pushButton1,&QPushButton::clicked,[=]()
    {
        QString path=QFileDialog::getOpenFileName(this,"打开文件","H:\\document_previewer\\build-document_previewer-Desktop_Qt_5_12_11_MinGW_32_bit-Debugtext\\test.txt");
        ui->lineEdit->setText(path);
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QByteArray array=file.readAll();
        ui->textEdit->setText(array);
        file.close();
        QFile file2("md.html");
        if (file2.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file2);
            out<<path;
            out << endl;
        }
        connect(ui->pushButton2,&QPushButton::clicked,[=]()
        {
            markdown Markdown(path);
            Markdown.operate();
            QDesktopServices :: openUrl(QUrl(QLatin1String("md.html")));
        });
    });
    connect(ui->pushButton3,&QPushButton::clicked,this,&QWidget::close);
}
document_previewer::~document_previewer()
{
    delete ui;
}

