#include "scalar_expression.hpp"
#include <boost/container_hash/hash.hpp>
#include <sstream>

namespace components::expressions {

    template<class OStream>
    OStream& operator<<(OStream& stream, const scalar_expression_t::param_storage& param) {
        std::visit([&stream](const auto& p) {
            using type = std::decay_t<decltype(p)>;
            if constexpr (std::is_same_v<type, core::parameter_id_t>) {
                stream << "#" << p;
            } else if constexpr (std::is_same_v<type, key_t>) {
                stream << "\"$" << p << "\"";
            } else if constexpr (std::is_same_v<type, expression_ptr>) {
                stream << p->to_string();
            }
        }, param);
        return stream;
    }

    template<class OStream>
    OStream& operator<<(OStream& stream, const scalar_expression_t* expr) {
        if (expr->params().empty()) {
            stream << expr->key();
        } else {
            if (!expr->key().is_null()) {
                stream << expr->key() << ": ";
            }
            if (expr->type() != scalar_type::get_field) {
                stream << "{$" << magic_enum::enum_name(expr->type()) << ": ";
            }
            if (expr->params().size() > 1) {
                stream << "[";
                bool is_first = true;
                for (const auto& param : expr->params()) {
                    if (is_first) {
                        is_first = false;
                    } else {
                        stream << ", ";
                    }
                    stream << param;
                }
                stream << "]";
            } else {
                stream << expr->params().at(0);
            }
            if (expr->type() != scalar_type::get_field) {
                stream << "}";
            }
        }
        return stream;
    }

    scalar_expression_t::scalar_expression_t(std::pmr::memory_resource *resource, scalar_type type, const key_t& key)
        : expression_i(expression_group::scalar)
        , type_(type)
        , key_(key)
        , params_(resource) {
    }

    scalar_type scalar_expression_t::type() const {
        return type_;
    }

    const key_t& scalar_expression_t::key() const {
        return key_;
    }

    const std::pmr::vector<scalar_expression_t::param_storage>& scalar_expression_t::params() const {
        return params_;
    }

    void scalar_expression_t::append_param(const scalar_expression_t::param_storage& param) {
        params_.push_back(param);
    }

    hash_t scalar_expression_t::hash_impl() const {
        hash_t hash_{0};
        boost::hash_combine(hash_, type_);
        boost::hash_combine(hash_, key_.hash());
        for (const auto& param : params_) {
            auto param_hash = std::visit([](const auto& value) {
                using param_type = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<param_type, core::parameter_id_t>) {
                    return std::hash<uint64_t>()(value);
                } else if constexpr (std::is_same_v<param_type, key_t>) {
                    return value.hash();
                } else if constexpr (std::is_same_v<param_type, expression_ptr>) {
                    return value->hash();
                }
            }, param);
            boost::hash_combine(hash_, param_hash);
        }
        return hash_;
    }

    std::string scalar_expression_t::to_string_impl() const {
        std::stringstream stream;
        stream << this;
        return stream.str();
    }

    bool scalar_expression_t::equal_impl(const expression_i* rhs) const {
        auto *other = static_cast<const scalar_expression_t*>(rhs);
        return type_ == other->type_ &&
               key_ == other->key_ &&
               params_.size() == other->params_.size() &&
               std::equal(params_.begin(), params_.end(), other->params_.begin());
    }


    scalar_expression_ptr make_scalar_expression(std::pmr::memory_resource *resource, scalar_type type, const key_t& key) {
        return new scalar_expression_t(resource, type, key);
    }

    scalar_expression_ptr make_scalar_expression(std::pmr::memory_resource *resource, scalar_type type) {
        return make_scalar_expression(resource, type, key_t{});
    }

    scalar_expression_ptr make_scalar_expression(std::pmr::memory_resource *resource, scalar_type type, const key_t& key, const key_t& field) {
        auto expr = make_scalar_expression(resource, type, key);
        expr->append_param(field);
        return expr;
    }

    scalar_type get_scalar_type(const std::string& key) {
        auto type = magic_enum::enum_cast<scalar_type>(key);
        if (type.has_value()) {
            return type.value();
        }
        return scalar_type::invalid;
    }

    bool is_scalar_type(const std::string& key) {
        return magic_enum::enum_cast<scalar_type>(key).has_value();
    }

} // namespace components::expressions