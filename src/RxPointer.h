#ifndef RXPOINTER_HEADER
#define RXPOINTER_HEADER

#include <atomic>

/**
 * @brief A custom shared pointer class for managing shared ownership of objects.
 *
 * RxPointer enables multiple pointers to share ownership of a dynamically allocated
 * object, automatically deleting it when the last reference is destroyed.
 *
 * @tparam MemoryType The type of the object being managed.
 */
template<typename MemoryType>
class RxPointer {
private:
    MemoryType* data;                       ///< Pointer to the managed object.
    std::atomic<int>* shared_references;    ///< Pointer to the shared reference count.

    /**
     * @brief Initializes the pointer with the given data and reference count.
     *
     * Configures the data pointer and links it to a shared reference count,
     * incrementing the count to track ownership.
     *
     * @param data Pointer to the object to manage.
     * @param shared_references Pointer to the atomic reference count.
     */
    void init_data(MemoryType*, std::atomic<int>*);

    /**
     * @brief Releases the managed object and its reference count.
     *
     * Deletes the managed object and reference count when no references remain.
     */
    void release_data();

public:
    /**
     * @brief Default constructor creating an empty RxPointer.
     *
     * Initializes data and shared_references to nullptr, representing no ownership.
     */
    RxPointer():data(nullptr), shared_references(nullptr){}

    /**
     * @brief Constructs an RxPointer from a raw pointer.
     *
     * Assumes ownership of the raw pointer and sets the reference count to 1.
     *
     * @param data The raw pointer to manage.
     */
    RxPointer(MemoryType*);

    /**
     * @brief Constructs an RxPointer with variadic arguments.
     *
     * Allocates a new object of MemoryType with the given arguments and sets the
     * reference count to 1.
     *
     * @tparam Args Variadic template for constructor arguments.
     * @param args Arguments to pass to the MemoryType constructor.
     */
    template<typename ...Args>
    RxPointer(Args&&... args);

    /**
     * @brief Copy constructor for sharing ownership.
     *
     * Shares the managed object and reference count with another RxPointer,
     * incrementing the count.
     *
     * @param other The RxPointer to share ownership with.
     */
    RxPointer(RxPointer<MemoryType>&);

    /**
     * @brief Destructor that cleans up resources.
     *
     * Decrements the reference count and releases resources if it reaches zero.
     */
    ~RxPointer();

    /**
     * @brief Retrieves the raw pointer to the managed object.
     *
     * Returns the underlying pointer, or nullptr if no object is managed.
     *
     * @return MemoryType* The raw pointer to the managed object.
     */
    MemoryType* get_data();

    /**
     * @brief Assignment operator for sharing ownership.
     *
     * Releases current resources (if any) and shares ownership with another RxPointer,
     * adjusting reference counts accordingly.
     *
     * @param other The RxPointer to assign from.
     * @return RxPointer<MemoryType>& Reference to this RxPointer.
     */
    RxPointer<MemoryType>& operator=(RxPointer<MemoryType>&);
};

#include "RxPointer.tpp"

#endif