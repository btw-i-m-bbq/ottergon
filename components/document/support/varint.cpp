#include "varint.hpp"
#include "utils.hpp"

#include <cstdint>
#include <cstring>

#include <algorithm>

#include <components/document/support/better_assert.hpp>
#include <components/document/support/endian.hpp>

namespace document {

size_t size_of_var_int(uint64_t n) {
    size_t size = 1;
    while (n >= 0x80) {
        size++;
        n >>= 7;
    }
    return size;
}

size_t put_uvar_int(void *buf, uint64_t n) {
    uint8_t* dst = static_cast<uint8_t*>(buf);
    while (n >= 0x80) {
        *dst++ = (n & 0xFF) | 0x80;
        n >>= 7;
    }
    *dst++ = uint8_t(n);
    return size_t(dst - static_cast<uint8_t*>(buf));
}

size_t put_int_of_length(void *buf, int64_t n, bool is_unsigned) {
    auto littlen = int64_t(endian::little_enc64(uint64_t(n)));
    memcpy(buf, &littlen, 8);
    size_t size;
    if (is_unsigned) {
        for (size = 8; size > 1; --size) {
            uint8_t byte = static_cast<uint8_t*>(buf)[size-1];
            if (byte != 0) {
                break;
            }
        }
        return size;
    } else {
        uint8_t trim = (n >= 0) ? 0 : 0xFF;
        for (size = 8; size > 1; --size) {
            uint8_t byte = static_cast<uint8_t*>(buf)[size-1];
            if (byte != trim) {
                if ((byte ^ trim) & 0x80)
                    ++size;
                break;
            }
        }
        return size;
    }
}

size_t _get_uvar_int(std::string_view  buf, uint64_t *n) {
    auto pos = reinterpret_cast<const uint8_t*>(buf.data());
    auto end = pos + std::min(buf.size(), size_t(max_varint_len64));
    uint64_t result = *pos++ & 0x7F;
    int shift = 7;
    while (pos < end) {
        uint8_t byte = *pos++;
        if (_usually_true(byte >= 0x80)) {
            result |= uint64_t(byte & 0x7F) << shift;
            shift += 7;
        } else {
            result |= uint64_t(byte) << shift;
            *n = result;
            auto nBytes = size_t(pos - reinterpret_cast<const uint8_t*>(buf.data()));
            if (_usually_false(nBytes == max_varint_len64 && byte > 1))
                nBytes = 0;
            return nBytes;
        }
    }
    return 0;
}

size_t _get_uvar_int32(std::string_view buf, uint32_t *n) {
    uint64_t n64;
    size_t size = _get_uvar_int(buf, &n64);
    if (size == 0 || n64 > UINT32_MAX)
        return 0;
    *n = uint32_t(n64);
    return size;
}

}
