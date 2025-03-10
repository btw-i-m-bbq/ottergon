#pragma once

#include <components/session/session.hpp>

namespace services::database {

    using components::session::session_id_t;
    class database_t;
    using database_ptr = std::unique_ptr<database_t>;

} // namespace services::database