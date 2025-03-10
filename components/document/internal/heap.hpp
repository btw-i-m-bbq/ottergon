#pragma once

#include <components/document/core/value.hpp>
#include <components/document/support/ref_counted.hpp>

namespace document::impl::internal {

    class value_slot_t;

    struct offset_value_t {
        uint8_t pad_ = 0xFF;
        uint8_t header_ = 0;

        offset_value_t() = default;
        offset_value_t(const offset_value_t&) = delete;
        offset_value_t(offset_value_t&&) = delete;
        offset_value_t& operator=(const offset_value_t&) = delete;
    };

    class heap_value_t : public ref_counted_t
        , offset_value_t {
    public:
        static heap_value_t* create(nullptr_t);
        static heap_value_t* create(bool b);
        static heap_value_t* create(int i);
        static heap_value_t* create(unsigned i);
        static heap_value_t* create(int64_t i);
        static heap_value_t* create(uint64_t i);
        static heap_value_t* create(float f);
        static heap_value_t* create(double d);
        static heap_value_t* create(std::string_view s);
        static heap_value_t* create(const value_t* v);

        static const value_t* as_value(heap_value_t* v) PURE;
        const value_t* as_value() const PURE;

        static bool is_heap_value(const value_t* v) PURE;
        static heap_value_t* as_heap_value(const value_t* v);

        static const value_t* retain(const value_t* v);
        static void release(const value_t* v);

        void* operator new(size_t size);
        void operator delete(void* ptr);
        void operator delete(void* ptr, size_t);

    protected:
        ~heap_value_t() = default;
        static heap_value_t* create(tags tag, int tiny, std::string_view extra_data);
        heap_value_t(tags tag, int tiny);
        tags tag() const;

    private:
        friend class document::impl::internal::value_slot_t;

        static void* operator new(size_t size, size_t extra_size);
        heap_value_t() = default;
        static heap_value_t* create_str(internal::tags tag, std::string_view s);

        template<class INT>
        static heap_value_t* create_int(INT i, bool is_unsigned);
    };

    class heap_collection_t : public heap_value_t {
    public:
        static retained_t<heap_collection_t> mutable_copy(const value_t* v, tags if_type);
        bool is_changed() const PURE;

    protected:
        heap_collection_t(internal::tags tag);
        ~heap_collection_t() = default;

        void set_changed(bool c);

    private:
        bool changed_{false};
    };

}
