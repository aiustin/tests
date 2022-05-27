#include "sevensegmentdigitwidget.h"

#include <QPainter>
#include <QPaintEvent>

struct Segment
{
    float x0 = 0;
    float y0 = 0;
    float x1 = 0;
    float y1 = 0;
};

static std::array<Segment, 7> LCDSegments {
    Segment{0.f, 0.f, 1.f, 0.f},
    Segment{1.f, 0.f, 1.f, .5f},
    Segment{1.f, .5f, 1.f, 1.f},
    Segment{1.f, 1.f, 0.f, 1.f},
    Segment{0.f, 1.f, 0.f, .5f},
    Segment{0.f, .5f, 0.f, .0f},
    Segment{0.f, .5f, 1.f, .5f}
};

static std::array<std::array<bool, 7>, 10> SegmentsForDigit {
    std::array<bool, 7>{ true   , true  , true  , true  , true  , true  , false }, // 0
    std::array<bool, 7>{ false  , true  , true  , false , false , false , false }, // 1
    std::array<bool, 7>{ true   , true  , false , true  , true  , false , true  }, // 2
    std::array<bool, 7>{ true   , true  , true  , true  , false , false , true  }, // 3
    std::array<bool, 7>{ false  , false , true  , false , false , true  , true  }, // 4
    std::array<bool, 7>{ true   , false , true  , true  , false , true  , true  }, // 5
    std::array<bool, 7>{ false  , false , true  , true  , true  , true  , true  }, // 6
    std::array<bool, 7>{ true   , true  , true  , false , false , false , false }, // 7
    std::array<bool, 7>{ true   , true  , true  , true  , true  , true  , true  }, // 8
    std::array<bool, 7>{ true   , true  , true  , false , false , true  , true  }  // 9
};


SevenSegmentDigitWidget::SevenSegmentDigitWidget(quint8 digit /*= 0*/, QWidget *parent) :
    QWidget(parent)
{
    setDigit(digit);
}

QSize SevenSegmentDigitWidget::sizeHint() const
{
    return QSize(10, 30);
}

void SevenSegmentDigitWidget::setDigit(quint8 d)
{
    Q_ASSERT(d < 10);
    digit_ = d > 10 ? 0 : d;
}

#include <QDebug>
void SevenSegmentDigitWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::black);

    const auto h = height() - 3;
    const auto w = width() - 3;

    const auto& segments = SegmentsForDigit[digit_];
    for(auto i = 0; i < segments.size(); ++i) {
        if (segments[i]) {
            const auto& lcd_segment = LCDSegments[i];
            int x0 = lcd_segment.x0 * w;
            int x1 = lcd_segment.x1 * w;
            int y0 = lcd_segment.y0 * h;
            int y1 = lcd_segment.y1 * h;
            if (x1 < x0) {
                std::swap(x0, x1);
            }
            if (y1 < y0) {
                std::swap(y0, y1);
            }
            if (x1 == x0) {
                painter.drawLine(x0, y0 + 2, x1, y1 - 2);
            }
            else {
                painter.drawLine(x0 + 2, y0, x1 - 2, y1);
            }
        }
    }
    QWidget::paintEvent(e);
}
