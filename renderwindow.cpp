#include "renderwindow.h"
#include "datafield/cdatafieldt.h"

RenderWindow::RenderWindow(render_widget *renderWid, QWidget *parent) :
    QWidget(parent), m_RenderWidget(renderWid)
{
    m_Slider = new QSlider(Qt::Orientation::Horizontal);
    m_MainLayout = new QVBoxLayout;
    m_MainLayout->addWidget(m_RenderWidget);
    m_MainLayout->addWidget(m_Slider);
    setLayout(m_MainLayout);

    //m_RenderWidget->grabKeyboard();


    connect(m_Slider, SIGNAL(valueChanged(int)), m_RenderWidget, SLOT(sliderValChanged(int)));
    connect(this, SIGNAL(keyForward(QKeyEvent*)), m_RenderWidget, SLOT(forwardedKeys(QKeyEvent*)));
    connect(m_RenderWidget, SIGNAL(setSliderMinMax(int,int)), this, SLOT(setSliderMinMax(int,int)));
}

void RenderWindow::setSliderRange(cFileData *fileData){
    cDataFieldT<double> *df_t =
            static_cast<cDataFieldT<double>*>(fileData->getDatafield("time"));
    if (!df_t)
        return;
    m_Slider->setMinimum(df_t->at(0));
    m_Slider->setValue(m_Slider->minimum());
    m_Slider->setMaximum(df_t->at(df_t->numEntries()-1));
}

void RenderWindow::setSliderMinMax(int min, int max){
    m_Slider->setMinimum(min);
    m_Slider->setMaximum(max);
    m_Slider->setValue(m_Slider->minimum());
}

void RenderWindow::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    switch (key){
    case Qt::Key_Right:
        m_Slider->setValue(m_Slider->value()+1);
        //QMetaObject::invokeMethod(m_RenderWidget, "sliderValChanged", Q_ARG(int, m_Slider->value()));
        break;
    case Qt::Key_Left:
        m_Slider->setValue(m_Slider->value()-1);
        //QMetaObject::invokeMethod(m_RenderWidget, "sliderValChanged", Q_ARG(int, m_Slider->value()));
        break;
    default:
       emit keyForward(event);
    }
}
