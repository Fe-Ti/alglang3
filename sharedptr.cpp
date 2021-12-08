#include <atomic>

class SharedPtrControlBlock;

template<typename T> class SharedPtr
{
    T* _cb_ptr;

  public:
    SharedPtr()
    {
        _cb_ptr = SharedPtrControlBlock();
    }
    SharedPtr(T* ptr)
    {
        _cb_ptr = SharedPtrControlBlock();
    }
    SharedPtr(const SharedPtr& r);
    SharedPtr(SharedPtr&& r);
    ~SharedPtr();
    auto operator=(const SharedPtr& r) -> SharedPtr&;
    auto operator=(SharedPtr&& r) -> SharedPtr&;

    // проверяет, указывает ли указатель на объект
    operator bool() const;
    auto operator*() const -> T&;
    auto operator->() const -> T*;

    auto get() -> T*;
    void reset();
    void reset(T* ptr);
    void swap(SharedPtr& r);
    // возвращает количество объектов SharedPtr, которые ссылаются на тот же
    // управляемый объект
    auto use_count() const -> size_t;
    ~SharedPtr()
    {
        
    }
};

template<typename T> class SharedPtrControlBlock
{
    T* obj_ptr;
    std::atomic<std::uint64_t> ref_cnt;
    // std::atomic<std::uint64_t> weak_ref_cnt;
  public:
    SharedPtrControlBlock(T* _obj_ptr = nullptr)
    {
        this->_obj_ptr = _obj_ptr;
    }

    void inc_ref_cnt()
    {
        ++ref_cnt;
    }
    void dec_ref_cnt()
    {
        --ref_cnt;
    }

    T* get_raw_ptr()
    {
        return _obj_ptr;
    }
    void set_raw_ptr(T* _obj_ptr)
    {
        this->_obj_ptr = _obj_ptr;
    }

    size_t use_count()
    {
        return ref_cnt;
    }
    bool is_alive()
    {
        return ref_cnt > 0;
    }
};
