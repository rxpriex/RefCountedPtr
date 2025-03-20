#include "RxPointer.h"
#include <cstdlib>
#include <utility>

template <typename MemoryType>
void 
RxPointer<MemoryType>::init_data(MemoryType *data, int *shared_reference) {
  this->data = data;
  *shared_reference++;
  this->shared_reference = shared_reference;
}

template <typename MemoryType>
void
RxPointer<MemoryType>::resolve_data() {
  delete data;
}

template <typename MemoryType>
RxPointer<MemoryType>::RxPointer(MemoryType *data) {
  init_data(data, new int(0));
}

template <typename MemoryType>
template <typename... Args>
RxPointer<MemoryType>::RxPointer(Args &&...args) {
  init_data(new MemoryType(std::forward<Args>(args)...), new int(0));
}

template <typename MemoryType>
RxPointer<MemoryType>::RxPointer(RxPointer &other) {
  init_data(other.data, other.shared_reference);
}

template <typename MemoryType> RxPointer<MemoryType>::~RxPointer() {}

template <typename MemoryType> MemoryType *RxPointer<MemoryType>::get_data() {
  return data;
}