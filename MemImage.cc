
#include "MemImage.h"

#include <string>
#include <algorithm>

namespace NS_MemImage {

// common methods of MemImage
void MemImage::clear(addr_t addr, size_t size) {
    const size_t SIZE = 512;
    uint8_t buf[SIZE];
    for (size_t off = 0; off < size; off += SIZE) {
        writeMem(addr + off, buf, std::min(SIZE, size - off));
    }
}
MemImage * MemImage::clone() {
    MemImage *target = createMemImage("simple");
    auto cb = [target](addr_t addr, void *src_buf, size_t size) {
        target->writeMem(addr, src_buf, size);
    };
    traverse(cb);
    return target;
}

class MemImageSimple;

MemImage *createMemImage(const char *_type)
{
    std::string type(_type);
    if (type == "simple")
        return MemImage::create<MemImageSimple>();
    else
        return nullptr;
}

} // namespace

