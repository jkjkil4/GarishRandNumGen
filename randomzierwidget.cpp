#include "randomzierwidget.h"

RandomzierWidget::RandomzierWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(RG_WIDTH, RG_HEIGHT);
    setMaximumSize(RG_WIDTH, RG_HEIGHT);
    for(int i = 0; i < RG_VALUE_COUNT; i++)
        rects[i] = QRect(RG_SPACING + i * RG_PER_OFFSET, RG_SPACING, RG_PER_WIDTH, RG_PER_WIDTH);
}

void RandomzierWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);

    if(highlight)
        p.fillRect(rects[index].x(), rects[index].y() + rects[index].height() - 4, rects[index].width(), 4, downColor);

    for(int i = 0; i < RG_VALUE_COUNT; i++) {
        bool isCenter = i == RG_VALUE_COUNT / 2;
        p.fillRect(rects[i], isCenter ? QColor(Qt::yellow).lighter(180) : QColor(240, 240, 240));
        if(!values[i].valid)
            continue;
        p.setPen(QPen(isCenter ? Qt::black : Qt::gray, isCenter ? 22 : 18));
        p.drawText(rects[i], Qt::AlignCenter | Qt::AlignVCenter, QString::number(values[i].data));
    }
}

void RandomzierWidget::random(int min, int max) {
    for(int i = 0; i < RG_VALUE_COUNT - 1; i++) {
        const Value &next = values[i + 1];
        Value &cur = values[i];
        if(next.valid) {
            cur.data = next.data;
            cur.valid = true;
        }
    }
    int range = max - min;
    Value &last = values[RG_VALUE_COUNT - 1];
    last.data = min + (int)(mRand.generate() % (uint)(range + 1));
    last.valid = true;
    index--;
    if(index < 0)
        index = RG_VALUE_COUNT - 1;
    update();
}
