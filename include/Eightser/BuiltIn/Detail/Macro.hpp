#ifndef EIGHTSER_BUILTIN_DETAIL_MACRO_HPP
#define EIGHTSER_BUILTIN_DETAIL_MACRO_HPP

#define EIGHTSER_BITPACK(common_fields_type, archive, ...) \
    { \
        auto xxbitpack = ::eightser::bitpack<common_fields_type>(archive); \
        EIGHTSER_CONCAT(EIGHTSER_BITPACK_, EIGHTSER_VA_ARGS_SIZE(__VA_ARGS__))(__VA_ARGS__) \
    }

#define EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_FIRST_ARGUMENT field_and_bits = xxbitpack field_and_bits;
#define EIGHTSER_BITPACK_1(field_and_bits) EIGHTSER_BITFIELD(field_and_bits)
#define EIGHTSER_BITPACK_2(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_1(__VA_ARGS__)
#define EIGHTSER_BITPACK_3(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_2(__VA_ARGS__)
#define EIGHTSER_BITPACK_4(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_3(__VA_ARGS__)
#define EIGHTSER_BITPACK_5(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_4(__VA_ARGS__)
#define EIGHTSER_BITPACK_6(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_5(__VA_ARGS__)
#define EIGHTSER_BITPACK_7(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_6(__VA_ARGS__)
#define EIGHTSER_BITPACK_8(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_7(__VA_ARGS__)

#define EIGHTSER_BITPACK_9(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_8(__VA_ARGS__)
#define EIGHTSER_BITPACK_10(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_9(__VA_ARGS__)
#define EIGHTSER_BITPACK_11(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_10(__VA_ARGS__)
#define EIGHTSER_BITPACK_12(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_11(__VA_ARGS__)
#define EIGHTSER_BITPACK_13(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_12(__VA_ARGS__)
#define EIGHTSER_BITPACK_14(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_13(__VA_ARGS__)
#define EIGHTSER_BITPACK_15(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_14(__VA_ARGS__)
#define EIGHTSER_BITPACK_16(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_15(__VA_ARGS__)

#define EIGHTSER_BITPACK_17(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_16(__VA_ARGS__)
#define EIGHTSER_BITPACK_18(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_17(__VA_ARGS__)
#define EIGHTSER_BITPACK_19(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_18(__VA_ARGS__)
#define EIGHTSER_BITPACK_20(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_19(__VA_ARGS__)
#define EIGHTSER_BITPACK_21(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_20(__VA_ARGS__)
#define EIGHTSER_BITPACK_22(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_21(__VA_ARGS__)
#define EIGHTSER_BITPACK_23(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_22(__VA_ARGS__)
#define EIGHTSER_BITPACK_24(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_23(__VA_ARGS__)

#define EIGHTSER_BITPACK_25(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_24(__VA_ARGS__)
#define EIGHTSER_BITPACK_26(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_25(__VA_ARGS__)
#define EIGHTSER_BITPACK_27(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_26(__VA_ARGS__)
#define EIGHTSER_BITPACK_28(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_27(__VA_ARGS__)
#define EIGHTSER_BITPACK_29(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_28(__VA_ARGS__)
#define EIGHTSER_BITPACK_30(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_29(__VA_ARGS__)
#define EIGHTSER_BITPACK_31(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_30(__VA_ARGS__)
#define EIGHTSER_BITPACK_32(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_31(__VA_ARGS__)

#define EIGHTSER_BITPACK_33(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_32(__VA_ARGS__)
#define EIGHTSER_BITPACK_34(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_33(__VA_ARGS__)
#define EIGHTSER_BITPACK_35(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_34(__VA_ARGS__)
#define EIGHTSER_BITPACK_36(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_35(__VA_ARGS__)
#define EIGHTSER_BITPACK_37(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_36(__VA_ARGS__)
#define EIGHTSER_BITPACK_38(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_37(__VA_ARGS__)
#define EIGHTSER_BITPACK_39(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_38(__VA_ARGS__)
#define EIGHTSER_BITPACK_40(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_39(__VA_ARGS__)

#define EIGHTSER_BITPACK_41(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_40(__VA_ARGS__)
#define EIGHTSER_BITPACK_42(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_41(__VA_ARGS__)
#define EIGHTSER_BITPACK_43(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_42(__VA_ARGS__)
#define EIGHTSER_BITPACK_44(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_43(__VA_ARGS__)
#define EIGHTSER_BITPACK_45(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_44(__VA_ARGS__)
#define EIGHTSER_BITPACK_46(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_45(__VA_ARGS__)
#define EIGHTSER_BITPACK_47(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_46(__VA_ARGS__)
#define EIGHTSER_BITPACK_48(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_47(__VA_ARGS__)

#define EIGHTSER_BITPACK_49(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_48(__VA_ARGS__)
#define EIGHTSER_BITPACK_50(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_49(__VA_ARGS__)
#define EIGHTSER_BITPACK_51(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_50(__VA_ARGS__)
#define EIGHTSER_BITPACK_52(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_51(__VA_ARGS__)
#define EIGHTSER_BITPACK_53(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_52(__VA_ARGS__)
#define EIGHTSER_BITPACK_54(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_53(__VA_ARGS__)
#define EIGHTSER_BITPACK_55(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_54(__VA_ARGS__)
#define EIGHTSER_BITPACK_56(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_55(__VA_ARGS__)

#define EIGHTSER_BITPACK_57(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_56(__VA_ARGS__)
#define EIGHTSER_BITPACK_58(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_57(__VA_ARGS__)
#define EIGHTSER_BITPACK_59(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_58(__VA_ARGS__)
#define EIGHTSER_BITPACK_60(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_59(__VA_ARGS__)
#define EIGHTSER_BITPACK_61(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_60(__VA_ARGS__)
#define EIGHTSER_BITPACK_62(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_61(__VA_ARGS__)
#define EIGHTSER_BITPACK_63(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_62(__VA_ARGS__)
#define EIGHTSER_BITPACK_64(field_and_bits, ...) EIGHTSER_BITFIELD(field_and_bits) EIGHTSER_BITPACK_63(__VA_ARGS__)
// and etc.

#endif // EIGHTSER_BUILTIN_DETAIL_MACRO_HPP
