#include "qanimationstackedwidget.h"

#include <QPropertyAnimation>
#include <QPainter>

QAnimationStackedWidget::QAnimationStackedWidget(QWidget *parent)
    :QStackedWidget(parent)
{
    isAnimation_ = false;    // 出始状态
    duration_ = 500;         //设置默认的时间间隔

    //初始化animation框架、并连接信号和槽
    animation_ = new QPropertyAnimation(this, QByteArray());
    connect(animation_, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(animation_, SIGNAL(finished()), this, SLOT(animationFinished()));
}

void QAnimationStackedWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)

    if(isAnimation_) {

        QPainter paint(this);

        // 获取Widget
        QWidget *currentWidget = widget(currentIndex());
        QWidget *nextWidget    = widget(nextIndex_);

        // 将Widget绘制到Pixmap上面
        QPixmap currentPixmap(currentWidget->size());
        QPixmap nextPixmap(nextWidget->size());
        currentWidget->render(&currentPixmap);
        nextWidget->render(&nextPixmap);

        // 获取Widget的大小
        QRect currentRect = currentWidget->geometry();
        QRect nextRect    = nextWidget->geometry();

        nextWidget->resize(nextRect.width(), nextRect.height());

        double value = currentValue_.toDouble();

        // 绘制Widget
        QRectF currentR1;
        QRectF currentR2;
        QRectF nextR1;
        QRectF nextR2;
        if (slide == right) {

            // 绘制当前Widget
            currentR1.setRect(value, 0.0, currentRect.width() - value, currentRect.height());
            currentR2.setRect(value, 0.0, currentRect.width() - value, currentRect.height());
            paint.drawPixmap(currentR1, currentPixmap, currentR1);

            // 绘制下一个Widget
            nextR1.setRect(0.0, 0.0, value, nextRect.height());
            nextR2.setRect(nextRect.width() - value, 0.0, value, nextRect.height());
            paint.drawPixmap(nextR1, nextPixmap, nextR2);

        } else if (slide == left) {
            // 绘制当前Widget
            currentR1.setRect(0.0, 0.0, currentRect.width() - value, currentRect.height());
            currentR2.setRect(0.0, 0.0, currentRect.width() - value, currentRect.height());
            paint.drawPixmap(currentR1, currentPixmap, currentR1);

            // 绘制下一个Widget
            nextR1.setRect(nextRect.width() - value, 0.0, value, nextRect.height());
            nextR2.setRect(0.0, 0.0, value, nextRect.height());
            paint.drawPixmap(nextR1, nextPixmap, nextR2);
        }
    }
}

void QAnimationStackedWidget::setDuration(int time)
{
    duration_ = time;
}

void QAnimationStackedWidget::setEasingCurve(const QEasingCurve &easing)
{
    animation_->setEasingCurve(easing);
}

// 向左滑动
void QAnimationStackedWidget::slideLeft()
{
    slide = left;
    setAnimation();
}

// 向右滑动
void QAnimationStackedWidget::slideRight()
{
    slide = right;
    setAnimation();
}

void QAnimationStackedWidget::valueChanged_slot(QVariant value)
{
    currentValue_ = value;
    update();
}

void QAnimationStackedWidget::animationFinished()
{
    isAnimation_ = false;
    widget(currentIndex())->show();
    setCurrentIndex(nextIndex_);
}

void QAnimationStackedWidget::setAnimation()
{
    //如果正在动画，退出函数
    if( isAnimation_ )
        return;

    isAnimation_ = true;

    // 计算切换页面的索引
    widgetCount_ = count();
    int c = currentIndex();
    if (slide == right)
        nextIndex_ = (c - 1 + widgetCount_) % widgetCount_;
    else
        nextIndex_ = (c + 1) % widgetCount_;

    //隐藏当前的widget
    widget(c)->hide();

    // 开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    animation_->setStartValue(0);
    animation_->setEndValue(g.width());
    animation_->setDuration(duration_);
    animation_->start();
}

void QAnimationStackedWidget::paintPrevious(QPainter &p, int index)
{
    // 获得当前页面的Widget
    QWidget *w = widget(index);
    QPixmap pixmap(w->size());

    // 将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
    w->render(&pixmap);
    QRect r = w->geometry();

    // 绘制当前的Widget
    double value = currentValue_.toDouble();
    QRectF r1(value, 0.0, r.width() - value, r.height());
    QRectF r2(value, 0.0, r.width() - value, r.height());
    p.drawPixmap(r1, pixmap, r2);
}

void QAnimationStackedWidget::paintNext(QPainter &p, int index)
{
    QWidget *nextWidget = widget(index);
    QRect r = geometry();

    //这行代码不加会有bug，第一次切换的时候，QStackedWidget并没有为child分配大小
    nextWidget->resize(r.width(), r.height());

    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = currentValue_.toDouble();
    QRectF r1(0.0, 0.0, value, r.height());
    QRectF r2(r.width() - value, 0.0, value, r.height());
    p.drawPixmap(r1, nextPixmap, r2);
}
