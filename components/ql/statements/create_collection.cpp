#include "create_collection.hpp"

namespace components::ql {

    create_collection_t::create_collection_t(const database_name_t& database, const collection_name_t &collection)
        : ql_statement_t(statement_type::create_collection, database, collection) {
    }

} // namespace components::ql
