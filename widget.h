#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QPolygon initPoly;
    QPolygon** polyToDraw;
public slots:
    void randRgb();
private:
    void uiSet(QWidget*);

    QSlider *itSlider;
    QCheckBox *altGradient;
    QSlider *r_s;
    QSlider *g_s;
    QSlider *b_s;
    QSlider *r_e;
    QSlider *g_e;
    QSlider *b_e;

    QComboBox *lineStyle;

    QPushButton *randomize;
    Ui::Widget *ui;

    int iterations;

    void genPolys(QPolygon, int);
    void sierpinskify();



protected slots:
    void paintEvent(QPaintEvent *);
};

#endif // WIDGET_H
