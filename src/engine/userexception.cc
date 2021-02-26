#include "engine/userexception.h"

#include <stdexcept>
#include <string>

namespace simple
{
    UserException::UserException(const std::string& message)
        : std::runtime_error(message){};
}
