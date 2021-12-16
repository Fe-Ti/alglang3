//  Copyright 2021 Fe-Ti
//  Shared Pointer

#include <iostream>

template<typename T> SharedPtrControlBlock<T>::SharedPtrControlBlock()
{
    this->_obj_ptr = nullptr;
    this->ref_cnt = 1;
}
template<typename T>
SharedPtrControlBlock<T>::SharedPtrControlBlock(T* _obj_ptr)
{
    this->_obj_ptr = _obj_ptr;
    this->ref_cnt = 1;
}

template<typename T> SharedPtrControlBlock<T>::~SharedPtrControlBlock()
{
    delete _obj_ptr;
}

template<typename T>
void
SharedPtrControlBlock<T>::inc_ref_cnt()
{
    ++ref_cnt;
}

template<typename T>
void
SharedPtrControlBlock<T>::dec_ref_cnt()
{
    --ref_cnt;
    if(!is_alive())
        delete this;
}

template<typename T>
T*
SharedPtrControlBlock<T>::get_raw_ptr() const
{
    return _obj_ptr;
}

template<typename T>
void
SharedPtrControlBlock<T>::set_raw_ptr(T* _obj_ptr)
{
    this->_obj_ptr = _obj_ptr;
}

template<typename T>
size_t
SharedPtrControlBlock<T>::use_count() const
{
    return ref_cnt;
}

template<typename T>
bool
SharedPtrControlBlock<T>::is_alive() const
{
    return ref_cnt > 0;
}

template<typename T> SharedPtr<T>::SharedPtr()
{
    _cb_ptr = new SharedPtrControlBlock<T>();
}
template<typename T> SharedPtr<T>::SharedPtr(T* ptr)
{
    _cb_ptr = new SharedPtrControlBlock<T>(ptr);
}
template<typename T> SharedPtr<T>::SharedPtr(const SharedPtr<T>& r)
{
    _cb_ptr = r._cb_ptr;
    _cb_ptr->inc_ref_cnt();
}
template<typename T> SharedPtr<T>::SharedPtr(SharedPtr&& r)
{
    _cb_ptr = r._cb_ptr;
    r._cb_ptr = nullptr;
}
template<typename T> SharedPtr<T>::~SharedPtr()
{
    if(_cb_ptr != nullptr)
        _cb_ptr->dec_ref_cnt();
}
template<typename T>
SharedPtr<T>&
SharedPtr<T>::operator=(const SharedPtr<T>& r)
{
    _cb_ptr->dec_ref_cnt();
    _cb_ptr = r._cb_ptr;
    _cb_ptr->inc_ref_cnt();
    return *this;
}
template<typename T>
SharedPtr<T>&
SharedPtr<T>::operator=(SharedPtr&& r)
{
    _cb_ptr = r._cb_ptr;
    r._cb_ptr = nullptr;
    return *this;
}
template<typename T>
T&
SharedPtr<T>::operator[](size_t index)
{
    return _cb_ptr->get_raw_ptr()[index];
}
template<typename T> SharedPtr<T>::operator bool() const
{
    return _cb_ptr->get_raw_ptr() != nullptr;
}
template<typename T>
T&
SharedPtr<T>::operator*()
{
    return *_cb_ptr->get_raw_ptr();
}
template<typename T>
T*
SharedPtr<T>::operator->()
{
    return _cb_ptr->get_raw_ptr();
}
template<typename T>
T*
SharedPtr<T>::get() const
{
    return _cb_ptr->get_raw_ptr();
}
template<typename T>
void
SharedPtr<T>::reset(T* ptr)
{
    if(_cb_ptr->get_raw_ptr() != ptr)
        _cb_ptr->dec_ref_cnt();
    _cb_ptr = new SharedPtrControlBlock<T>(ptr);
}

template<typename T>
void
SharedPtr<T>::reset()
{
    reset(nullptr);
}
template<typename T>
void
SharedPtr<T>::swap(SharedPtr& r)
{
    T* tmp = r._cb_ptr->get_raw_ptr();
    r._cb_ptr->set_raw_ptr(this->_cb_ptr->get_raw_ptr());
    this->_cb_ptr->set_raw_ptr(tmp);
}
template<typename T>
size_t
SharedPtr<T>::use_count() const
{
    return _cb_ptr->use_count();
}
