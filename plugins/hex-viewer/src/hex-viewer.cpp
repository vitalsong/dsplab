#include "hex-viewer.h"

#include <QTextCodec>
#include <QTimer>
#include <QDebug>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
HexViewer::HexViewer()
  : Device(1, 0)
  , m_lineLenght(16)
  , m_counter(0)
  , m_bHidden(true)
  , m_bChangeList(true)
{
    Device::setAvaliableInputFormats({{Special, Raw}});

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

//-------------------------------------------------------------------------------------------------
HexViewer::~HexViewer()
{
    HexViewer::stop();
}

//-------------------------------------------------------------------------------------------------
void HexViewer::start()
{
    m_timer->start(TIMEOUT_TIMER);
    Device::start();
}

//-------------------------------------------------------------------------------------------------
void HexViewer::stop()
{
    m_counter = 0;
    m_timer->stop();
    Device::stop();
}

//-------------------------------------------------------------------------------------------------
void HexViewer::processing()
{
    if (!m_bHidden) {
        while (Device::ioSize()) {
            Device::ioRead(m_frame);
            auto pack = m_frame.getSpecial();
            const char* data = pack.data();
            int size = pack.size();
            QByteArray bytes;

            for (int i = 0; i < size; ++i) {
                m_buffer.push_back(data[i]);
                int size = m_buffer.size();
                if (size == m_lineLenght) {
                    bytes = processText(m_buffer.data(), size);
                    m_buffer.clear();
                    if (m_bChangeList) {
                        m_bChangeList = false;
                        m_listHex.push_back(process(bytes));
                        m_listText.push_back(m_codec->toUnicode(bytes));
                        m_bChangeList = true;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void HexViewer::onTimeout()
{
    QStringList hex, text;
    int size = m_listHex.size() < MAX_COUTER ? m_listHex.size() : MAX_COUTER;

    for (int i = 0; i < size; ++i) {
        hex.push_back(m_listHex.at(i));
        text.push_back(m_listText.at(i));
    }
    emit eventLine(hex, text);
    if (m_bChangeList) {
        m_bChangeList = false;
        m_listHex.clear();
        m_listText.clear();
        m_bChangeList = true;
    }
}

//-------------------------------------------------------------------------------------------------
QString HexViewer::process(QByteArray text)
{
    QString line = processCounter(m_counter);
    m_counter += m_lineLenght;
    line += ": " + processHex(text.left(m_lineLenght / sizeof(short))) + " |";
    line += " " + processHex(text.right(m_lineLenght / sizeof(short)));

    return line;
}

//-------------------------------------------------------------------------------------------------
QString HexViewer::processCounter(int counter)
{
    QString str = QString::number(counter, 16);
    while (str.size() < 10) {
        str.insert(0, "0");
    }

    return str.toLatin1();
}

//-------------------------------------------------------------------------------------------------
constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//-------------------------------------------------------------------------------------------------
static std::string stringToHex(unsigned char* data, int len)
{
    std::string output(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        output[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
        output[2 * i + 1] = hexmap[data[i] & 0x0F];
    }

    return output;
}

//-------------------------------------------------------------------------------------------------
QString HexViewer::processHex(QByteArray text)
{
    QString hex;
    unsigned char* symbol = (unsigned char*)text.data();
    std::string h = stringToHex(symbol, text.size());
    hex.push_back(QString::fromStdString(h));

    return hex.toLatin1();
}

//-------------------------------------------------------------------------------------------------
QByteArray HexViewer::processText(const char* data, int size)
{
    QByteArray text;
    for (int i = 0; i < size; ++i) {
        text.push_back(data[i]);
    }

    return text;
}

//-------------------------------------------------------------------------------------------------
void HexViewer::setMaxText(int value)
{
    m_lineLenght = value;
}

//-------------------------------------------------------------------------------------------------
int HexViewer::getMaxText() const noexcept
{
    return m_lineLenght;
}

//-------------------------------------------------------------------------------------------------
void HexViewer::setCodecData(int index)
{
    switch (index) {
    case 0:
        m_codec = QTextCodec::codecForName("Windows-1251");
        break;
    case 1:
        m_codec = QTextCodec::codecForName("UTF-8");
        break;
    case 2:
        m_codec = QTextCodec::codecForName("IBM 866");
        break;
    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------
QByteArray HexViewer::getCodec() const noexcept
{
    return m_codec->name();
}

//-------------------------------------------------------------------------------------------------
void HexViewer::setHidden(bool state)
{
    m_bHidden = state;
}

//-------------------------------------------------------------------------------------------------
bool DspLab::HexViewer::getHidden() const noexcept
{
    return m_bHidden;
}
