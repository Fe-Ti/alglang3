//  Copyright 2021 Fe-Ti
//  Shared Pointer
#include <atomic>
#include <cstddef>
template<typename T> class SharedPtrControlBlock
{
    T* _obj_ptr;
    std::atomic<uint64_t> ref_cnt;
    // std::atomic<uint64_t> weak_ref_cnt;
  public:
    SharedPtrControlBlock();
    SharedPtrControlBlock(T* _obj_ptr);
    ~SharedPtrControlBlock();

    void inc_ref_cnt();
    void dec_ref_cnt();

    T* get_raw_ptr() const;
    void set_raw_ptr(T* _obj_ptr);
    size_t use_count() const;
    bool is_alive() const;
};

template<typename T> class SharedPtr
{
    SharedPtrControlBlock<T>* _cb_ptr;

  public:
    SharedPtr();
    SharedPtr(T* ptr);
    SharedPtr(const SharedPtr& r);
    SharedPtr(SharedPtr&& r);
    ~SharedPtr();

    SharedPtr& operator=(const SharedPtr& r);
    SharedPtr& operator=(SharedPtr&& r);

    T& operator[](size_t index);

    operator bool() const;

    T& operator*();
    T* operator->();
    T* get() const;

    void reset(T* ptr);
    void reset();

    void swap(SharedPtr& r);
    size_t use_count() const;
};

#include "sharedptr.cpp"
