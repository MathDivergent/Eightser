#ifndef EIGHTSER_TESTING_BASE_HPP
#define EIGHTSER_TESTING_BASE_HPP

#ifndef EIGHTSER_RTTI_ENABLE
    #define EIGHTSER_TYPE_HASH(type) -1 // mock
    #define EIGHTSER_EXPRESSION_HASH(expression) -1 // mock
#endif // EIGHTSER_RTTI_ENABLE

#include <Eightser/Core.hpp>
#include <Eightest/Core.hpp>

using eightser::oarchive;
using eightser::iarchive;

using eightser::hierarchy;

using eightser::instantiable_t;

#endif // EIGHTSER_TESTING_BASE_HPP
