#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>

#include "DisplayImageView/qanimationstackedwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *ev) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void resize_image(const QSize &size);
    void warning_info(const QString& info);

private slots:
    void on_actPrevious_triggered();

    void on_actNext_triggered();

    void on_actOpen_triggered();

private:
    Ui::MainWindow *ui;
    QAnimationStackedWidget* sw;
    QVector<QLabel*> labImages_;
    QVector<QPixmap> pixImages_;
};
#endif // MAINWINDOW_H
