#include "timer.h"

#include <functional>
#include <assert.h>
#include <thread>
#include <chrono>
#include <vector>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
Timer::Timer(int period_ms, std::function<void()> fn)
{
    using namespace std::chrono;

    //запуск таймера
    m_started = true;
    m_thread = std::thread([this, period_ms, fn] {
        int64_t frame_mc = period_ms * 1000;
        int64_t frame_num = 0;
        auto t0 = high_resolution_clock::now();
        while (m_started) {
            auto t1 = high_resolution_clock::now();
            auto dt = duration_cast<microseconds>(t1 - t0);
            auto mc_count = dt.count() - frame_num * frame_mc;
            if (mc_count > 0) {
                fn();
                frame_num += 1;
            }
            std::this_thread::sleep_for(milliseconds(1));
        }
    });
}

//-------------------------------------------------------------------------------------------------
Timer::~Timer()
{
    assert(std::this_thread::get_id() != m_thread.get_id());
    m_started = false;
    m_thread.join();
}
