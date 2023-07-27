#pragma once

#include "operator_merge.hpp"

namespace services::collection::operators::merge {

    class operator_or_t : public operator_merge_t {
    public:
        explicit operator_or_t(context_collection_t* context, predicates::limit_t limit);

    private:
        void on_merge_impl(components::pipeline::context_t* pipeline_context) final;
    };

} // namespace services::collection::operators::merge
