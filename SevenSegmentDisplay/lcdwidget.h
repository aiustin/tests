#ifndef LCDWIDGET_H
#define LCDWIDGET_H

#include <QWidget>

class SevenSegmentDigitWidget;

class LCDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LCDWidget(QWidget *parent = nullptr);

    void setTime(const QTime& time);

signals:

private:
    SevenSegmentDigitWidget* h0Widget_ = nullptr;
    SevenSegmentDigitWidget* h1Widget_ = nullptr;
    SevenSegmentDigitWidget* m0Widget_ = nullptr;
    SevenSegmentDigitWidget* m1Widget_ = nullptr;

};

#endif // LCDWIDGET_H
