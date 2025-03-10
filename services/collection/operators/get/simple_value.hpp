#pragma once

#include "operator_get.hpp"

namespace services::collection::operators::get {

    class simple_value_t : public operator_get_t {
    public:
        static operator_get_ptr create(const components::expressions::key_t &key);

    private:
        const components::expressions::key_t key_;

        explicit simple_value_t(const components::expressions::key_t &key);

        document::wrapper_value_t get_value_impl(const components::document::document_ptr& document) final;
    };

} // namespace services::collection::operators::get
