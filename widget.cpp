#include "widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mSpinCount->setRange(1, 20);
    mSpinMin->setRange(-6000, 6000);
    mSpinMax->setRange(-6000, 6000);
    mListWidget->setSelectionMode(QListWidget::SelectionMode::NoSelection);
    mListWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    mListWidget->setItemAlignment(Qt::AlignCenter);
    mListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mListWidget->setMinimumWidth(RG_WIDTH + 20);
    add();

    QMenuBar *menuBar = new QMenuBar;

    QMenu *menu = new QMenu("关于");
    QAction *actAbout = new QAction("关于作者");
    connect(actAbout, &QAction::triggered, [this] {
        QMessageBox::about(this, "关于作者", "...");
    });
    QAction *actAboutQt = new QAction("关于Qt");
    connect(actAboutQt, &QAction::triggered, [this] {
        QMessageBox::aboutQt(this);
    });
    menu->addAction(actAbout);
    menu->addAction(actAboutQt);
    menuBar->addMenu(menu);

    connect(mSpinCount, SIGNAL(valueChanged(int)), this, SLOT(onCountChanged(int)));
    connect(mBtnDo, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));

    //创建布局
    mLaySpinRange->addWidget(new QLabel("范围:"));
    mLaySpinRange->addWidget(mSpinMin);
    mLaySpinRange->addWidget(new QLabel("~"));
    mLaySpinRange->addWidget(mSpinMax);
    mLaySpinRange->addStretch();

    QHBoxLayout *laySpinCount = new QHBoxLayout;
    laySpinCount->addWidget(new QLabel("抽取数量:"));
    laySpinCount->addWidget(mSpinCount);

    QHBoxLayout *layTop = new QHBoxLayout;
    layTop->addLayout(laySpinCount);
    layTop->addStretch();
    layTop->addWidget(mBtnDo);

    QVBoxLayout *layCentral = new QVBoxLayout;
    layCentral->addLayout(mLaySpinRange);
    layCentral->addLayout(layTop);
    layCentral->addWidget(mListWidget);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layCentral);

    QVBoxLayout *layMain = new QVBoxLayout;
    layMain->setMargin(0);
    layMain->addWidget(menuBar);
    layMain->addWidget(centralWidget);
    setLayout(layMain);

    adjustSize();
    resize(width(), 500);

    //读取配置文件
    QSettings config(APP_DIR + "/config.ini", QSettings::IniFormat);
    mSpinCount->setValue(config.value("config/count", 1).toInt());
    mSpinMin->setValue(config.value("config/min", 1).toInt());
    mSpinMax->setValue(config.value("config/max", 10).toInt());
}

Widget::~Widget() {}

void Widget::closeEvent(QCloseEvent *) {
    //写入配置文件
    QSettings config(APP_DIR + "/config.ini", QSettings::IniFormat);
    config.setValue("config/count", mSpinCount->value());
    config.setValue("config/min", mSpinMin->value());
    config.setValue("config/max", mSpinMax->value());
}

void Widget::add() {
    QListWidgetItem *item = new QListWidgetItem;
    RandomzierWidget *widget = new RandomzierWidget;
    item->setSizeHint(widget->size());
    mListWidget->addItem(item);
    mListWidget->setItemWidget(item, widget);

}

void Widget::onCountChanged(int value) {
    int count = mListWidget->count();
    for(int i = 0; i < count; i++) {
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *widget = mListWidget->itemWidget(item);
        mListWidget->removeItemWidget(item);
        delete widget;
    }
    mListWidget->clear();
    for(int i = 0; i < value; i++)
        add();
}

void Widget::onClick() {
    if(isDoing) {
        mTimer->stop();
        int count = mListWidget->count();
        for(int i = 0; i < count; i++)
            ((RandomzierWidget*)mListWidget->itemWidget(mListWidget->item(i)))->setHighlight(false);
        mBtnDo->setText("抽取");
    } else {
        mMin = mSpinMin->value();
        mMax = mSpinMax->value();
        if(mMin >= mMax) {
            QMessageBox::critical(this, "错误", "无效的随机范围");
            return;
        }

        mTimer->start(70);
        int count = mListWidget->count();
        for(int i = 0; i < count; i++)
            ((RandomzierWidget*)mListWidget->itemWidget(mListWidget->item(i)))->setHighlight(true);
        mBtnDo->setText("停止");
    }
    isDoing = !isDoing;
}

void Widget::onTimer() {
    int count = mListWidget->count();
    for(int i = 0; i < count; i++) {
        RandomzierWidget *widget = (RandomzierWidget*)mListWidget->itemWidget(mListWidget->item(i));
        widget->random(mMin, mMax);
    }
}


