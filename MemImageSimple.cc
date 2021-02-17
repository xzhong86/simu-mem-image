
#include "MemImage.h"

#include <unordered_map>
#include <algorithm>

#include <string.h>
#include <assert.h>

namespace NS_MemImage {

class MemImageSimple : public MemImage {
    struct Block {
        static const size_t SIZE = 16UL << 10; // 16KB
        uint8_t data[SIZE];

        static addr_t align(addr_t addr) {
            return addr & ~(SIZE - 1);
        }
    };
    std::unordered_map<addr_t, Block*> bank_;

    addr_t align(addr_t addr) const { return Block::align(addr); }

    Block *getBlock(addr_t _addr, bool create = false) {
        auto addr = align(_addr);
        auto it = bank_.find(addr);
        if (it != bank_.end())
            return it->second;
        if (!create)
            return nullptr;
        Block *blk = new Block;
        bank_.insert({addr, blk});
        return blk;
    }
    size_t rwBlock(bool rd, Block *blk, addr_t addr, void *_buf, size_t max_size) {
        size_t blk_off = addr - align(addr);
        size_t size = std::min(Block::SIZE - blk_off, max_size);
        uint8_t *buf = (uint8_t*)_buf;
        if (blk) {
            void *dst = rd ? buf : blk->data + blk_off;
            void *src = rd ? blk->data + blk_off : buf;
            memcpy(dst, src, size);
        }
        else if (rd) {
            memset(buf, 0, size);
        }
        else {
            assert(0);
        }
        return size;
    }

public:
    MemImageSimple() {
    }

    void readMem(addr_t addr, void *_buf, size_t size) override {
        uint8_t *buf = (uint8_t*)_buf;
        for (int off = 0; off < size; ) {
            Block *blk = getBlock(addr + off);
            off += rwBlock(true, blk, addr + off, buf + off, size - off);
        }
    }
    void writeMem(addr_t addr, void *_buf, size_t size) override {
        uint8_t *buf = (uint8_t*)_buf;
        for (int off = 0; off < size; ) {
            Block *blk = getBlock(addr + off, true);
            off += rwBlock(false, blk, addr + off, buf + off, size - off);
        }
    }

    void traverse(const std::function<void(addr_t,void*,size_t)> &cb) override {
        for (auto it : bank_) {
            cb(it.first, it.second->data, Block::SIZE);
        }
    }
    
};


template <>
MemImage * MemImage::create<MemImageSimple>() { return new MemImageSimple(); }


} // namespace


