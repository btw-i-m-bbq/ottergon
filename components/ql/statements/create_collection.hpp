#pragma once

#include <msgpack.hpp>
#include <msgpack/zone.hpp>
#include <msgpack/adaptor/list.hpp>
#include "components/ql/ql_statement.hpp"

namespace components::ql {

    struct create_collection_t final : ql_statement_t {
        create_collection_t(const database_name_t& database, const collection_name_t &collection);
        create_collection_t() = default;
        create_collection_t(const create_collection_t&) = default;
        create_collection_t& operator=(const create_collection_t&) = default;
        create_collection_t(create_collection_t&&) = default;
        create_collection_t& operator=(create_collection_t&&) = default;
        ~create_collection_t() final = default;
    };

} // namespace components::ql

// User defined class template specialization
namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {

            template<>
            struct convert<components::ql::create_collection_t> final {
                msgpack::object const& operator()(msgpack::object const& o, components::ql::create_collection_t& v) const {
                    if (o.type != msgpack::type::ARRAY) {
                        throw msgpack::type_error();
                    }
                    if (o.via.array.size != 2) {
                        throw msgpack::type_error();
                    }
                    v.database_ = o.via.array.ptr[0].as<database_name_t>();
                    v.collection_ = o.via.array.ptr[1].as<database_name_t>();
                    return o;
                }
            };

            template<>
            struct pack<components::ql::create_collection_t> final {
                template<typename Stream>
                packer<Stream>& operator()(msgpack::packer<Stream>& o, components::ql::create_collection_t const& v) const {
                    o.pack_array(2);
                    o.pack(v.database_);
                    o.pack(v.collection_);
                    return o;
                }
            };

            template<>
            struct object_with_zone<components::ql::create_collection_t> final {
                void operator()(msgpack::object::with_zone& o, components::ql::create_collection_t const& v) const {
                    o.type = type::ARRAY;
                    o.via.array.size = 2;
                    o.via.array.ptr = static_cast<msgpack::object*>(o.zone.allocate_align(sizeof(msgpack::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(msgpack::object)));
                    o.via.array.ptr[0] = msgpack::object(v.database_, o.zone);
                    o.via.array.ptr[1] = msgpack::object(v.collection_, o.zone);
                }
            };

        } // namespace adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack
