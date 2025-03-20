#include "RxPointer.h"
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <utility>

/**
 * @brief Initializes the RxPointer with a managed object and reference count.
 *
 * Sets the data pointer and associates it with a shared reference count,
 * incrementing the count atomically to reflect the new ownership.
 *
 * @tparam MemoryType The type of the managed object.
 * @param data Pointer to the object to manage.
 * @param shared_references Pointer to the atomic reference count.
 */
template <typename MemoryType>
void RxPointer<MemoryType>::init_data(MemoryType *data,
                                      std::atomic<int> *shared_references) {
  this->data = data;
  this->shared_references = shared_references;
  this->shared_references->fetch_add(1, std::memory_order_relaxed);
}

/**
 * @brief Releases the managed object and its reference count.
 *
 * Deletes the managed object and the reference count. This function is intended
 * for internal use when the reference count reaches zero.
 *
 * @tparam MemoryType The type of the managed object.
 */
template <typename MemoryType> void RxPointer<MemoryType>::release_data() {
  delete data;
  delete shared_references;
}

/**
 * @brief Retrieves the raw pointer to the managed object.
 *
 * Provides direct access to the underlying object, returning nullptr if no
 * object is managed.
 *
 * @tparam MemoryType The type of the managed object.
 * @return MemoryType* The raw pointer to the managed object.
 */
template <typename MemoryType> MemoryType *RxPointer<MemoryType>::get_data() {
  return data;
}

/**
 * @brief Constructs an RxPointer from a raw pointer.
 *
 * Takes ownership of the provided raw pointer and initializes the reference
 * count to 1 by creating a new atomic counter and incrementing it.
 *
 * @tparam MemoryType The type of the managed object.
 * @param data The raw pointer to manage.
 */
template <typename MemoryType>
RxPointer<MemoryType>::RxPointer(MemoryType *data) {
  init_data(data, new std::atomic<int>(0));
}

/**
 * @brief Constructs an RxPointer with variadic arguments.
 *
 * Creates a new object of MemoryType using the provided arguments and
 * initializes the reference count to 1 by creating a new atomic counter and
 * incrementing it.
 *
 * @tparam MemoryType The type of the managed object.
 * @tparam Args Variadic template for constructor arguments.
 * @param args Arguments forwarded to the MemoryType constructor.
 */
template <typename MemoryType>
template <typename... Args>
RxPointer<MemoryType>::RxPointer(Args &&...args) {
  init_data(new MemoryType(std::forward<Args>(args)...),
            new std::atomic<int>(0));
}

/**
 * @brief Copy constructor for sharing ownership.
 *
 * Shares ownership of the managed object and reference count with another
 * RxPointer, incrementing the reference count atomically.
 *
 * @tparam MemoryType The type of the managed object.
 * @param other The RxPointer to share ownership with.
 */
template <typename MemoryType>
RxPointer<MemoryType>::RxPointer(RxPointer &other) {
  init_data(other.data, other.shared_references);
}

/**
 * @brief Destructor that manages resource cleanup.
 *
 * Decrements the reference count atomically. If the count reaches zero after
 * decrementing, deletes the managed object and reference count.
 *
 * @tparam MemoryType The type of the managed object.
 */
template <typename MemoryType> RxPointer<MemoryType>::~RxPointer() {
  if (shared_references != nullptr) {
    // Atomically decrement and check if this was the last reference
    if (shared_references->fetch_sub(1, std::memory_order_acq_rel) == 1) {
      release_data();
    }
  }
}

/**
 * @brief Assignment operator for sharing ownership.
 *
 * Releases the currently managed object (if any) by decrementing its reference
 * count, then shares ownership with another RxPointer, incrementing the new
 * reference count.
 *
 * @tparam MemoryType The type of the managed object.
 * @param other The RxPointer to assign from.
 * @return RxPointer<MemoryType>& Reference to this RxPointer.
 */
template <typename MemoryType>
RxPointer<MemoryType> &
RxPointer<MemoryType>::operator=(RxPointer<MemoryType> &other) {
  if (this != &other) {
    // Release current resources
    if (shared_references != nullptr) {
      if (shared_references->fetch_sub(1, std::memory_order_acq_rel) == 1) {
        release_data();
      }
    }

    // Take on the new reference
    this->data = other.data;
    this->shared_references = other.shared_references;
    if (shared_references != nullptr) {
      shared_references->fetch_add(
          1, std::memory_order_relaxed); // Atomically increment
    }
  }
  return *this;
}