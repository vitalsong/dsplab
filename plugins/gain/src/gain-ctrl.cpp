#include "gain-ctrl.h"
#include "ui_main-window.h"
#include "gain.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
GainCtrl::GainCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_gain = new Gain();
    this->setWindowTitle("Gain");
    this->setDeviceModel(m_gain);

    connect(ui->gainDial, SIGNAL(valueChanged(int)), this, SLOT(onGainDialChanged(int)));
}

//-------------------------------------------------------------------------------------------------
GainCtrl::~GainCtrl()
{
    delete m_gain;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QWidget* GainCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
int GainCtrl::valuesCount() const
{
    return 1;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> GainCtrl::valuesList() const
{
    std::vector<DisplayValue> list;
    list.push_back({"Gain", m_gain->currentGain(), 2, DisplayValue::VALUE_UNIT, "dB"});
    return list;
}

//-------------------------------------------------------------------------------------------------
void GainCtrl::onGainDialChanged(int gain)
{
    //настройка усиления в устройстве
    m_gain->changeGain(gain);

    //проверка установленного в устройстве уровня
    gain = m_gain->currentGain();

    //изменение текста
    ui->gainLabel->setText(QString("Gain: %1 dB").arg(int(gain)));
}
