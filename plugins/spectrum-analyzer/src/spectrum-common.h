#ifndef SPECTRUMCOMMON_H
#define SPECTRUMCOMMON_H

namespace Spectrum
{

/*!
 * \brief Тип оконной функции
 */
enum FftWindow
{
    Rectangle,      ///< треугольное окно
    Sinus,          ///< синус-окно
    Hann,           ///< окно Ханна
    Hamming,        ///< окно Хэмминга
    Blackman,       ///< окно Блэкмана
    Gauss           ///< окно Гаусса
};

/*!
 * \brief Режим накопления спектра
 */
enum AccumulationMode
{
    NotAccumulation,        ///< без накопления (результат на каждые N входных отсчетов)
    AccumulationMax,        ///< накопление максимума (результат на каждые N входных отсчетов)
    AccumulationMin,        ///< накопление минимума (результат на каждые N входных отсчетов)
    AccumulationMean        ///< накопление среднего (результат на каждые N*M входных отсчетов)
};

struct State
{
    int fft_size;
    FftWindow wind_mode;
    AccumulationMode accum_mode;
    int accum_size;
    bool add_noise;
};

}   ///< Spectrum

#endif // SPECTRUMCOMMON_H
