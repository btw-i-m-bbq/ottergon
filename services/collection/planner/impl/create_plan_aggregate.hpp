#pragma once

#include <components/logical_plan/node.hpp>
#include <services/collection/operators/operator.hpp>

namespace services::collection::planner::impl {

    operators::operator_ptr create_plan_aggregate(context_collection_t* context,
            const components::logical_plan::node_ptr& node,
            components::ql::limit_t limit);

}
