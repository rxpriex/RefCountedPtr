#include "RefCountedPtr.h"
#include <atomic>
#include <utility>

/**
 * @brief Initializes the RefCountedPtr with a managed object and reference
 * count.
 *
 * Sets the data pointer and associates it with a shared reference count,
 * incrementing the count atomically to reflect the new ownership.
 *
 * @tparam T The type of the managed object.
 * @param data Pointer to the object to manage.
 * @param shared_references Pointer to the atomic reference count.
 */
template <typename T>
void RefCountedPtr<T>::init_data(T *data, std::atomic<int> *shared_references) {
  this->data = data;
  this->shared_references = shared_references;
  this->shared_references->fetch_add(1, std::memory_order_relaxed);
}

/**
 * @brief Releases the managed object and its reference count.
 *
 * Deletes the managed object and the reference count when no references remain.
 *
 * @tparam T The type of the managed object.
 */
template <typename T> void RefCountedPtr<T>::release_data() {
  delete data;
  delete shared_references;
}

/**
 * @brief Retrieves the raw pointer to the managed object.
 *
 * Provides direct access to the underlying object, returning nullptr if no
 * object is managed.
 *
 * @tparam T The type of the managed object.
 * @return T* The raw pointer to the managed object.
 */
template <typename T> T *RefCountedPtr<T>::get_data() { return data; }

/**
 * @brief Constructs a RefCountedPtr from a raw pointer.
 *
 * Takes ownership of the provided raw pointer and initializes the reference
 * count to 1 by creating a new atomic counter and incrementing it.
 *
 * @tparam T The type of the managed object.
 * @param data The raw pointer to manage.
 */
template <typename T> RefCountedPtr<T>::RefCountedPtr(T *data) {
  init_data(data, new std::atomic<int>(0));
}

/**
 * @brief Constructs a RefCountedPtr with variadic arguments.
 *
 * Creates a new object of T using the provided arguments and initializes the
 * reference count to 1 by creating a new atomic counter and incrementing it.
 *
 * @tparam T The type of the managed object.
 * @tparam Args Variadic template for constructor arguments.
 * @param args Arguments forwarded to the T constructor.
 */
template <typename T>
template <typename... Args>
RefCountedPtr<T>::RefCountedPtr(Args &&...args) {
  init_data(new T(std::forward<Args>(args)...), new std::atomic<int>(0));
}

/**
 * @brief Copy constructor for sharing ownership.
 *
 * Shares ownership of the managed object and reference count with another
 * RefCountedPtr, incrementing the reference count atomically.
 *
 * @tparam T The type of the managed object.
 * @param other The RefCountedPtr to share ownership with.
 */
template <typename T> RefCountedPtr<T>::RefCountedPtr(RefCountedPtr<T> &other) {
  init_data(other.data, other.shared_references);
}

/**
 * @brief Destructor that manages resource cleanup.
 *
 * Decrements the reference count atomically. If the count reaches zero after
 * decrementing, deletes the managed object and reference count.
 *
 * @tparam T The type of the managed object.
 */
template <typename T> RefCountedPtr<T>::~RefCountedPtr() {
  if (shared_references != nullptr) {
    if (shared_references->fetch_sub(1, std::memory_order_acq_rel) == 1) {
      release_data();
    }
  }
}

/**
 * @brief Assignment operator for sharing ownership.
 *
 * Releases the currently managed object (if any) by decrementing its reference
 * count, then shares ownership with another RefCountedPtr, incrementing the new
 * reference count.
 *
 * @tparam T The type of the managed object.
 * @param other The RefCountedPtr to assign from.
 * @return RefCountedPtr<T>& Reference to this RefCountedPtr.
 */
template <typename T>
RefCountedPtr<T> &RefCountedPtr<T>::operator=(RefCountedPtr<T> &other) {
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
      shared_references->fetch_add(1, std::memory_order_relaxed);
    }
  }
  return *this;
}