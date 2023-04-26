/**
 * @file subject.hpp
 * @author fuechs
 * @brief Error Subject Header
 * @version 0.1
 * @date 2023-03-25
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../metadata.hpp"
#include "marking.hpp"
#include "suggestion.hpp"

class Subject {
public:
    using Ptr = shared_ptr<Subject>;
    using Vec = vector<Ptr>;

    Subject(Metadata meta = {}, Marking::Vec markings = {}, Suggestion::Vec suggestions = {},
        Vec references = {}, Ptr traceback = nullptr);
    ~Subject();

    string print();

private:
    Marking::Vec markings;
    Suggestion::Vec suggestions;
    Vec references;
    Ptr traceback;
    Metadata meta;
};