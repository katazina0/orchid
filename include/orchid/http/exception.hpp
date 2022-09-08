#pragma once

#include <exception>

namespace orchid::http
{
    class InvalidRequestException : std::exception
    {

    };
}