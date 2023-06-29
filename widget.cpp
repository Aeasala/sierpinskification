#include "widget.h"
#include "ui_widget.h"
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QCheckBox>
#include <cmath>
#include <QPainter>
#include <QVariant>
#include <QDebug>

void genPolys(int iterations, QPolygon firstPoly);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    iterations = 8; // max iterations should never go above 10
    uiSet(this);

    srand(static_cast<unsigned int>(time(nullptr)));

    initPoly << QPoint(384, 0) << QPoint(640, 512) << QPoint(128, 512); // setting root triangle values, equilateral with side length 512
    initPoly.translate(200,100);

    genPolys(initPoly, iterations);

}



Widget::~Widget()
{
    delete ui;
}

void Widget::sierpinskify()
{
    /*
     * this is an algorithm for drawing a sierpinski triangle based on a root triangle
     * based on one triangle, it draws three triangles inside of it, and then three more triangles per each of those triangles and so on
     * it takes points and midpoints of the previous triangle it is drawing from and does this for every single triangle for each iteration
    */


    for(int i = 0; i<iterations; i++)
    {
        qDebug() << "here2";
        for(int j = 0; j<static_cast<int>((pow(3, i))); j++)// j = how many triangles to draw inside of current triangle
        {

            // first triangle inside previous triangle
                polyToDraw[i+1][3*j].putPoints(0, 1, (polyToDraw[i][j].point(0)).x(), (polyToDraw[i][j].point(0)).y());
                // ^^ same as top point of previous triangle
                polyToDraw[i+1][3*j].putPoints(1, 1, ((polyToDraw[i][j].point(1).x() + (polyToDraw[i][j].point(0)).x())/2), ((polyToDraw[i][j].point(1)).y() + (polyToDraw[i][j].point(0)).y())/2);
                // ^^ midpoint between top and bottom right points of previous triangle
                polyToDraw[i+1][3*j].putPoints(2, 1, (((polyToDraw[i][j].point(0)).x() + (polyToDraw[i][j].point(2)).x())/2), (((polyToDraw[i][j].point(2)).y() + (polyToDraw[i][j].point(0)).y())/2));
                // ^^ midpoint between top and bottom left points of previous triangle

            //second triangle inside previous triangle
                polyToDraw[i+1][3*j+1].putPoints(1, 1, (polyToDraw[i][j].point(1)).x(), (polyToDraw[i][j].point(1)).y());
                // ^^ same as bottom right point of previous triangle
                polyToDraw[i+1][3*j+1].putPoints(0, 1, ((polyToDraw[i][j].point(1)).x() + (polyToDraw[i][j].point(0)).x())/2, ((polyToDraw[i][j].point(1)).y() + (polyToDraw[i][j].point(0)).y())/2);
                // ^^ midpoint between top and bottom right points of previous triangle
                polyToDraw[i+1][3*j+1].putPoints(2, 1,((polyToDraw[i][j].point(1)).x() + (polyToDraw[i][j].point(2)).x())/2, ((polyToDraw[i][j].point(1)).y() + (polyToDraw[i][j].point(2)).y())/2);
                // ^^ midpoint between bottom right and bottom left points of previous triangle

            // third triangle inside previous triangle
                polyToDraw[i+1][3*j+2].putPoints(2, 1, (polyToDraw[i][j].point(2)).x(), (polyToDraw[i][j].point(2).y()));
                // same as bottom left point of previous triangle
                polyToDraw[i+1][3*j+2].putPoints(1, 1, ((polyToDraw[i][j].point(1)).x() + (polyToDraw[i][j].point(2)).x())/2, ((polyToDraw[i][j].point(1)).y() + (polyToDraw[i][j].point(2)).y())/2);
                // midpoint between bottom right and bottom left points of previous triangle
                polyToDraw[i+1][3*j+2].putPoints(0, 1, (((polyToDraw[i][j].point(0)).x() + (polyToDraw[i][j].point(2)).x())/2), (((polyToDraw[i][j].point(2)).y() + (polyToDraw[i][j].point(0)).y())/2));
                // midpoint between top and bottom left points of pervious triangle
        }
    }

    return;
 }

void Widget::genPolys(QPolygon source, int iterations)
{
    // iterations: number of iterations to generate, with number of triangles per iteration being 3^iteration (with 0 iterations being one triangle, aka the source)

    polyToDraw = new QPolygon*[iterations]; // allocates for an <iterations> sized array of QPolygon* pointers
    *polyToDraw = new QPolygon; // allocating space for the root polygon (only one polygon)
    **polyToDraw = source; // setting content of polyToDraw[0][0] (root polygon) to the source polygon


    // first polygon has been set up, now allocating space for remaining polygons to be drawn based on the root polygon
    int size;

    for(int i = 1; i<=iterations; i++)
    {

        size = static_cast<int>(pow(3, i)); // number of polygons per iteration can be equated to n = 3^iteration (0th iteration being the root polygon)

        polyToDraw[i] = new QPolygon[size]; // allocate for n = 3^iteration number of QPolygons
    }

    sierpinskify(); // calling the sierpinskify function, which "sierpinskifies" the polyToDraw member variable

return;

}

