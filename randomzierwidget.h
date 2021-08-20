#pragma once

#include <QWidget>

#include <QPainter>
#include <QRandomGenerator>

#define RG_VALUE_COUNT 5
#define RG_PER_WIDTH 65
#define RG_SPACING 10
#define RG_PER_OFFSET (RG_PER_WIDTH + RG_SPACING)
#define RG_WIDTH (RG_PER_OFFSET * RG_VALUE_COUNT + 2 * RG_SPACING)
#define RG_HEIGHT (RG_PER_WIDTH + 2 * RG_SPACING)

class RandomzierWidget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *) override;

public:
    explicit RandomzierWidget(QWidget *parent = nullptr);

    void random(int min, int max);
    void setHighlight(bool on) { highlight = on; update(); }

private:
    QRandomGenerator mRand = QRandomGenerator::securelySeeded();

    struct Value
    {
        int data;
        bool valid = false;
    } values[RG_VALUE_COUNT];
    QRect rects[RG_VALUE_COUNT];

    int index = 0;
    bool highlight = false;
    QColor downColor = QColor(Qt::green).lighter();
};

