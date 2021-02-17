// -*- C++ -*-
// public header

#pragma once

#include <cstdint>
#include <functional>

#ifndef NS_MemImage
#define NS_MemImage mem_image
#endif

namespace NS_MemImage {

class MemImage {
public:
    typedef std::uint64_t addr_t;
    typedef std::uint32_t size_t;

    virtual void readMem (addr_t addr, void *buf, size_t size) = 0;
    virtual void writeMem(addr_t addr, void *buf, size_t size) = 0;
    virtual void traverse(const std::function<void(addr_t,void*,size_t)> &cb) = 0;

    virtual MemImage *clone();
    virtual void clear(addr_t addr, size_t size);

    template <class T>
    static MemImage * create();
};

MemImage *createMemImage(const char *type);

} // namespace

