/**
 * @file keypair.hpp
 * @author fuechs
 * @brief fux keypair header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022
 * 
 */

#pragma once


template<class K, class T>
struct keypair {
    keypair(K key, T value) : key(k), value(v) {}
    keypair() {}
    
    void set(K k, T val) {
        key = k;
        value = val;
    }

    K key;
    T value;
};