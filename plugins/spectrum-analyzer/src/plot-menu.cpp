#include "plot-menu.h"

#include <QMenu>
#include <QEvent>
#include <QMouseEvent>

namespace Spectrum {

/*!
 * \brief Класс меню с автоматической настройкой checkable для QAction
 */
class QCheckableMenu : public QMenu
{
public:
    explicit QCheckableMenu(const QString &title, QWidget *parent = Q_NULLPTR) :
        QMenu(title, parent)
    {
        //nothing to do
    }

    template <typename Func1>
    inline QAction *addAction(const QString &text, Func1 slot, const QKeySequence &shortcut = 0)
    {
        Q_UNUSED(shortcut);
        QAction *result = QMenu::addAction(text);
        result->setCheckable(true);
        connect(result, &QAction::triggered, slot);
        return result;
    }
};

//-------------------------------------------------------------------------------------------------
PlotMenu::PlotMenu(QWidget *parent) : QObject(parent)
{
    //перехват событий родителя
    if (parent != Q_NULLPTR) {
        parent->installEventFilter(this);
    }

    m_state.accum_mode = NotAccumulation;
    m_state.accum_size = 0;
    m_state.add_noise = false;
    m_state.fft_size = 1024;
    m_state.wind_mode = Hann;
}

//-------------------------------------------------------------------------------------------------
bool PlotMenu::eventFilter(QObject* watched, QEvent *event)
{
    //если перехват не для родительского класса
    if (watched != this->parent()) {
        return QObject::eventFilter(watched, event);
    }

    //если событие - нажатие кнопки мыши
    if (event->type() == QEvent::MouseButtonPress)
    {
        //если событие - нажатие правой кнопки мыши
        QMouseEvent* mouseEvent = static_cast <QMouseEvent*> (event);
        if (mouseEvent->button() == Qt::RightButton)
        {
            //запуск меню в позиции курсора
            QPoint pos = mouseEvent->globalPos();
            _exec(pos);
        }
    }

    //возвращаем управление
    return QObject::eventFilter(watched, event);
}

//-------------------------------------------------------------------------------------------------
void PlotMenu::setState(State state)
{
    m_state = state;
}

//-------------------------------------------------------------------------------------------------
State PlotMenu::getState() const
{
    return m_state;
}

//-------------------------------------------------------------------------------------------------
void PlotMenu::_exec(QPoint pos)
{
    QMenu menu;

    QCheckableMenu fftSizeMenu("Размер выборки");
    fftSizeMenu.addAction("256", [&](){emit fftSizeChanged(256);})
            ->setChecked(m_state.fft_size == 256);
    fftSizeMenu.addAction("512", [&](){emit fftSizeChanged(512);})
            ->setChecked(m_state.fft_size == 512);
    fftSizeMenu.addAction("1024", [&](){emit fftSizeChanged(1024);})
            ->setChecked(m_state.fft_size == 1024);
    fftSizeMenu.addAction("2048", [&](){emit fftSizeChanged(2048);})
            ->setChecked(m_state.fft_size == 2048);
    fftSizeMenu.addAction("4096", [&](){emit fftSizeChanged(4096);})
            ->setChecked(m_state.fft_size == 4096);
    fftSizeMenu.addAction("8192", [&](){emit fftSizeChanged(8192);})
            ->setChecked(m_state.fft_size == 8192);
    menu.addMenu(&fftSizeMenu);

    QCheckableMenu fftWindMenu("Оконная функция");
    fftWindMenu.addAction("Rectangle", [&](){emit fftWindowChanged(Rectangle);})
            ->setChecked(m_state.wind_mode == Rectangle);
    fftWindMenu.addAction("Sinus", [&](){emit fftWindowChanged(Sinus);})
            ->setChecked(m_state.wind_mode == Sinus);
    fftWindMenu.addAction("Hann", [&](){emit fftWindowChanged(Hann);})
            ->setChecked(m_state.wind_mode == Hann);
    fftWindMenu.addAction("Hamming", [&](){emit fftWindowChanged(Hamming);})
            ->setChecked(m_state.wind_mode == Hamming);
    fftWindMenu.addAction("Blackman", [&](){emit fftWindowChanged(Blackman);})
            ->setChecked(m_state.wind_mode == Blackman);
    fftWindMenu.addAction("Gauss", [&](){emit fftWindowChanged(Gauss);})
            ->setChecked(m_state.wind_mode == Gauss);
    menu.addMenu(&fftWindMenu);

    QCheckableMenu accumModeMenu("Режим накопления");
    accumModeMenu.addAction("Без накопления", [&](){emit accumModeChanged(NotAccumulation);})
            ->setChecked(m_state.accum_mode == NotAccumulation);
    accumModeMenu.addAction("Минимум", [&](){emit accumModeChanged(AccumulationMin);})
            ->setChecked(m_state.accum_mode == AccumulationMin);
    accumModeMenu.addAction("Максимум", [&](){emit accumModeChanged(AccumulationMax);})
            ->setChecked(m_state.accum_mode == AccumulationMax);
    accumModeMenu.addAction("Среднее", [&](){emit accumModeChanged(AccumulationMean);})
            ->setChecked(m_state.accum_mode == AccumulationMean);
    menu.addMenu(&accumModeMenu);

    QCheckableMenu accumFactorMenu("Степень усреднения");
    accumFactorMenu.addAction("2", [&](){emit accumFactorChanged(2);})
            ->setChecked(m_state.accum_size == 2);
    accumFactorMenu.addAction("4", [&](){emit accumFactorChanged(4);})
            ->setChecked(m_state.accum_size == 4);
    accumFactorMenu.addAction("8", [&](){emit accumFactorChanged(8);})
            ->setChecked(m_state.accum_size == 8);
    accumFactorMenu.addAction("16", [&](){emit accumFactorChanged(16);})
            ->setChecked(m_state.accum_size == 16);
    accumFactorMenu.addAction("32", [&](){emit accumFactorChanged(32);})
            ->setChecked(m_state.accum_size == 32);
    accumFactorMenu.addAction("64", [&](){emit accumFactorChanged(64);})
            ->setChecked(m_state.accum_size == 64);
    menu.addMenu(&accumFactorMenu);

    QCheckableMenu addNoiseMenu("Визуальный шум");
    addNoiseMenu.addAction("Вкл", [&](){emit addNoiseEnableChanged(true);})
            ->setChecked(m_state.add_noise);
    addNoiseMenu.addAction("Выкл", [&](){emit addNoiseEnableChanged(false);})
            ->setChecked(!m_state.add_noise);
    menu.addMenu(&addNoiseMenu);

    //запуск меню
    menu.exec(pos);
}

//-------------------------------------------------------------------------------------------------
void PlotMenu::_addFftSizeAction(QMenu& menu, int size)
{
    QAction* action;
    action = menu.addAction(QString::number(size), [&](){emit fftSizeChanged(size);});
    action->setCheckable(true);
    action->setChecked(m_state.fft_size == size);
}

} ///< Spectrum
