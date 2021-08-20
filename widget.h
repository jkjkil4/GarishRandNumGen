#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>
#include <QMenuBar>

#include <QCoreApplication>
#include <QSettings>
#include <QTimer>

#include "randomzierwidget.h"

#define APP_DIR QCoreApplication::applicationDirPath()

class Widget : public QWidget
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *) override;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;

    void add();

public slots:
    void onCountChanged(int value);
    void onClick();
    void onTimer();

private:
    QSpinBox *mSpinCount = new QSpinBox;
    QSpinBox *mSpinMin = new QSpinBox;
    QSpinBox *mSpinMax = new QSpinBox;
    QHBoxLayout *mLaySpinRange = new QHBoxLayout;
    QPushButton *mBtnDo = new QPushButton("抽取");
    QListWidget *mListWidget = new QListWidget;

    bool isDoing = false;
    QTimer *mTimer = new QTimer(this);

    int mMin, mMax;
};

