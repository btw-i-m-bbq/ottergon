#pragma once

#include <vector>

#include <components/document/document.hpp>
#include <components/document/wrapper_value.hpp>
#include <components/expressions/compare_expression.hpp>
#include <components/ql/index.hpp>
#include <components/cursor/cursor.hpp>

namespace components::index {

    using components::ql::index_type;
    using document_ptr = components::document::document_ptr;
    using doc_t = components::document::document_ptr;
    using key_t = expressions::key_t;
    using components::ql::keys_base_storage_t;
    using id_index = uint32_t;
    using value_t = ::document::wrapper_value_t;
    using query_t = expressions::compare_expression_ptr;
    using result_set_t = cursor::sub_cursor_t;

}