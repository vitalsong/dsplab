#pragma once

#include <string.h>

template<typename T>
class TFifoBuffer
{
public:
    TFifoBuffer();
    TFifoBuffer(int size);
    ~TFifoBuffer();

    int read(T* data, int max_size);
    void write(const T* data, int size);

    T* data() const;
    int size() const;
    int capacity() const;
    bool reserve(int size);
    void clear();
    void clear(int size);

private:
    T* m_buffer;
    int m_buffCapacity;
    int m_dataSize;
};

//------------------------------------------------------------------------------------
template<typename T>
TFifoBuffer<T>::TFifoBuffer()
  : m_buffCapacity(1)
  , m_dataSize(0)
{
    m_buffer = new T[m_buffCapacity];
}

//------------------------------------------------------------------------------------
template<typename T>
TFifoBuffer<T>::TFifoBuffer(int size)
  : m_buffCapacity(size)
  , m_dataSize(0)
{
    m_buffer = new T[m_buffCapacity];
}

//------------------------------------------------------------------------------------
template<typename T>
TFifoBuffer<T>::~TFifoBuffer()
{
    delete[] m_buffer;
}

//------------------------------------------------------------------------------------
template<typename T>
int TFifoBuffer<T>::read(T* data, int max_size)
{
    int size = std::min(max_size, m_dataSize);
    memcpy(data, m_buffer, size * sizeof(T));
    memmove(m_buffer, m_buffer + size, (m_dataSize - size) * sizeof(T));
    return size;
}

//------------------------------------------------------------------------------------
template<typename T>
void TFifoBuffer<T>::write(const T* data, int size)
{
    //проверка на недостаток буфера
    if (m_dataSize + size > m_buffCapacity) {
        reserve(m_dataSize + size);
    }

    //заполняем буфер
    memcpy(m_buffer + m_dataSize, data, size * sizeof(T));
    m_dataSize += size;
}

//------------------------------------------------------------------------------------
template<typename T>
T* TFifoBuffer<T>::data() const
{
    return m_buffer;
}

//------------------------------------------------------------------------------------
template<typename T>
int TFifoBuffer<T>::size() const
{
    return m_dataSize;
}

//------------------------------------------------------------------------------------
template<typename T>
int TFifoBuffer<T>::capacity() const
{
    return m_buffCapacity;
}

//------------------------------------------------------------------------------------
template<typename T>
bool TFifoBuffer<T>::reserve(int size)
{
    m_buffCapacity = size;

    T* temp = new T[m_buffCapacity];
    memcpy(temp, m_buffer, m_dataSize * sizeof(T));
    delete[] m_buffer;
    m_buffer = temp;

    return true;
}

//------------------------------------------------------------------------------------
template<typename T>
void TFifoBuffer<T>::clear()
{
    memset(m_buffer, 0, m_buffCapacity * sizeof(T));
    m_dataSize = 0;
}

//------------------------------------------------------------------------------------
template<typename T>
void TFifoBuffer<T>::clear(int size)
{
    if (size >= m_dataSize) {
        clear();
        return;
    }

    m_dataSize -= size;
    memmove(m_buffer, m_buffer + size, m_dataSize * sizeof(T));
}
