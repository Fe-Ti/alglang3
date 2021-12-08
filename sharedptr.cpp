//  Copyright 2021 Fe-Ti
//  Shared Pointer
#include <atomic>

class SharedPtrControlBlock;

template<typename T> class SharedPtr
{
    SharedPtrControlBlock* _cb_ptr;
    bool is_invalid() const
    {
        return _cb_ptr == nullptr;
    }

  public:
    SharedPtr()
    {
        _cb_ptr = new SharedPtrControlBlock();
    }
    SharedPtr(T* ptr)
    {
        _cb_ptr = new SharedPtrControlBlock(ptr);
    }
    SharedPtr(const SharedPtr& r)
    {
        _cb_ptr = r._cb_ptr;
        if(!is_invalid())
            _cb_ptr->inc_ref_cnt();
    }
    SharedPtr(SharedPtr&& r)
    {
        _cb_ptr = r._cb_ptr;
        r._cb_ptr = nullptr;
    }
    ~SharedPtr()
    {
        if(is_invalid()) // if is invalid, i.e. _cb_prt == nullptr
            return;      // do nothing
        _cb_ptr->dec_ref_cnt();
        if(!_cb_ptr->is_alive()) { // if is not alive
            delete _cb_ptr;        // then delete the control block
        }
    }

    SharedPtr& operator=(const SharedPtr& r)
    {
        if(!is_invalid())
            _cb_ptr->dec_ref_cnt();
        _cb_ptr = r._cb_ptr;
        if(!is_invalid())
            _cb_ptr->inc_ref_cnt();
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& r)
    {
        _cb_ptr = r._cb_ptr;
        r._cb_ptr = nullptr;
        return *this;
    }

    operator bool() const;
    auto operator*() const -> T&;
    auto operator->() const -> T*;

    T* get() const
    {
        return _cb_ptr->get_raw_ptr();
    }

    void reset()
    {
        delete _cb_ptr->get_raw_ptr();
    }
    void reset(T* ptr)
    {
        ;
    }

    void swap(SharedPtr& r)
    {
        T* tmp = r._cb_ptr->get_raw_ptr();
        r._cb_ptr->set_raw_ptr(this->_cb_ptr->get_raw_ptr());
        this->_cb_ptr->set_raw_ptr(tmp);
    }
    size_t use_count() const
    {
        return _cb_ptr->use_count();
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
    ~SharedPtrControlBlock()
    {
        delete _obj_ptr;
    }

    void inc_ref_cnt()
    {
        ++ref_cnt;
    }
    void dec_ref_cnt()
    {
        --ref_cnt;
        if(!is_alive())
            ~SharedPtrControlBlock();
    }

    T* get_raw_ptr() const
    {
        return _obj_ptr;
    }
    void set_raw_ptr(T* _obj_ptr)
    {
        this->_obj_ptr = _obj_ptr;
    }

    size_t use_count() const
    {
        return ref_cnt;
    }
    bool is_alive() const
    {
        return ref_cnt > 0;
    }
};
