#include "Pool.h"

bool is_power_of_two(uintptr_t x)
{
    return ((x & (x - 1)) == 0);
}

Pool::Pool(const size_t& size)
{
    this->buffer = new char[size];
    this->buffer_size = size;
    this->offset = 0;
}

void* Pool::alloc(const size_t& size, const size_t& allignment)
{
    assert(is_power_of_two(allignment));

    uintptr_t currPtr = uintptr_t(this->buffer) + this->offset;

    uintptr_t a = (uintptr_t)allignment;
    uintptr_t modulo = currPtr & (a - 1);


    if (modulo != 0)
        currPtr += (a - modulo);


    if (this->offset + size <= this->buffer_size)
    {
        void* ptr = (this->buffer + this->offset);
        this->offset += size;
        //memset(ptr, 0, size);
       return ptr;
    }
    else
        return NULL;
}