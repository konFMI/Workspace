//#include <iostream>


// template <typename T>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ChiPtrView
///
/// @brief  This class provides C++ interfaces over C style arrays of the form (raw pointer + size)
///         This enables them to be used naturally by std::algorithms and within ranged based for loops
///         As a view type, this object does not define/own the lifetime of the data being viewed
///
/// @tparam T Type of elements within this slice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class ChiPtrView
// {
// public:
//     explicit constexpr ChiPtrView(T* pBegin, T* pEnd) noexcept : pBegin(pBegin), pEnd(pEnd) {}
//     explicit constexpr ChiPtrView(size_t numElements, T* pBegin) noexcept : ChiPtrView(pBegin, pBegin + numElements) {}
//
//     constexpr T*       begin()  const noexcept { return pBegin; }
//     constexpr T*       end()    const noexcept { return pEnd; }
//     constexpr const T* cbegin() const noexcept { return pBegin; }
//     constexpr const T* cend()   const noexcept { return pEnd; }
//
//     constexpr auto rbegin()  const noexcept { return std::reverse_iterator(pEnd); }
//     constexpr auto rend()    const noexcept { return std::reverse_iterator(pBegin); }
//     constexpr auto crbegin() const noexcept { return std::reverse_iterator(pEnd); }
//     constexpr auto crend()   const noexcept { return std::reverse_iterator(pBegin); }
//
//     constexpr size_t size() const noexcept { return std::distance(pBegin, pEnd); }
//
//     constexpr auto& operator[](size_t idx) noexcept { return pBegin[idx]; }
//     constexpr auto& operator[](size_t idx) const  noexcept { return pBegin[idx]; }
//     ChiPtrView(const ChiPtrView& rOther) = default;
//
// private:
//     T* pBegin;
//     T* pEnd;
// };


int main()
{
    //int a;
    //int *arr[] = {};

    return 0;
}