void Widget::randRgb()
{
    // set random RGB value for sliders
    r_s->setValue(rand()%256);
    g_s->setValue(rand()%256);
    b_s->setValue(rand()%256);
    r_e->setValue(rand()%256);
    g_e->setValue(rand()%256);
    b_e->setValue(rand()%256);
    return;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // antialiasing
    painter.setPen((lineStyle->currentData()).value<QPen>()); // sets the pen style based on the combobox value
    QColor colorToDraw;
    colorToDraw.setRgb(r_s->value(), g_s->value(), b_s->value()); // sets starting rgb value

    int divisor;
        for(int i = 0; i<=itSlider->value(); i++)
        {
            divisor = itSlider->value();

            if (divisor == 0) // preventing divide by zero
                divisor = 1;

            if(!altGradient->isChecked())
            {
                // this is an algorithm to approach an ending rgb value from a starting rgb value per iteration
                // finds the difference between the slider values, and divides it by "divisor", which is the value of the iterations slider

                colorToDraw.setRed((i * (r_e->value() - r_s->value())/divisor) + r_s->value());
                colorToDraw.setGreen((i * (g_e->value() - g_s->value())/divisor) + g_s->value());
                colorToDraw.setBlue((i * (b_e->value() - b_s->value())/divisor) + b_s->value());

                painter.setBrush(colorToDraw); // change painter brush to new color

            }

            for(int j = 0; j<pow(3,i); j++)
            {
                // similar to previous color algorithm, but instead this changes the divisor based on (current triangle)/(total number of triangles in the iteration)
                if(altGradient->isChecked())
                {

                    divisor = static_cast<int>(pow(3,i));
                    colorToDraw.setRed((j * (r_e->value() - r_s->value())/divisor) + r_s->value());
                    colorToDraw.setGreen((j * (g_e->value() - g_s->value())/divisor) + g_s->value());
                    colorToDraw.setBlue((j * (b_e->value() - b_s->value())/divisor) + b_s->value());
                    painter.setBrush(colorToDraw);
                }


            painter.drawPolygon(polyToDraw[i][j]); // draws the current polygon (one by one throughout the loop)

            }
        }

}
void Widget::uiSet(QWidget *widg)
{

    QLabel *itLabel = new QLabel;

    itLabel->setText("Iterations to Draw");

    itSlider = new QSlider(Qt::Horizontal);
    itSlider->setRange(0,iterations);

    r_s = new QSlider(Qt::Horizontal);
    g_s = new QSlider(Qt::Horizontal);
    b_s = new QSlider(Qt::Horizontal);

    r_s->setRange(0,255);
    g_s->setRange(0,255);
    b_s->setRange(0,255);

    r_s->setValue(255);
    g_s->setValue(255);
    b_s->setValue(255);


    QLabel *rgbs = new QLabel;
    QLabel *rgbe = new QLabel;

    QLabel *gradLabel = new QLabel;
    altGradient = new QCheckBox;

    gradLabel->setText("Alternate Gradient");

    r_e = new QSlider(Qt::Horizontal);
    g_e = new QSlider(Qt::Horizontal);
    b_e = new QSlider(Qt::Horizontal);

    r_e->setRange(0,255);
    g_e->setRange(0,255);
    b_e->setRange(0,255);

    r_e->setValue(0);
    g_e->setValue(0);
    b_e->setValue(0);



    QLabel *lineLabel = new QLabel();
    lineStyle = new QComboBox();
    QPen line = Qt::SolidLine;
    QPen dash = Qt::DashLine;
    QPen none = Qt::NoPen;


    lineStyle->addItem("Solid", line);
    lineStyle->addItem("Dashed", dash);
    lineStyle->addItem("None", none);

    lineLabel->setText("Pen Style");

    rgbs->setText("Start RGB");
    rgbe->setText("End RGB");

    QVBoxLayout *layout = new QVBoxLayout;
       QGridLayout *gridboy = new QGridLayout;
       gridboy->addLayout(layout, 0, 0);

       layout->addWidget(itLabel);
       layout->addWidget(itSlider);
       layout->addStretch(1);
       layout->addWidget(rgbs);



       layout->addWidget(r_s);
       layout->addWidget(g_s);
       layout->addWidget(b_s);
       layout->addStretch(1);
       layout->addWidget(rgbe);
       layout->addWidget(r_e);
       layout->addWidget(g_e);
       layout->addWidget(b_e);
       layout->addStretch(1);
       randomize = new QPushButton("Randomize RGB");
       layout->addWidget(randomize);
       layout->addStretch(1);
       layout->addWidget(gradLabel);

       layout->addWidget(altGradient);
       layout->addStretch(1);

              layout->addWidget(lineLabel);
       layout->addWidget(lineStyle);
       layout->addStretch(8);
       gridboy->setColumnStretch(3, 1);


       widg->setLayout(gridboy);

       widg->resize(1000, 800);

       connect(lineStyle, SIGNAL(currentIndexChanged(int)), SLOT(repaint()));
       connect(altGradient, SIGNAL(stateChanged(int)), SLOT(repaint()));


       connect(itSlider, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(r_s, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(g_s, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(b_s, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(r_e, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(g_e, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(b_e, SIGNAL(valueChanged(int)), SLOT(repaint()));
       connect(randomize, SIGNAL(clicked(bool)), SLOT(randRgb(void)));


       return;

  }
