#include "device-facade.h"

#include <base/port-adapter.h>
#include <dsplab/i_controller.h>
#include <scene/i_graphics-device.h>

#include <QWidget>
#include <QTimer>

namespace {
const int UPDATE_STATE_TIME = 300;
const int STANDBY_TIME = 500;
}   // namespace

static quint32 g_id = 42;

//-------------------------------------------------------------------------------------------------
DeviceFacade::DeviceFacade(std::shared_ptr<DspLab::IController> dev, std::shared_ptr<IGraphicsDevice> view,
                           QString name)
{
    m_id = ++g_id;
    m_dev = dev;
    m_view = view;
    m_name = name;

    //настройка таймера для обновления отображения переменных
    m_timer = new QTimer(this);
    m_timer->start(UPDATE_STATE_TIME);
    connect(m_timer, &QTimer::timeout, this, &DeviceFacade::updateState);

    this->configurateView();

    //warn: call after configurateView
    this->configurateIO();
}

//-------------------------------------------------------------------------------------------------
DeviceFacade::~DeviceFacade()
{
    m_timer->stop();
    m_dev.reset();
    m_view.reset();
}

//-------------------------------------------------------------------------------------------------
IGraphicsDevice* DeviceFacade::sceneView() const
{
    return m_view.get();
}

//-------------------------------------------------------------------------------------------------
DspLab::IController* DeviceFacade::controller() const
{
    return m_dev.get();
}

//-------------------------------------------------------------------------------------------------
QString DeviceFacade::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
quint32 DeviceFacade::id() const
{
    return m_id;
}

//-------------------------------------------------------------------------------------------------
int DeviceFacade::nin() const
{
    return m_in.size();
}

//-------------------------------------------------------------------------------------------------
int DeviceFacade::nout() const
{
    return m_out.size();
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::configurateIO()
{
    int nin = m_dev->device()->num_inputs();
    int nout = m_dev->device()->num_outputs();

    for (int i = 0; i < nin; ++i) {
        auto devPort = m_dev->device()->input(i);
        auto viewPort = m_view->input(i);
        m_in.push_back(PortAdapterPtr(new PortAdapter(this, devPort, viewPort, i)));
    }

    for (int i = 0; i < nout; ++i) {
        auto devPort = m_dev->device()->output(i);
        auto viewPort = m_view->output(i);
        m_out.push_back(PortAdapterPtr(new PortAdapter(this, devPort, viewPort, i)));
    }
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::configurateView()
{
    int nin = m_dev->device()->num_inputs();
    int nout = m_dev->device()->num_outputs();

    //device name
    m_view->setNameText(m_name);

    //device status
    this->updateStatus();

    //input/output count
    m_view->setNumInputs(nin);
    m_view->setNumOutputs(nout);
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::updateStatus()
{
    if (m_dev->device()->started()) {
        m_view->setStartedStatus(IGraphicsDevice::Started);
    } else {
        m_view->setStartedStatus(IGraphicsDevice::Stopped);
    }
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::updateState()
{
    auto values = m_dev->valuesList();
    QStringList string_list;
    for (const DspLab::DisplayValue& it : values) {
        string_list.push_back(it.label());
    }

    //update view variables
    if (!string_list.empty()) {
        m_view->updateValuesList(string_list);
    }

    //update device states
    auto device = m_dev->device();
    if (device->started()) {
        auto delta = device->standby();
        if (delta > STANDBY_TIME) {
            m_view->setStartedStatus(IGraphicsDevice::Waiting);
        } else {
            m_view->setStartedStatus(IGraphicsDevice::Started);
        }
    } else {
        m_view->setStartedStatus(IGraphicsDevice::Stopped);
    }
}

//-------------------------------------------------------------------------------------------------
PortAdapter* DeviceFacade::input(int num) const
{
    return m_in[num].get();
}

//-------------------------------------------------------------------------------------------------
PortAdapter* DeviceFacade::output(int num) const
{
    return m_out[num].get();
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::start()
{
    m_dev->device()->start();
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::stop()
{
    m_dev->device()->stop();
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::show()
{
    if (m_dev->window() != nullptr) {
        //TODO: restore last pos
        m_dev->window()->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
        m_dev->window()->show();
    }
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::hide()
{
    if (m_dev->window() != nullptr) {
        //TODO: save current pos
        m_dev->window()->hide();
    }
}

//-------------------------------------------------------------------------------------------------
QVariantMap DeviceFacade::paramList() const
{
    return m_dev->paramList();
}

//-------------------------------------------------------------------------------------------------
void DeviceFacade::setParam(QVariantMap param)
{
    m_dev->setParam(param);
}
