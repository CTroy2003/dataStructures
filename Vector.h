#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { 
        if (_capacity == 0) {
            _capacity = 1;
        } 
        else {
            _capacity *= 2;
        }
    T* newArray = new T[_capacity];
    std::move(array, array + _size, newArray);
    delete[] array;
    array = newArray;  
    }

public:
    Vector() noexcept { 
        //default constructor
        _capacity = 0;
        _size = 0;
         array = nullptr;

    }

    Vector(size_t count, const T& value) { 
        //paramatized contructor
        _capacity = count;
        _size = count;
         array = new T[_capacity];

        for (size_t i=0;i<_size;++i){
            array[i] = value;
        }
    }

    explicit Vector(size_t count) { 
        //paramatized contructor
        _capacity = count;
        _size = count;
        array = new T[_capacity]{};

        
    }

    Vector(const Vector& other) { 
        //copy contructor
            _size = other._size;
            _capacity = other._capacity;
            array = new T[_capacity];
            for (size_t i=0;i<_size;++i){
                array[i] = other.array[i];
             }
    
    }

    Vector(Vector&& other) noexcept :_capacity(other._capacity), _size(other._size), array(other.array) {
        other._capacity = 0;
        other._size = 0;
        other.array = nullptr;
    }

    ~Vector() { 
        //destructor
        delete[] array;
        array = nullptr;
        _size = 0;
        _capacity = 0;
        //do we dereference the pointer?
    }

    Vector& operator=(const Vector& other) { 
        //Copy assignment operator
        _size = other._size;
        _capacity = other._capacity;
        if (this != &other){
            delete[]array;
            array = new T[_capacity];
            for (size_t i=0;i<_size;++i){
                array[i] = other.array[i];
            }
            
        }

    }

    Vector& operator=(Vector&& other) noexcept { 
         //Move assignment Operator
         if (this != &other){
            delete[]array;
            array = other.array;
            _size = other._size;
            _capacity = other._capacity;
            other.array = nullptr;
            other._size = 0;
            other._capacity = 0;
         }
            
    }

    iterator begin() noexcept { return iterator(array); }

    iterator end() noexcept { return iterator(array + _size); }

    [[nodiscard]] bool empty() const noexcept { 
        if (_size > 0){
            return false;
        }
        if (_size == 0){
            return true;
        }
    
    }

    size_t size() const noexcept { 
        
        return _size;
    }

    size_t capacity() const noexcept { 
        
        return _capacity;
    }

    T& at(size_t pos) { 
        if (pos < 0 || pos >= _size){
            throw std::out_of_range("");
        }
        return array[pos];
    }

    const T& at(size_t pos) const {
        if (pos < 0 || pos >= _size){
            throw std::out_of_range("");
        }
        return array[pos];
     }

    T& operator[](size_t pos) {
        return array[pos];
     }


    const T& operator[](size_t pos) const { 
        return array[pos]; }

    T& front() { 
        if (_size == 0){
            throw std::out_of_range("");
        }
        return array[0]; }

    const T& front() const { 
        if (_size == 0){
            throw std::out_of_range("");
        }
        return array[0]; }

    T& back() { 
        if (_size == 0){
            throw std::out_of_range("");
        }
        return array[_size-1]; }

    const T& back() const { 
        if (_size == 0){
            throw std::out_of_range("");
        }
        return array[_size-1]; }

    void push_back(const T& value) { 
        if (_capacity > _size){
            array[_size] = value; 
            ++_size;
        }
        else{
            grow();
            array[_size] = value;
            ++_size;
        }
     }

    void push_back(T&& value) { 

        if (_capacity > _size){
            array[_size] = std::move(value);
            ++_size;
        }
        else{
            grow();
            array[_size] = std::move(value);
            ++_size;
        }
    }

    void pop_back() { 
        _size -= 1;
     }

    iterator insert(iterator pos, const T& value) { 
        size_t index = pos - begin();
        if (_size == _capacity){
            grow();
        }
        for (size_t i = _size; i > index; --i){
            array[i] = std::move(array[i-1]);
        }
        array[index] = std::move(value);
        ++_size;
        pos = &array[index];
        return pos;
     }

    iterator insert(iterator pos, T&& value) { 
        size_t index = pos - begin();
        if (_size == _capacity){
                grow();   
        }
        for (size_t i = _size; i > index; --i){
            array[i] = std::move(array[i-1]);
        }
        array[index] = std::move(value);
        ++_size;
        pos = &array[index];
        return pos;
     }

    iterator insert(iterator pos, size_t count, const T& value) { 
        size_t index = pos - begin();
        if (_size + count > _capacity){
                while (_size + count > _capacity){
                   grow(); 
                }
        }
        for (size_t i = _size + count-1; i >= index + count; --i){
            array[i] = std::move(array[i-count]);
        }
        for (size_t i = 0; i < count; ++i){
            array[index+i] = std::move(value);
        }
        _size += count;
        pos = &array[index];
        return iterator(array + index);
        
     }

    iterator erase(iterator pos) {
        if (pos == end()) {
        return end();
    }
    size_t index = pos - begin();
    // Shift elements to the left to fill the gap created by erasing the element.
    for (size_t i = index; i < _size - 1; ++i) {
        array[i] = std::move(array[i + 1]);
    }
    // Decrease the size of the array to reflect the erasure.
    --_size;
    return pos;
    }

    iterator erase(iterator first, iterator last) { 
        size_t startIndex = first - begin();
        size_t endIndex = last - begin();

        if (startIndex >= _size || endIndex > _size || startIndex >= endIndex) {
            // Invalid range, return last to indicate nothing was erased.
            return last;
        }

        // Shift elements to the left to fill the gap created by erasing the range.
        size_t elementsToRemove = endIndex - startIndex;
        for (size_t i = endIndex; i < _size; ++i) {
            array[i - elementsToRemove] = std::move(array[i]);
        }
        _size -= elementsToRemove;
        return begin()+startIndex;}


    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
            pointer ptr;
    public:
        iterator() { 
            ptr = nullptr;
         }
        iterator(pointer a){
            ptr = a;
         }
        // Add any constructors that you may need

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { return *ptr; }
        [[nodiscard]] pointer operator->() const noexcept { return ptr; }

        // Prefix Increment: ++a
        iterator& operator++() noexcept { 
            ++ptr;
           return *this; 
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept { 
            iterator tmp = *this;
           ++ptr;
           return tmp; 
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
             --ptr;
           return *this; 
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept { 
            iterator tmp = *this;
           --ptr;
           return tmp;
        }

        iterator& operator+=(difference_type offset) noexcept { 
            ptr += offset;
           return *this; 
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { return iterator(ptr + offset); }
        
        iterator& operator-=(difference_type offset) noexcept { 
            ptr -= offset;
           return *this; 
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { return iterator(ptr - offset); }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { return ptr - rhs.ptr; }
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { return *(ptr + offset); }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { return ptr == rhs.ptr; }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { return ptr != rhs.ptr; }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { return ptr < rhs.ptr; }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { return ptr > rhs.ptr; }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { return ptr <= rhs.ptr; }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { return ptr >= rhs.ptr; }
    };


    void clear() noexcept { _size = 0; }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { return iterator + offset; }

#endif
