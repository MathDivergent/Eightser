#ifndef EIGHTSER_STANDARD_COMPLEX_HPP
#define EIGHTSER_STANDARD_COMPLEX_HPP

#include <complex> // complex

#include <Eightser/Serializable.hpp>

TEMPLATE_SERIALIZABLE_DECLARATION(template <typename ValueType>, std::complex<ValueType>)
SERIALIZABLE_DECLARATION_INIT()

TEMPLATE_SERIALIZABLE_SAVE(complex, template <typename ValueType>, std::complex<ValueType>)
    BIN_SERIALIZABLE
    (
        auto re = complex.real();
        auto im = complex.imag();

        archive & re & im;
    )
SERIALIZABLE_INIT()

TEMPLATE_SERIALIZABLE_LOAD(complex, template <typename ValueType>, std::complex<ValueType>)
    BIN_SERIALIZABLE
    (
        ValueType re{};
        ValueType im{};

        archive & re & im;

        complex.real(re);
        complex.imag(im);
    )
SERIALIZABLE_INIT()

#endif // EIGHTSER_STANDARD_COMPLEX_HPP
