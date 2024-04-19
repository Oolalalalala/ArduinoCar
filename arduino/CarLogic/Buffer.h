#include <stdint.h>

// A non-owning buffer
struct Buffer
{
    void* Data = nullptr;
    uint32_t Size = 0;

    Buffer() = default;
    Buffer(const Buffer&) = default;

    Buffer(uint32_t size)
    {
        Allocate(size);
    }

    void Allocate(uint32_t size)
    {
        Size = size;
        Data = new uint8_t[size];
    }

    void Release()
    {
        delete[] Data;
        Data = nullptr;
        Size = 0;
    }

    template<typename T>
    T* As()
    {
        return (T*)Data;
    }

};