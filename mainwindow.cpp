#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sw(new QAnimationStackedWidget)
{
    ui->setupUi(this);

    setCentralWidget(sw);
    sw->setEasingCurve(QEasingCurve::InOutBack);

    this->setMinimumSize(600, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sw;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resize_image(sw->size());
}

void MainWindow::resize_image(const QSize &size)
{
    for (int i = 0; i < labImages_.size(); i++) {
        QPixmap p = pixImages_[i].scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        labImages_[i]->setPixmap(p);
        labImages_[i]->resize(size);
    }
}

void MainWindow::warning_info(const QString &info)
{
    QString dlgTitle = "温馨提示";
    QString text = info;
    int btn = QMessageBox::warning(this, dlgTitle, text, QMessageBox::Ok, QMessageBox::Cancel);
}

//前一张图片
void MainWindow::on_actPrevious_triggered()
{
    if (labImages_.isEmpty())
        return ;
    if (sw->currentIndex() == 0) {
        warning_info("这里是第一张哟！");
        return ;
    }

    sw->slideRight();
}

//后一张图片
void MainWindow::on_actNext_triggered()
{
    if (labImages_.isEmpty())
        return ;

    if (sw->currentIndex() == sw->count() - 1) {
        warning_info("这里是最后一张哟！");
        return ;
    }

    sw->slideLeft();
}

//打开文件
void MainWindow::on_actOpen_triggered()
{
    QString dlgTitle = "选择多个文件";
    QString filter = "图片 (*.jpg *.png);;所有文件 (*.*)";
    QString currPath = "D:/wallpaper";

    QStringList aFileNames = QFileDialog::getOpenFileNames(this, dlgTitle, currPath, filter);

    int index = 0;

    labImages_.clear();
    pixImages_.clear();

    foreach (QString str, aFileNames) {

        QLabel* label = new QLabel(sw);

        labImages_.push_back(label);

        QPixmap pixmap(str);
        QPixmap p =pixmap.scaled(sw->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixImages_.push_back(pixmap);

        label->setPixmap(p);

        label->setScaledContents(true);

        label->resize(sw->size());

        sw->insertWidget(index++, label);
    }

    sw->setCurrentIndex(0);
}

