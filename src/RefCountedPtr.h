#ifndef REFCOUNTEDPTR_HEADER
#define REFCOUNTEDPTR_HEADER

#include <atomic>

/**
 * @brief A custom shared pointer class for managing shared ownership of
 * objects.
 *
 * RefCountedPtr enables multiple pointers to share ownership of a dynamically
 * allocated object, automatically deleting it when the last reference is
 * destroyed.
 *
 * @tparam T The type of the object being managed.
 */
template <typename T> class RefCountedPtr {
private:
  T *data; ///< Pointer to the managed object.
  std::atomic<int>
      *shared_references; ///< Pointer to the shared reference count.

  /**
   * @brief Initializes the pointer with the given data and reference count.
   *
   * Configures the data pointer and links it to a shared reference count,
   * incrementing the count to track ownership.
   *
   * @param data Pointer to the object to manage.
   * @param shared_references Pointer to the atomic reference count.
   */
  void init_data(T *, std::atomic<int> *);

  /**
   * @brief Releases the managed object and its reference count.
   *
   * Deletes the managed object and reference count when no references remain.
   */
  void release_data();

public:
  /**
   * @brief Default constructor creating an empty RefCountedPtr.
   *
   * Initializes data and shared_references to nullptr, representing no
   * ownership.
   */
  RefCountedPtr() : data(nullptr), shared_references(nullptr) {}

  /**
   * @brief Constructs a RefCountedPtr from a raw pointer.
   *
   * Assumes ownership of the raw pointer and sets the reference count to 1.
   *
   * @param data The raw pointer to manage.
   */
  RefCountedPtr(T *);

  /**
   * @brief Constructs a RefCountedPtr with variadic arguments.
   *
   * Allocates a new object of T with the given arguments and sets the
   * reference count to 1.
   *
   * @tparam Args Variadic template for constructor arguments.
   * @param args Arguments to pass to the T constructor.
   */
  template <typename... Args> RefCountedPtr(Args &&...args);

  /**
   * @brief Copy constructor for sharing ownership.
   *
   * Shares the managed object and reference count with another RefCountedPtr,
   * incrementing the count.
   *
   * @param other The RefCountedPtr to share ownership with.
   */
  RefCountedPtr(RefCountedPtr<T> &);

  /**
   * @brief Destructor that cleans up resources.
   *
   * Decrements the reference count and releases resources if it reaches zero.
   */
  ~RefCountedPtr();

  /**
   * @brief Retrieves the raw pointer to the managed object.
   *
   * Returns the underlying pointer, or nullptr if no object is managed.
   *
   * @return T* The raw pointer to the managed object.
   */
  T *get_data();

  /**
   * @brief Assignment operator for sharing ownership.
   *
   * Releases current resources (if any) and shares ownership with another
   * RefCountedPtr, adjusting reference counts accordingly.
   *
   * @param other The RefCountedPtr to assign from.
   * @return RefCountedPtr<T>& Reference to this RefCountedPtr.
   */
  RefCountedPtr<T> &operator=(RefCountedPtr<T> &);
};

#include "RefCountedPtr.tpp"

#endif