#pragma once

#include <dsplab/device.h>
#include <QObject>
#include <QStringList>

class QTextCodec;
class QTimer;

namespace DspLab {

const short SIZE_TEXT = 16;    // количество символов в одной ячейке
const short MAX_COUTER = 24;   // количество строк в таблице
const short TIMEOUT_TIMER = 300;   // частота обновления отображения в миллисекундах

class HexViewer : public Device
{
    Q_OBJECT

public:
    HexViewer();
    virtual ~HexViewer();

    //Device
    void processing() final;
    void start() final;
    void stop() final;

    //максимальный размер символов одной ячейки
    void setMaxText(int value);
    int getMaxText() const noexcept;

    //установка текущей кодировки данных
    void setCodecData(int index);
    QByteArray getCodec() const noexcept;

    void setHidden(bool state);
    bool getHidden() const noexcept;

private:
    //magic from string to hex
    QString process(QByteArray text);
    QString processCounter(int counter);
    QString processHex(QByteArray text);
    QByteArray processText(const char* data, int size);

private:
    Frame m_frame;

    QStringList m_listHex;
    QStringList m_listText;

    int m_lineLenght;
    int m_counter;
    std::vector<char> m_buffer;

    QTextCodec* m_codec;
    bool m_bHidden;

    QTimer* m_timer;

    std::atomic_bool m_bChangeList;

private slots:
    void onTimeout();

signals:
    void eventLine(QStringList, QStringList);
};

}   // namespace DspLab
