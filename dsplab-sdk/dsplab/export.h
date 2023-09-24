#pragma once

#include <dsplab/i_plugin-factory.h>
#include <cstdint>

extern "C" {

/*!
 * \brief Разместить внутренние ресурсы библиотеки
 *
 * \details При первом вызове может выполняться некоторое время.
 * Повторный вызов просто вернет указатель.
 *
 * \return Указатель на фабрику плагинов
 */
DspLab::IPluginFactory* instance();

const char* name();

uint64_t guid();

const char* version();

}
