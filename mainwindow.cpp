#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sw(new QAnimationStackedWidget)
{
    ui->setupUi(this);

    /// 加载qss样式表
    QFile file(":/Style/QStyle.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    setCentralWidget(sw);
    sw->setEasingCurve(QEasingCurve::InOutBack);

    this->setMinimumSize(600, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sw;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Delete) {
        if(!labImages_.isEmpty() || !pixImages_.isEmpty()) {

            /// 移除预览图片
            labImages_.removeAt(sw->currentIndex());
            pixImages_.removeAt(sw->currentIndex());
            sw->removeWidget(sw->currentWidget());

            /// 移除最后一张图片时，设置背景颜色为黑色
            if (sw->count() == 0) {
                QPalette pal(sw->palette());
                pal.setColor(QPalette::Window, Qt::black);
                sw->setAutoFillBackground(true);
                sw->setPalette(pal);

                return ;
            }
        }

        return ;
    }

    QMainWindow::keyPressEvent(ev);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resize_image(sw->size());
}

///重新设置Label和Pixmap的缩放
void MainWindow::resize_image(const QSize &size)
{
    for (int i = 0; i < labImages_.size(); i++) {
        QPixmap p = pixImages_[i].scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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

///前一张图片
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

///后一张图片
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

///打开文件
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
        QPixmap p =pixmap.scaled(sw->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixImages_.push_back(pixmap);

        label->setPixmap(p);

        label->setScaledContents(true);

        label->resize(sw->size());

        sw->insertWidget(index++, label);
    }

    sw->setCurrentIndex(0);
}

