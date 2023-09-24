#include "buffer-ctrl.h"
#include "buffer-dev.h"
#include "ui_main-window.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
BufferCtrl::BufferCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_unit = new BufferDev();
    this->setWindowTitle("Buffer");
    this->setDeviceModel(m_unit);

    connect(ui->speedBox, SIGNAL(valueChanged(int)), this, SLOT(onBufferSizeBoxChanged(int)));
}
//-------------------------------------------------------------------------------------------------
BufferCtrl::~BufferCtrl()
{
    delete m_unit;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QWidget* BufferCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
int BufferCtrl::valuesCount() const
{
    return 1;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> BufferCtrl::valuesList() const
{
    return {{"size", m_unit->getBufferSize(), DisplayValue::VALUE}};
}

//-------------------------------------------------------------------------------------------------
void BufferCtrl::onBufferSizeBoxChanged(int value)
{
    m_unit->setBufferSize(value);
}
