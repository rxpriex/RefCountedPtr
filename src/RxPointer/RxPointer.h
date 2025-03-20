#ifndef RXPOINTER_HEADER
#define RXPOINTER_HEADER

template<typename MemoryType> class RxPointer{
private:
    MemoryType* data;
    int* shared_reference;

    void init_data(MemoryType*, int*);

    void resolve_data();

public:
    template<typename ...Args> RxPointer(Args&&... args);
    RxPointer(RxPointer<MemoryType>&);
    RxPointer(MemoryType*);

    ~RxPointer();

    MemoryType* get_data();

};

#include "RxPointer.tpp"

#endif