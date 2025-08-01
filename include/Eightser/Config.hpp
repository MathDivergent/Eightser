#ifndef EIGHTSER_CONFIG_HPP
#define EIGHTSER_CONFIG_HPP

namespace eightser
{

#ifdef EIGHTSER_TYPE_PORTABLE_ENABLE

static_assert(sizeof(char) == 1, "Require character size.");

static_assert(sizeof(float) == 4, "Require floating point number size.");
static_assert(sizeof(double) == 8, "Require double precision floation point number size.");

static_assert(sizeof(int) == 4, "Require integer number size.");

#endif // EIGHTSER_TYPE_PORTABLE_ENABLE

} // namespace eightser

#endif // EIGHTSER_CONFIG_HPP
