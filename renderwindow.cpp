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

    m_RenderWidget->grabKeyboard();

    connect(m_Slider, SIGNAL(sliderMoved(int)), m_RenderWidget, SLOT(sliderValChanged(int)));

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
