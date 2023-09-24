#include <base/port-adapter.h>

#include <dsplab/i_port-input.h>
#include <dsplab/i_port-output.h>

#include <scene/i_graphics-port.h>

//-------------------------------------------------------------------------------------------------
PortAdapter::PortAdapter(DeviceFacade* parent, DspLab::IPortInput* port, IGraphicsPort* view, int num)
{
    m_parent = parent;
    m_type = Input;
    m_input = port;
    m_output = nullptr;
    m_num = num;
    m_view = view;

    this->configurateFormatsView();
}

//-------------------------------------------------------------------------------------------------
PortAdapter::PortAdapter(DeviceFacade* parent, DspLab::IPortOutput* port, IGraphicsPort* view, int num)
{
    m_parent = parent;
    m_type = Output;
    m_input = nullptr;
    m_output = port;
    m_num = num;
    m_view = view;

    this->configurateFormatsView();
}

//-------------------------------------------------------------------------------------------------
PortAdapter::~PortAdapter()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
DeviceFacade* PortAdapter::parent() const
{
    return m_parent;
}

//-------------------------------------------------------------------------------------------------
PortAdapter::Type PortAdapter::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
int PortAdapter::num() const
{
    return m_num;
}

//-------------------------------------------------------------------------------------------------
bool PortAdapter::attach(PortAdapter* ioport)
{
    //проверка возможности соединения
    if ((m_type == Input) && (ioport->type() == Input)) {
        return false;
    }

    if ((m_type == Output) && (ioport->type() == Output)) {
        return false;
    }

    //коммутация
    if (m_type == Input) {
        if (m_input->attach(ioport->m_output)) {
            ioport->m_connections.insert(this);
            m_connections.insert(ioport);
            m_view->attach(ioport->m_view);
            return true;
        }
    } else {
        if (m_output->attach(ioport->m_input)) {
            ioport->m_connections.insert(this);
            m_connections.insert(ioport);
            m_view->attach(ioport->m_view);
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
bool PortAdapter::detach(PortAdapter* ioport)
{
    //проверка возможности разрыва соединения
    if ((m_type == Input) && (ioport->type() == Input)) {
        return false;
    }

    if ((m_type == Output) && (ioport->type() == Output)) {
        return false;
    }

    //разрыв
    if (m_type == Input) {
        if (m_input->detach(ioport->m_output)) {
            ioport->m_connections.erase(this);
            m_connections.erase(ioport);
            m_view->detach(ioport->m_view);
            return true;
        }
    } else {
        if (m_output->detach(ioport->m_input)) {
            ioport->m_connections.erase(this);
            m_connections.erase(ioport);
            m_view->detach(ioport->m_view);
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
void PortAdapter::detachAll()
{
    if (m_type == Input) {
        m_input->detachAll();
        //m_view->detachAll();
    } else {
        m_output->detachAll();
        //m_view->detachAll();
    }
}

//-------------------------------------------------------------------------------------------------
std::set<PortAdapter*> PortAdapter::connections() const
{
    return m_connections;
}

//-------------------------------------------------------------------------------------------------
void PortAdapter::configurateFormatsView()
{
    std::vector<DspLab::Format> formats;

    if (m_input) {
        formats = m_input->avaliableFormats();
    }

    if (m_output) {
        formats = m_output->avaliableFormats();
    }

    int flags = 0;
    for (const DspLab::Format& f : formats) {
        if (f.base() == DspLab::Real) {
            flags |= IGraphicsPort::Real;
        }

        if (f.base() == DspLab::Complex) {
            flags |= IGraphicsPort::Complex;
        }

        if (f.base() == DspLab::Special) {
            flags |= IGraphicsPort::Binary;
        }
    }

    m_view->setAvailableFormats(flags);
}
