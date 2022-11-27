/**
 * @file list.hpp
 * @author fuechs
 * @brief fux list header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <algorithm>
#include <sstream>

using namespace std;

template <class T>
class List {
public:
    List() {
        init(); 
    }

    void init() {
        data = NULL;
        len = 0;
    }

    void push_back(T &data) {
        __expand();
        this->data[len - 1] = data;
    }

    void add(T data) {
        __expand();
        this->data[len - 1] = data;
    }

    void push_back() {
        __expand();
    }

    void insert(unsigned long long pos, T &data) {
        if (pos > len) {
            stringstream ss;
            ss 
                << "index out of bound List::insert() _X: " 
                << pos < " length: " << len
            << endl;   
            throw std::runtime_error(ss.str());
        }

        if (len == 0) 
            push_back(data);
        else {
            T *result = new T[len + 1];
            long long newLen = len + 1;

            for (long long i = 0; i < pos; i++)
                result[i] = this->data[i];

            result[pos] = data;  
            
            for (long long i = pos + 1; i < newLen; i++)
                result[i] = this->data[i - 1];

            free();
            len = newLen;
            this->data = result;
        }
    }

    void remove(unsigned long long _X) {
        if (_X >= len || len == 0) {
            stringstream ss;
            ss 
                << "index out of bound List::remove() _X: " 
                << _X < " length: " << len
            << endl;   
            throw std::runtime_error(ss.str());
        }

        if (len == 1)
            free();
        else if (len == 2) {
            if (_X == 0)
                data[0] = data[1];
            
            __shrink();
        } else {
            T *result = new T[len - 1];
            long long newLen = len - 1, iter = 0;

            for (long long i = 0; i < _X; i++)
                result[iter++] = data[i]; 

            for (long long i = _X + 1; i < len; i++)
                result[iter++] = data[i - 1];

            free();
            len = newLen;
            data = result;
        }
    }

    /**
     * @warning programmer responsible for freeing data
     * 
     */
    void remove(T &_X) {
        for (unsigned long i = 0; i < len; i++) {
            if (_X == data[i]) {
                remove(i);
                return;
            }
        }
    }

    T at(unsigned long _X) {
        if (_X >= len) {
            stringstream ss;
            ss 
                << "index out of bound List::at() _X: " 
                << _X < " length: " << len
            << endl;   
            throw std::runtime_error(ss.str());
        }

        return data[_X];
    }

    // reference to data ( see List::at() )
    T &get(unsigned long _X) {
        if (_X >= len) {
            stringstream ss;
            ss 
                << "index out of bound List::get() _X: " 
                << _X < " length: " << len
            << endl;   
            throw std::runtime_error(ss.str());
        }

        return data[_X];
    }

    T &last() {
        if (len == 0) {
            stringstream ss;
            ss << "illegal state List::last() length: 0" << endl;
            throw std::runtime_error(ss.str());
        }

        return data[len - 1];
    }

    void free() {
        if (data != NULL) 
            delete[] data;
        data = NULL;
        len = 0;
    }

    unsigned long size() { return len; }

    void pop_back() {
        __shrink();
    }

    void addif(T _V) {
        for (unsigned int i = 0; i < len; i++)
            if (_V == data[i])
                return;
        push_back(_V);
    }

    void removefirst(T _V) {
        unsigned int iter = -1;
        for (unsigned int i = 0; i < len; i++)
            if (_V == data[i]) {
                iter = i;
                break;
            }
        
        if (iter != -1)
            remove(iter);
    }

    /**
     * @warning programmer responsible for freeing data
     * 
     */
    void replace(unsigned long _X, T repl) {
        if (_X >= len || _X < 0) {
            stringstream ss;
            ss 
                << "index out of bound List::replace() _X: " 
                << _X < " length: " << len
            << endl;   
            throw std::runtime_error(ss.str());
        }
        data[_X] = repl;
    }

    void addAll(List<T> &list){
        free();
        for (unsigned int i = 0; i < list.size(); i++)
            push_back(list.get(i));
    }

    void appendAll(List<T> &list){
        for (unsigned int i = 0; i < list.size(); i++)
            push_back(list.get(i));
    }

    // takes std::list instead of (this) List 
    void addAll(std::list<T> &list) {
        free();
        for (T &t : list)
            push_back(t);
    }

    bool find(T data) {
        for (unsigned int i = 0; i < len; i++)
            if (data == this->data[i])
                return true;
        
        return false;
    }

    long long indexof(T data) {
        for (unsigned int i = 0; i < len; i++)
            if (data == this->data[i])
                return i;

        return -1;
    }

    bool empty() { return len == 0; }


private:
    inline void __expand() {
        try {
            T *newData = new T[len + 1];
            std::copy(data, (T *) std::min(len, len + 1), newData);
            delete[] data;
            data = newData;
            ++len;
        } catch (std::exception &e) {
            --len;
            throw e;
        }
    }

    inline void __shrink() {
        try {
            if ((len - 1) <= 0) {
                free();
                return;
            }

            T *newbuf = new T[len - 1];
            std::copy_n(data, std::min(len - 1, len - 1), newbuf);
            delete[] data;
            data = newbuf;
            --len;
        } catch (std::exception &e) {
            ++len;
            throw e;
        }
    }
    
    T* data;
    unsigned long long len;
};