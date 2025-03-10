#include "operator_max.hpp"
#include <services/collection/collection.hpp>

namespace services::collection::operators::aggregate {

    constexpr auto key_result_ = "max";

    operator_max_t::operator_max_t(context_collection_t *context, components::index::key_t key)
        : operator_aggregate_t(context)
        , key_(std::move(key)) {
    }

    document_ptr operator_max_t::aggregate_impl() {
        if (left_ && left_->output()) {
            const auto &documents = left_->output()->documents();
            auto max = std::max_element(documents.cbegin(), documents.cend(), [&](const document_ptr &doc1, const document_ptr &doc2) {
                return get_value_from_document(doc1, key_) < get_value_from_document(doc2, key_);
            });
            if (max != documents.cend()) {
                return components::document::make_document(key_result_, *get_value_from_document(*max, key_));
            }
        }
        return components::document::make_document(key_result_, 0);
    }

    std::string operator_max_t::key_impl() const {
        return key_result_;
    }

} // namespace services::collection::operators::aggregate
