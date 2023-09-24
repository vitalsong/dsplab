#include "hex-viewer-ctrl.h"
#include "hex-viewer.h"

#include "ui_hex-viewer.h"
#include <QFontDatabase>

using namespace DspLab;

//-------------------------------------------------------------------------
HexVewierCtrl::HexVewierCtrl()
  : ui(new Ui::IOHexVewierCtrl)
{
    ui->setupUi(this);

    m_device = new HexViewer();
    this->setWindowTitle("Hex Viewer");
    this->setDeviceModel(m_device);

    m_device->setMaxText(SIZE_TEXT);

    initForm();

    // событие об изменении кодировки
    connect(ui->codec, SIGNAL(currentIndexChanged(int)), this, SLOT(onCodecData(int)));

    // событие отображения hex строк на таблицу
    connect(m_device, SIGNAL(eventLine(QStringList, QStringList)), this,
            SLOT(onViewDataLines(QStringList, QStringList)));

    //реакция от устройства
    connect(m_device, SIGNAL(deviceStarted()), this, SLOT(onDeviceStarted()));
    connect(m_device, SIGNAL(deviceStopped()), this, SLOT(onDeviceStopped()));
}

//-------------------------------------------------------------------------
HexVewierCtrl::~HexVewierCtrl()
{
    delete m_device;
    delete ui;
}

//-------------------------------------------------------------------------
QWidget* HexVewierCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------
void HexVewierCtrl::showEvent(QShowEvent* e)
{
    QDialog::showEvent(e);
    m_device->setHidden(false);
}

//-------------------------------------------------------------------------
void HexVewierCtrl::hideEvent(QHideEvent* e)
{
    QDialog::hideEvent(e);
    m_device->setHidden(true);
}

//-------------------------------------------------------------------------
void HexVewierCtrl::initForm()
{
    //устанавливаем на вывод моноширный шрифт
    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->text->setFont(font);

    //устанавливаем кодировку в utf-8
    this->onCodecData(ui->codec->currentIndex());

    //создаем в таблице строчки для дальнейшего заполнения
    ui->text->setRowCount(MAX_COUTER);
    for (int i = 0; i < MAX_COUTER; ++i) {
        ui->text->setItem(i, 0, new QTableWidgetItem());
        ui->text->setItem(i, 1, new QTableWidgetItem());
    }
}

//-------------------------------------------------------------------------
void HexVewierCtrl::clearViewData()
{
    for (int i = 0; i < MAX_COUTER; ++i) {
        ui->text->item(i, 0)->setText("");
        ui->text->item(i, 1)->setText("");
    }
}

//-------------------------------------------------------------------------
void HexVewierCtrl::onCodecData(int index)
{
    m_device->setCodecData(index);
}

//-------------------------------------------------------------------------
void HexVewierCtrl::onDeviceStarted()
{
    this->clearViewData();
}

//-------------------------------------------------------------------------
void HexVewierCtrl::onDeviceStopped()
{
    this->clearViewData();
}

//-------------------------------------------------------------------------
void HexVewierCtrl::onViewDataLines(QStringList listHex, QStringList listText)
{
    int size = listText.size();
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < MAX_COUTER - 1; ++k) {
            ui->text->item(k, 0)->setText(ui->text->item(k + 1, 0)->text());
            ui->text->item(k, 1)->setText(ui->text->item(k + 1, 1)->text());
        }

        ui->text->item(MAX_COUTER - 1, 0)->setText(listHex.at(i));
        ui->text->item(MAX_COUTER - 1, 1)->setText(listText.at(i));
    }
}
