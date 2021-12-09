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

    operator bool() const
    {
        return _cb_ptr->get_raw_ptr() != nullptr;
    }

    T& operator[](size_t index)
    {
        return _cb_ptr->obj_ptr[index];
    }

    T& operator*()
    {
        return *_cb_ptr->obj_ptr;
    }
    T* operator->()
    {
        return _cb_ptr->get_raw_ptr();
    }
    T* get() const
    {
        return _cb_ptr->get_raw_ptr();
    }

    void reset()
    {
        reset(nullptr);
    }
    void reset(T* ptr)
    {
        if(!is_invalid()) {
            if(_cb_ptr->get_raw_ptr() != ptr)
                _cb_ptr->dec_ref_ptr();
            _cb_ptr = SharedPtrControlBlock(ptr);
        }
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
        this->ref_cnt = 1;
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


#include <iostream>
int
main()
{
    int n = 9;
    SharedPtr<uint64_t> sp(new uint64_t[n]);
    for ( int i = 0; i < n; ++i){
        sp[i] = i;
    }
    SharedPtr<uint64_t> sp1 = sp;
    SharedPtr<uint64_t> sp2 = sp1;
    SharedPtr<uint64_t> sp3 = sp2;
    sp.reset(new uint64_t[n]);
    for ( int i = 0; i < n; ++i){
        sp[i] = i * 4.023;
    }
    for ( int i = 0; i < n; ++i){
        std::cout << sp[i];
    }
    for ( int i = 0; i < n; ++i){
        std::cout << sp1[i];
    }
    return 0;
}

