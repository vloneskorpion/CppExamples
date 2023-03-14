// https://www.cppstories.com/2021/constexpr-new-cpp20/
#include <numeric>
#include <vector>
#include <memory>

template <typename T>
class Buffer {
public:
    constexpr Buffer(size_t n) noexcept : size_(n), mem_(new T[n]) { }
    constexpr ~Buffer() noexcept { delete [] mem_; }

    constexpr Buffer(const Buffer& other) noexcept : size_(other.size_) {
        if (&other != this) {
            mem_ = new T[size_];            
            std::copy(other.mem_, other.mem_ + size_, mem_);
        }
    }

    constexpr Buffer(Buffer&& other) noexcept {
        if (&other != this) {
            mem_ = other.mem_;            
            size_ = other.size_;
            other.mem_ = nullptr;
            other.size_ = 0;
        }
    }

    constexpr Buffer& operator=(const Buffer& other) noexcept {
        if (&other != this) {
            mem_ = new T[size_];            
            std::copy(other.mem_, other.mem_ + size_, mem_);
        }
        return *this;
    }

    constexpr Buffer& operator=(Buffer&& other) noexcept {
        if (&other != this) {
            mem_ = other.mem_;            
            size_ = other.size_;
            other.mem_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    constexpr T& operator[](size_t id) noexcept { return mem_[id]; }
    constexpr const T& operator[](size_t id) const noexcept{ return mem_[id]; }

    constexpr T* data() const noexcept { return mem_; }
    constexpr size_t size() const noexcept { return size_; }

private:
    T *mem_ { nullptr };
    size_t size_ { 0 };
};

constexpr void delFunc(int* p)
{
    delete [] p;
}

constexpr int naiveSum(unsigned int n) {
    auto p = new int[n];
    std::iota(p, p+n, 1);
    auto tmp = std::accumulate(p, p+n, 0);
    delFunc(p);
    return tmp;
}

constexpr int naiveSumBuffer(unsigned int n) {
    Buffer<int> buf(n);
    std::iota(buf.data(), buf.data()+n, 1);
    return std::accumulate(buf.data(), buf.data()+n, 0);
}

constexpr int smartSum(unsigned int n) {
    return n*(n+1)/2;
}

int main() {
    static_assert(naiveSum(10) == smartSum(10));
    static_assert(naiveSumBuffer(1'001) == smartSum(1'001));
    static_assert(naiveSumBuffer(1'002) == smartSum(1'002));


    return 0;
}