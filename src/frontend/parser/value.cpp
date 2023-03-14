/**
 * @file value.cpp
 * @author fuechs
 * @brief value struct
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "value.hpp"

ValueStruct::~ValueStruct() {
    if (type.kind == FuxType::LIT)
        __lit.clear();
}

#ifdef FUX_BACKEND
Eisdrache::Local &ValueStruct::getLLVMValue(Eisdrache *eisdrache) {
    Eisdrache::Local that;

    switch (type.kind) {
        case FuxType::BOOL:     that = Eisdrache::Local(eisdrache, eisdrache->getBool(__bool)); break;
        case FuxType::I8:       that = Eisdrache::Local(eisdrache, eisdrache->getSignedTy(8), eisdrache->getInt(8, __i8)); break;
        case FuxType::U8:       that = Eisdrache::Local(eisdrache, eisdrache->getInt(8, __u8)); break;
        case FuxType::C8:       that = Eisdrache::Local(eisdrache, eisdrache->getInt(8, __c8)); break;
        case FuxType::I16:      that = Eisdrache::Local(eisdrache, eisdrache->getSignedTy(16), eisdrache->getInt(16, __i16)); break;
        case FuxType::U16:      that = Eisdrache::Local(eisdrache, eisdrache->getInt(16, __u16)); break;
        case FuxType::C16:      that = Eisdrache::Local(eisdrache, eisdrache->getInt(16, __c16)); break;
        case FuxType::F16:      /*that = Eisdrache::Local(eisdrache, eisdrache->getFloatTy(16), eisdrache->getFloat(__f16));*/ break;
        case FuxType::I32:      that = Eisdrache::Local(eisdrache, eisdrache->getSignedTy(32), eisdrache->getInt(32, __i32)); break;
        case FuxType::U32:      that = Eisdrache::Local(eisdrache, eisdrache->getInt(32, __u32)); break;
        case FuxType::F32:      that = Eisdrache::Local(eisdrache, eisdrache->getFloatTy(32), eisdrache->getFloat(__f32)); break;
        case FuxType::I64:      that = Eisdrache::Local(eisdrache, eisdrache->getSignedTy(64), eisdrache->getInt(64, __i64)); break;
        case FuxType::U64:      that = Eisdrache::Local(eisdrache, eisdrache->getInt(64, __u64)); break;
        case FuxType::F64:      that = Eisdrache::Local(eisdrache, eisdrache->getFloatTy(64), eisdrache->getFloat(__f64)); break;
        case FuxType::LIT:      that = Eisdrache::Local(eisdrache, eisdrache->getUnsignedPtrTy(8), eisdrache->getLiteral(__lit, "literal")); break;
        default:                that = Eisdrache::Local(eisdrache);
    }

    return eisdrache->getCurrentParent().addLocal(that);
}
#endif