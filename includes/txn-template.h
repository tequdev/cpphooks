#include "macro.h"

// clang-format off
#define DEFINE_MULTIPLE_FIELD_INDICEX_8(X)                                    \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)

#define DEFINE_MULTIPLE_FIELD_INDICEX_10(X)                                    \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10)

#define DEFINE_MULTIPLE_FIELD_INDICEX_16(X)                                    \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14)   \
  X(15) X(16)

#define DEFINE_MULTIPLE_FIELD_INDICEX_32(X)                                    \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14)   \
  X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27)\
  X(28) X(29) X(30) X(31) X(32)

#define DEFINE_MULTIPLE_FIELD_INDICEX_64(X)                                    \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14)   \
  X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27)\
  X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) \
  X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) \
  X(54) X(55) X(56) X(57) X(58) X(59) X(60) X(61) X(62) X(63) X(64)

#define DEFINE_MULTIPLE_FIELD_INDICEX_512(X)                                   \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14)   \
  X(15) X(16) X(17) X(18) X(19) X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27)\
  X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) X(40) \
  X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) X(50) X(51) X(52) X(53) \
  X(54) X(55) X(56) X(57) X(58) X(59) X(60) X(61) X(62) X(63) X(64) X(65) X(66) X(67)\
  X(68) X(69) X(70) X(71) X(72) X(73) X(74) X(75) X(76) X(77) X(78) X(79) X(80) \
  X(81) X(82) X(83) X(84) X(85) X(86) X(87) X(88) X(89) X(90) X(91) X(92) X(93) \
  X(94) X(95) X(96) X(97) X(98) X(99) X(100) X(101) X(102) X(103) X(104) X(105) \
  X(106) X(107) X(108) X(109) X(110) X(111) X(112) X(113) X(114) X(115) X(116) X(117) \
  X(118) X(119) X(120) X(121) X(122) X(123) X(124) X(125) X(126) X(127) X(128)

// clang-format on

enum SerializedTypeID {
  // special types
  STI_UNKNOWN = -2,
  STI_NOTPRESENT = 0,

  // types (common)
  STI_UINT16 = 1,
  STI_UINT32 = 2,
  STI_UINT64 = 3,
  STI_UINT128 = 4,
  STI_UINT256 = 5,
  STI_AMOUNT = 6,
  STI_VL = 7,
  STI_ACCOUNT = 8,
  // 9-13 are reserved
  STI_OBJECT = 14,
  STI_ARRAY = 15,

  // types (uncommon)
  STI_UINT8 = 16,
  STI_UINT160 = 17,
  STI_PATHSET = 18,
  STI_VECTOR256 = 19,
  STI_UINT96 = 20,
  STI_UINT192 = 21,
  STI_UINT384 = 22,
  STI_UINT512 = 23,

  // high level types
  // cannot be serialized inside other types
  STI_TRANSACTION = 10001,
  STI_LEDGERENTRY = 10002,
  STI_VALIDATION = 10003,
  STI_METADATA = 10004,
};

#define FIELD_CODE(sfcode) (sfcode & 0xFFFF)
#define TYPE_CODE(sfcode) (sfcode >> 16)

constexpr uint16_t field_size_by_sto(uint32_t sfcode) {
  switch (TYPE_CODE(sfcode)) {
  case STI_UINT16:
    return 2;
  case STI_UINT32:
    return 4;
  case STI_UINT64:
    return 8;
  case STI_UINT128:
    return 16;
  case STI_UINT256:
    return 32;
  case STI_AMOUNT:
    return 48;
  case STI_VL: {
    switch (sfcode) {
    case sfSigningPubKey:
      return 33;
    default:
      // TODO
      return 1;
    }
  }
  case STI_ACCOUNT:
    return 20;
  case STI_OBJECT: {
    switch (sfcode) {
    case sfEmitDetails:
      return 116 - 3;
    default:
      // TODO
      return 1;
    }
  }
  case STI_ARRAY:
    // TODO
    return 1;
  case STI_UINT8:
    return 1;
  case STI_UINT160:
    return 20;
  case STI_PATHSET:
    // TODO
    return 1;
  case STI_VECTOR256:
    // TODO
    return 32;
  case STI_UINT96:
    return 12;
  case STI_UINT192:
    return 24;
  case STI_UINT384:
    return 48;
  case STI_UINT512:
    return 64;
  default:
    return 0;
  }
}

template <uint32_t sfcode, uint16_t SIZE = 0> struct TemplateField {};
// template <uint32_t sfcode> struct Field {};
template <uint32_t sfcode, uint16_t SIZE = 0> struct OptionalTemplateField {};
// template <uint32_t sfcode> struct OptionalField {};

#define CODE_SIZE(sfcode)                                                      \
  (1 + (TYPE_CODE(sfcode) >= 16 ? 1 : 0) + (FIELD_CODE(sfcode) >= 16 ? 1 : 0))

#define CODE_VALUE_1_1(sfcode)                                                 \
  { TYPE_CODE(sfcode) << 4 | FIELD_CODE(sfcode) }
#define CODE_VALUE_1_2(sfcode)                                                 \
  { TYPE_CODE(sfcode) << 4, FIELD_CODE(sfcode) }
#define CODE_VALUE_2_1(sfcode)                                                 \
  { FIELD_CODE(sfcode), TYPE_CODE(sfcode) }
#define CODE_VALUE_2_2(sfcode)                                                 \
  { 0x00, TYPE_CODE(sfcode), FIELD_CODE(sfcode) }

// TODO: Optional fields
#define CODE_OPTIONAL_VALUE_1_1(sfcode)                                        \
  { 0x99 }
#define CODE_OPTIONAL_VALUE_1_2(sfcode)                                        \
  { 0x99, 0x99 }
#define CODE_OPTIONAL_VALUE_2_1(sfcode)                                        \
  { 0x99, 0x99 }
#define CODE_OPTIONAL_VALUE_2_2(sfcode)                                        \
  { 0x99, 0x99, 0x99 }

// #define DEFINE_XXX_FIELD(sfcode, TYPE)                                         \
//   template <> struct TemplateField<sfcode> {                                           \
//     uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
//     uint8_t value[field_size_by_sto(sfcode)];                                  \
//   };                                                                           \
//   template <> struct OptionalTemplateField<sfcode> {                                   \
//     uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_2_##TYPE(sfcode);    \
//     uint8_t value[field_size_by_sto(sfcode)] = {[0 ... (field_size_by_sto(sfcode)-1)] = 0x99};                   \
//   };

#define DEFINE_UINT8_FIELD(sfcode, TYPE)                                       \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_2_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_UINT16_FIELD(sfcode, TYPE)                                      \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_UINT32_FIELD(sfcode, TYPE)                                      \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
    void setUint32(uint32_t value) {                                           \
      *(uint32_t *)(this->value) = FLIP_ENDIAN_32(value);                      \
    }                                                                          \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
    void setUint32(uint32_t value) {                                           \
      *(uint32_t *)(this->value) = FLIP_ENDIAN_32(value);                      \
    }                                                                          \
  };

#define DEFINE_UINT64_FIELD(sfcode, TYPE)                                      \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
    void setUint64(uint64_t value) {                                           \
      *(uint64_t *)(this->value) = FLIP_ENDIAN_64(value);                      \
    }                                                                          \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
    void setUint64(uint64_t value) {                                           \
      *(uint64_t *)(this->value) = FLIP_ENDIAN_64(value);                      \
    }                                                                          \
  };

#define DEFINE_UINT128_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_UINT256_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

// TODO: issuer, currency, value, setNativeAmount, setIOUAmount
#define DEFINE_AMOUNT_FIELD(sfcode, TYPE)                                      \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_NATIVE_AMOUNT_FIELD(sfcode, TYPE)                               \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[8] = {0x40};                                                 \
    void setDrops(uint64_t drops) {                                            \
      uint8_t *b = value;                                                      \
      *b++ = 0b01000000 + ((drops >> 56) & 0b00111111);                        \
      *b++ = (drops >> 48) & 0xFFU;                                            \
      *b++ = (drops >> 40) & 0xFFU;                                            \
      *b++ = (drops >> 32) & 0xFFU;                                            \
      *b++ = (drops >> 24) & 0xFFU;                                            \
      *b++ = (drops >> 16) & 0xFFU;                                            \
      *b++ = (drops >> 8) & 0xFFU;                                             \
      *b++ = (drops >> 0) & 0xFFU;                                             \
    }                                                                          \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
    void setDrops(uint64_t drops) {                                            \
      uint8_t *b = value;                                                      \
      *b++ = 0b01000000 + ((drops >> 56) & 0b00111111);                        \
      *b++ = (drops >> 48) & 0xFFU;                                            \
      *b++ = (drops >> 40) & 0xFFU;                                            \
      *b++ = (drops >> 32) & 0xFFU;                                            \
      *b++ = (drops >> 24) & 0xFFU;                                            \
      *b++ = (drops >> 16) & 0xFFU;                                            \
      *b++ = (drops >> 8) & 0xFFU;                                             \
      *b++ = (drops >> 0) & 0xFFU;                                             \
    }                                                                          \
  };

// TODO: length
#define DEFINE_VL_FIELD(sfcode, TYPE)                                          \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t length[1] = {field_size_by_sto(sfcode)};                           \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t length[1] = {0x99};                                                \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
      length[0] = 0x00;                                                        \
    }                                                                          \
  };

#define DEFINE_ACCOUNT_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t length[1] = {0x14};                                                \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t length[1] = {0x99};                                                \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
      length[0] = 0x14;                                                        \
    }                                                                          \
  };

// TODO: value
#define DEFINE_OBJECT_FIELD(sfcode, TYPE)                                      \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t prefix[1] = {0x7c};                                                \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
    uint8_t postfix[1] = {0xe1};                                               \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t prefix[1] = {0x99};                                                \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    uint8_t postfix[1] = {0x99};                                               \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
      prefix[0] = 0x7c;                                                        \
      postfix[0] = 0xe1;                                                       \
    }                                                                          \
  };

// TODO: value
#define DEFINE_ARRAY_FIELD(sfcode, TYPE, InnerFieldCode, SIZE)                 \
  template <> struct TemplateField<sfcode, SIZE> {                             \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t prefix[1] = {0xf4};                                                \
    TemplateField<InnerFieldCode> values[SIZE];                                \
    uint8_t postfix[1] = {0xf1};                                               \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode, SIZE> {                     \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t prefix[1] = {0x99};                                                \
    OptionalTemplateField<InnerFieldCode> values[SIZE];                        \
    uint8_t postfix[1] = {0x99};                                               \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
      prefix[0] = 0xf4;                                                        \
      postfix[0] = 0xf1;                                                       \
    }                                                                          \
  };

// TODO: issuer, currency?
#define DEFINE_UINT160_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode> {                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_2_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

// TODO:
#define DEFINE_PATHSET_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

// TODO: length
// SIZE 1~6 (32~192)
#define DEFINE_VECTOR256_FIELD_1(sfcode, TYPE, SIZE)                           \
  template <> struct TemplateField<sfcode, SIZE> {                             \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t length[1] = {SIZE * 32};                                           \
    uint8_t value[SIZE][32];                                                   \
    void setValue(uint8_t index, uint8_t *newValue) {                          \
      unsigned char *buf = (unsigned char *)value[index];                      \
      *(uint64_t *)(buf + 0) = *(uint64_t *)(newValue + 0);                    \
      *(uint64_t *)(buf + 8) = *(uint64_t *)(newValue + 8);                    \
      *(uint64_t *)(buf + 16) = *(uint64_t *)(newValue + 16);                  \
      *(uint64_t *)(buf + 24) = *(uint64_t *)(newValue + 24);                  \
    }                                                                          \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode, SIZE> {                     \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_2_##TYPE(sfcode);    \
    uint8_t length[1] = {SIZE * 32};                                           \
    uint8_t value[SIZE][32] = {[0 ...(SIZE - 1)] = {[0 ...(32 - 1)] = 0x99}};  \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
      length[0] = SIZE * 32;                                                   \
    }                                                                          \
    void setValue(uint8_t index, uint8_t *newValue) {                          \
      unsigned char *buf = (unsigned char *)value[index];                      \
      *(uint64_t *)(buf + 0) = *(uint64_t *)(newValue + 0);                    \
      *(uint64_t *)(buf + 8) = *(uint64_t *)(newValue + 8);                    \
      *(uint64_t *)(buf + 16) = *(uint64_t *)(newValue + 16);                  \
      *(uint64_t *)(buf + 24) = *(uint64_t *)(newValue + 24);                  \
    }                                                                          \
  };

// SIZE 7~390 (193~12480)
#define DEFINE_VECTOR256_FIELD_2(sfcode, TYPE, SIZE)                           \
  template <> struct TemplateField<sfcode, SIZE> {                             \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t length[2] = {                                                      \
        193 + static_cast<unsigned char>((SIZE * 32 - 193) >> 8),              \
        (SIZE * 32 - 193) & 0xff};                                             \
    uint8_t value[SIZE][32];                                                   \
    void setValue(uint8_t index, uint8_t *newValue) {                          \
      unsigned char *buf = (unsigned char *)value[index];                      \
      *(uint64_t *)(buf + 0) = *(uint64_t *)(newValue + 0);                    \
      *(uint64_t *)(buf + 8) = *(uint64_t *)(newValue + 8);                    \
      *(uint64_t *)(buf + 16) = *(uint64_t *)(newValue + 16);                  \
      *(uint64_t *)(buf + 24) = *(uint64_t *)(newValue + 24);                  \
    }                                                                          \
  };                                                                           \
  template <> struct OptionalTemplateField<sfcode, SIZE> {                     \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_2_##TYPE(sfcode);    \
    uint8_t length[2] = {0x99, 0x99};                                          \
    uint8_t value[SIZE][32] = {[0 ...(SIZE - 1)] = {[0 ...(32 - 1)] = 0x99}};  \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
      length[0] = 193 + static_cast<unsigned char>((SIZE * 32 - 193) >> 8);    \
      length[1] = (SIZE * 32 - 193) & 0xff;                                    \
    }                                                                          \
    void setValue(uint8_t index, uint8_t *newValue) {                          \
      unsigned char *buf = (unsigned char *)value[index];                      \
      *(uint64_t *)(buf + 0) = *(uint64_t *)(newValue + 0);                    \
      *(uint64_t *)(buf + 8) = *(uint64_t *)(newValue + 8);                    \
      *(uint64_t *)(buf + 16) = *(uint64_t *)(newValue + 16);                  \
      *(uint64_t *)(buf + 24) = *(uint64_t *)(newValue + 24);                  \
    }                                                                          \
  };

#define DEFINE_UINT96_FIELD(sfcode, TYPE)                                      \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

#define DEFINE_UINT192_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

#define DEFINE_UINT384_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

#define DEFINE_UINT512_FIELD(sfcode, TYPE)                                     \
  template <> struct TemplateField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

DEFINE_UINT8_FIELD(sfCloseResolution, 1)
DEFINE_UINT8_FIELD(sfMethod, 1)
DEFINE_UINT8_FIELD(sfTransactionResult, 1)
DEFINE_UINT8_FIELD(sfTickSize, 2)
DEFINE_UINT8_FIELD(sfUNLModifyDisabling, 2)
DEFINE_UINT8_FIELD(sfHookResult, 2)

DEFINE_UINT16_FIELD(sfLedgerEntryType, 1)
DEFINE_UINT16_FIELD(sfTransactionType, 1)
DEFINE_UINT16_FIELD(sfSignerWeight, 1)
DEFINE_UINT16_FIELD(sfTransferFee, 1)
DEFINE_UINT16_FIELD(sfVersion, 2)
DEFINE_UINT16_FIELD(sfHookStateChangeCount, 2)
DEFINE_UINT16_FIELD(sfHookEmitCount, 2)
DEFINE_UINT16_FIELD(sfHookExecutionIndex, 2)
DEFINE_UINT16_FIELD(sfHookApiVersion, 2)

DEFINE_UINT32_FIELD(sfNetworkID, 1)
DEFINE_UINT32_FIELD(sfFlags, 1)
DEFINE_UINT32_FIELD(sfSourceTag, 1)
DEFINE_UINT32_FIELD(sfSequence, 1)
DEFINE_UINT32_FIELD(sfPreviousTxnLgrSeq, 1)
DEFINE_UINT32_FIELD(sfLedgerSequence, 1)
DEFINE_UINT32_FIELD(sfCloseTime, 1)
DEFINE_UINT32_FIELD(sfParentCloseTime, 1)
DEFINE_UINT32_FIELD(sfSigningTime, 1)
DEFINE_UINT32_FIELD(sfExpiration, 1)
DEFINE_UINT32_FIELD(sfTransferRate, 1)
DEFINE_UINT32_FIELD(sfWalletSize, 1)
DEFINE_UINT32_FIELD(sfOwnerCount, 1)
DEFINE_UINT32_FIELD(sfDestinationTag, 1)
DEFINE_UINT32_FIELD(sfHighQualityIn, 2)
DEFINE_UINT32_FIELD(sfHighQualityOut, 2)
DEFINE_UINT32_FIELD(sfLowQualityIn, 2)
DEFINE_UINT32_FIELD(sfLowQualityOut, 2)
DEFINE_UINT32_FIELD(sfQualityIn, 2)
DEFINE_UINT32_FIELD(sfQualityOut, 2)
DEFINE_UINT32_FIELD(sfStampEscrow, 2)
DEFINE_UINT32_FIELD(sfBondAmount, 2)
DEFINE_UINT32_FIELD(sfLoadFee, 2)
DEFINE_UINT32_FIELD(sfOfferSequence, 2)
DEFINE_UINT32_FIELD(sfFirstLedgerSequence, 2)
DEFINE_UINT32_FIELD(sfLastLedgerSequence, 2)
DEFINE_UINT32_FIELD(sfTransactionIndex, 2)
DEFINE_UINT32_FIELD(sfOperationLimit, 2)
DEFINE_UINT32_FIELD(sfReferenceFeeUnits, 2)
DEFINE_UINT32_FIELD(sfReserveBase, 2)
DEFINE_UINT32_FIELD(sfReserveIncrement, 2)
DEFINE_UINT32_FIELD(sfSetFlag, 2)
DEFINE_UINT32_FIELD(sfClearFlag, 2)
DEFINE_UINT32_FIELD(sfSignerQuorum, 2)
DEFINE_UINT32_FIELD(sfCancelAfter, 2)
DEFINE_UINT32_FIELD(sfFinishAfter, 2)
DEFINE_UINT32_FIELD(sfSignerListID, 2)
DEFINE_UINT32_FIELD(sfSettleDelay, 2)
DEFINE_UINT32_FIELD(sfTicketCount, 2)
DEFINE_UINT32_FIELD(sfTicketSequence, 2)
DEFINE_UINT32_FIELD(sfNFTokenTaxon, 2)
DEFINE_UINT32_FIELD(sfMintedNFTokens, 2)
DEFINE_UINT32_FIELD(sfBurnedNFTokens, 2)
DEFINE_UINT32_FIELD(sfHookStateCount, 2)
DEFINE_UINT32_FIELD(sfEmitGeneration, 2)
DEFINE_UINT32_FIELD(sfLockCount, 2)
DEFINE_UINT32_FIELD(sfFirstNFTokenSequence, 2)
DEFINE_UINT32_FIELD(sfXahauActivationLgrSeq, 2)
DEFINE_UINT32_FIELD(sfImportSequence, 2)
DEFINE_UINT32_FIELD(sfRewardTime, 2)
DEFINE_UINT32_FIELD(sfRewardLgrFirst, 2)
DEFINE_UINT32_FIELD(sfRewardLgrLast, 2)

DEFINE_UINT64_FIELD(sfIndexNext, 1)
DEFINE_UINT64_FIELD(sfIndexPrevious, 1)
DEFINE_UINT64_FIELD(sfBookNode, 1)
DEFINE_UINT64_FIELD(sfOwnerNode, 1)
DEFINE_UINT64_FIELD(sfBaseFee, 1)
DEFINE_UINT64_FIELD(sfExchangeRate, 1)
DEFINE_UINT64_FIELD(sfLowNode, 1)
DEFINE_UINT64_FIELD(sfHighNode, 1)
DEFINE_UINT64_FIELD(sfDestinationNode, 1)
DEFINE_UINT64_FIELD(sfCookie, 1)
DEFINE_UINT64_FIELD(sfServerVersion, 1)
DEFINE_UINT64_FIELD(sfNFTokenOfferNode, 1)
DEFINE_UINT64_FIELD(sfEmitBurden, 1)

DEFINE_UINT64_FIELD(sfHookInstructionCount, 2)
DEFINE_UINT64_FIELD(sfHookReturnCode, 2)
DEFINE_UINT64_FIELD(sfReferenceCount, 2)
DEFINE_UINT64_FIELD(sfAccountIndex, 2)
DEFINE_UINT64_FIELD(sfAccountCount, 2)
DEFINE_UINT64_FIELD(sfRewardAccumulator, 2)

DEFINE_UINT128_FIELD(sfEmailHash, 1)

DEFINE_UINT160_FIELD(sfTakerPaysCurrency, 1)
DEFINE_UINT160_FIELD(sfTakerPaysIssuer, 1)
DEFINE_UINT160_FIELD(sfTakerGetsCurrency, 1)
DEFINE_UINT160_FIELD(sfTakerGetsIssuer, 1)

DEFINE_UINT256_FIELD(sfLedgerHash, 1)
DEFINE_UINT256_FIELD(sfParentHash, 1)
DEFINE_UINT256_FIELD(sfTransactionHash, 1)
DEFINE_UINT256_FIELD(sfAccountHash, 1)
DEFINE_UINT256_FIELD(sfPreviousTxnID, 1)
DEFINE_UINT256_FIELD(sfLedgerIndex, 1)
DEFINE_UINT256_FIELD(sfWalletLocator, 1)
DEFINE_UINT256_FIELD(sfRootIndex, 1)
DEFINE_UINT256_FIELD(sfAccountTxnID, 1)
DEFINE_UINT256_FIELD(sfNFTokenID, 1)
DEFINE_UINT256_FIELD(sfEmitParentTxnID, 1)
DEFINE_UINT256_FIELD(sfEmitNonce, 1)
DEFINE_UINT256_FIELD(sfEmitHookHash, 1)

DEFINE_UINT256_FIELD(sfBookDirectory, 2)
DEFINE_UINT256_FIELD(sfInvoiceID, 2)
DEFINE_UINT256_FIELD(sfNickname, 2)
DEFINE_UINT256_FIELD(sfAmendment, 2)
DEFINE_UINT256_FIELD(sfHookOn, 2)
DEFINE_UINT256_FIELD(sfDigest, 2)
DEFINE_UINT256_FIELD(sfChannel, 2)
DEFINE_UINT256_FIELD(sfConsensusHash, 2)
DEFINE_UINT256_FIELD(sfCheckID, 2)
DEFINE_UINT256_FIELD(sfValidatedHash, 2)
DEFINE_UINT256_FIELD(sfPreviousPageMin, 2)
DEFINE_UINT256_FIELD(sfNextPageMin, 2)
DEFINE_UINT256_FIELD(sfNFTokenBuyOffer, 2)
DEFINE_UINT256_FIELD(sfNFTokenSellOffer, 2)
DEFINE_UINT256_FIELD(sfHookStateKey, 2)
DEFINE_UINT256_FIELD(sfHookHash, 2)
DEFINE_UINT256_FIELD(sfHookNamespace, 2)
DEFINE_UINT256_FIELD(sfHookSetTxnID, 2)
DEFINE_UINT256_FIELD(sfOfferID, 2)
DEFINE_UINT256_FIELD(sfEscrowID, 2)
DEFINE_UINT256_FIELD(sfURITokenID, 2)
DEFINE_UINT256_FIELD(sfGovernanceFlags, 2)
DEFINE_UINT256_FIELD(sfGovernanceMarks, 2)
DEFINE_UINT256_FIELD(sfEmittedTxnID, 2)

DEFINE_AMOUNT_FIELD(sfAmount, 1)
DEFINE_AMOUNT_FIELD(sfBalance, 1)
DEFINE_AMOUNT_FIELD(sfLimitAmount, 1)
DEFINE_AMOUNT_FIELD(sfTakerPays, 1)
DEFINE_AMOUNT_FIELD(sfTakerGets, 1)
DEFINE_AMOUNT_FIELD(sfLowLimit, 1)
DEFINE_AMOUNT_FIELD(sfHighLimit, 1)
DEFINE_NATIVE_AMOUNT_FIELD(sfFee, 1)
DEFINE_AMOUNT_FIELD(sfSendMax, 1)
DEFINE_AMOUNT_FIELD(sfDeliverMin, 1)
DEFINE_AMOUNT_FIELD(sfMinimumOffer, 2)
DEFINE_AMOUNT_FIELD(sfRippleEscrow, 2)
DEFINE_AMOUNT_FIELD(sfDeliveredAmount, 2)
DEFINE_AMOUNT_FIELD(sfNFTokenBrokerFee, 2)
DEFINE_AMOUNT_FIELD(sfHookCallbackFee, 2)
DEFINE_AMOUNT_FIELD(sfLockedBalance, 2)
DEFINE_AMOUNT_FIELD(sfBaseFeeDrops, 2)
DEFINE_AMOUNT_FIELD(sfReserveBaseDrops, 2)
DEFINE_AMOUNT_FIELD(sfReserveIncrementDrops, 2)

DEFINE_VL_FIELD(sfPublicKey, 1)
DEFINE_VL_FIELD(sfMessageKey, 1)
DEFINE_VL_FIELD(sfSigningPubKey, 1)
DEFINE_VL_FIELD(sfTxnSignature, 1)
DEFINE_VL_FIELD(sfURI, 1)
DEFINE_VL_FIELD(sfSignature, 1)
DEFINE_VL_FIELD(sfDomain, 1)
DEFINE_VL_FIELD(sfFundCode, 1)
DEFINE_VL_FIELD(sfRemoveCode, 1)
DEFINE_VL_FIELD(sfExpireCode, 1)
DEFINE_VL_FIELD(sfCreateCode, 1)
DEFINE_VL_FIELD(sfMemoType, 1)
DEFINE_VL_FIELD(sfMemoData, 1)
DEFINE_VL_FIELD(sfMemoFormat, 1)
DEFINE_VL_FIELD(sfFulfillment, 2)
DEFINE_VL_FIELD(sfCondition, 2)
DEFINE_VL_FIELD(sfMasterSignature, 2)
DEFINE_VL_FIELD(sfUNLModifyValidator, 2)
DEFINE_VL_FIELD(sfValidatorToDisable, 2)
DEFINE_VL_FIELD(sfValidatorToReEnable, 2)
DEFINE_VL_FIELD(sfHookStateData, 2)
DEFINE_VL_FIELD(sfHookReturnString, 2)
DEFINE_VL_FIELD(sfHookParameterName, 2)
DEFINE_VL_FIELD(sfHookParameterValue, 2)
DEFINE_VL_FIELD(sfBlob, 2)

DEFINE_ACCOUNT_FIELD(sfAccount, 1)
DEFINE_ACCOUNT_FIELD(sfOwner, 1)
DEFINE_ACCOUNT_FIELD(sfDestination, 1)
DEFINE_ACCOUNT_FIELD(sfIssuer, 1)
DEFINE_ACCOUNT_FIELD(sfAuthorize, 1)
DEFINE_ACCOUNT_FIELD(sfUnauthorize, 1)
DEFINE_ACCOUNT_FIELD(sfRegularKey, 1)
DEFINE_ACCOUNT_FIELD(sfNFTokenMinter, 1)
DEFINE_ACCOUNT_FIELD(sfEmitCallback, 1)
DEFINE_ACCOUNT_FIELD(sfHookAccount, 2)
DEFINE_ACCOUNT_FIELD(sfInform, 2)

DEFINE_VECTOR256_FIELD_1(sfIndexes, 1, 1)
DEFINE_VECTOR256_FIELD_1(sfHashes, 1, 1)
DEFINE_VECTOR256_FIELD_1(sfAmendments, 1, 1)
DEFINE_VECTOR256_FIELD_1(sfNFTokenOffers, 1, 1)
DEFINE_VECTOR256_FIELD_1(sfHookNamespaces, 1, 1)

DEFINE_VECTOR256_FIELD_1(sfURITokenIDs, 2, 1)
DEFINE_VECTOR256_FIELD_1(sfURITokenIDs, 2, 2)
DEFINE_VECTOR256_FIELD_1(sfURITokenIDs, 2, 3)
DEFINE_VECTOR256_FIELD_1(sfURITokenIDs, 2, 4)
DEFINE_VECTOR256_FIELD_1(sfURITokenIDs, 2, 5)
DEFINE_VECTOR256_FIELD_1(sfURITokenIDs, 2, 6)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 7)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 8)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 9)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 10)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 11)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 12)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 13)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 14)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 15)
DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, 16)

// #define DEFINE_ONE_URI_TOKEN_IDS(idx)                                          \
//   DEFINE_VECTOR256_FIELD_2(sfURITokenIDs, 2, idx)
// DEFINE_MULTIPLE_FIELD_INDICEX_16(DEFINE_ONE_URI_TOKEN_IDS)

DEFINE_PATHSET_FIELD(sfPaths, 1)

DEFINE_OBJECT_FIELD(sfTransactionMetaData, 1)
DEFINE_OBJECT_FIELD(sfCreatedNode, 1)
DEFINE_OBJECT_FIELD(sfDeletedNode, 1)
DEFINE_OBJECT_FIELD(sfModifiedNode, 1)
DEFINE_OBJECT_FIELD(sfPreviousFields, 1)
DEFINE_OBJECT_FIELD(sfFinalFields, 1)
DEFINE_OBJECT_FIELD(sfNewFields, 1)
DEFINE_OBJECT_FIELD(sfTemplateEntry, 1)
DEFINE_OBJECT_FIELD(sfMemo, 1)
DEFINE_OBJECT_FIELD(sfSignerEntry, 1)
DEFINE_OBJECT_FIELD(sfNFToken, 1)
DEFINE_OBJECT_FIELD(sfEmitDetails, 1)
DEFINE_OBJECT_FIELD(sfHook, 1)
DEFINE_OBJECT_FIELD(sfSigner, 2)
DEFINE_OBJECT_FIELD(sfMajority, 2)
DEFINE_OBJECT_FIELD(sfDisabledValidator, 2)
DEFINE_OBJECT_FIELD(sfEmittedTxn, 2)
DEFINE_OBJECT_FIELD(sfHookExecution, 2)
DEFINE_OBJECT_FIELD(sfHookDefinition, 2)
DEFINE_OBJECT_FIELD(sfHookParameter, 2)
DEFINE_OBJECT_FIELD(sfHookGrant, 2)
DEFINE_OBJECT_FIELD(sfGenesisMint, 2)
DEFINE_OBJECT_FIELD(sfActiveValidator, 2)
DEFINE_OBJECT_FIELD(sfImportVLKey, 2)
DEFINE_OBJECT_FIELD(sfHookEmission, 2)
DEFINE_OBJECT_FIELD(sfMintURIToken, 2)
DEFINE_OBJECT_FIELD(sfAmountEntry, 2)

#define DEFINE_ONE_SIGNERS(idx) DEFINE_ARRAY_FIELD(sfSigners, 1, sfSigner, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_SIGNERS)
#define DEFINE_ONE_SIGNER_ENTRIES(idx)                                         \
  DEFINE_ARRAY_FIELD(sfSignerEntries, 1, sfSignerEntry, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_SIGNER_ENTRIES)
DEFINE_ARRAY_FIELD(sfTemplate, 1, sfTemplateEntry, 1)
// DEFINE_ARRAY_FIELD(sfNecessary, 1)
// DEFINE_ARRAY_FIELD(sfSufficient, 1)
// for sfModifiedNode, sfCreatedNode, sfDeletedNode
// DEFINE_ARRAY_FIELD(sfAffectedNodes, 1)
#define DEFINE_ONE_MEMOS(idx) DEFINE_ARRAY_FIELD(sfMemos, 1, sfMemo, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_16(DEFINE_ONE_MEMOS)
#define DEFINE_ONE_NFTOKENS(idx)                                               \
  DEFINE_ARRAY_FIELD(sfNFTokens, 1, sfNFToken, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_NFTOKENS)
#define DEFINE_ONE_HOOKS(idx) DEFINE_ARRAY_FIELD(sfHooks, 1, sfHook, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_10(DEFINE_ONE_HOOKS)
#define DEFINE_ONE_MAJORITIES(idx)                                             \
  DEFINE_ARRAY_FIELD(sfMajorities, 2, sfMajority, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_MAJORITIES)
#define DEFINE_ONE_DISABLED_VALIDATORS(idx)                                    \
  DEFINE_ARRAY_FIELD(sfDisabledValidators, 2, sfDisabledValidator, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_DISABLED_VALIDATORS)
#define DEFINE_ONE_HOOK_EXECUTIONS(idx)                                        \
  DEFINE_ARRAY_FIELD(sfHookExecutions, 2, sfHookExecution, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_64(DEFINE_ONE_HOOK_EXECUTIONS)
#define DEFINE_ONE_HOOK_PARAMETERS(idx)                                        \
  DEFINE_ARRAY_FIELD(sfHookParameters, 2, sfHookParameter, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_16(DEFINE_ONE_HOOK_PARAMETERS)
#define DEFINE_ONE_HOOK_GRANTS(idx)                                            \
  DEFINE_ARRAY_FIELD(sfHookGrants, 2, sfHookGrant, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_8(DEFINE_ONE_HOOK_GRANTS)
#define DEFINE_ONE_GENESIS_MINTS(idx)                                          \
  DEFINE_ARRAY_FIELD(sfGenesisMints, 2, sfGenesisMint, idx)
// MAX: 512
DEFINE_MULTIPLE_FIELD_INDICEX_64(DEFINE_ONE_GENESIS_MINTS)
#define DEFINE_ONE_ACTIVE_VALIDATORS(idx)                                      \
  DEFINE_ARRAY_FIELD(sfActiveValidators, 2, sfActiveValidator, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_ACTIVE_VALIDATORS)
#define DEFINE_ONE_IMPORT_VL_KEYS(idx)                                         \
  DEFINE_ARRAY_FIELD(sfImportVLKeys, 2, sfImportVLKey, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_16(DEFINE_ONE_IMPORT_VL_KEYS)
#define DEFINE_ONE_HOOK_EMISSIONS(idx)                                         \
  DEFINE_ARRAY_FIELD(sfHookEmissions, 2, sfHookEmission, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_HOOK_EMISSIONS)
#define DEFINE_ONE_AMOUNTS(idx)                                                \
  DEFINE_ARRAY_FIELD(sfAmounts, 2, sfAmountEntry, idx)
DEFINE_MULTIPLE_FIELD_INDICEX_32(DEFINE_ONE_AMOUNTS)

struct TransactionTemplateBase {
  // Field<sfTransactionType> TransactionType; specify in each template
  TemplateField<sfFlags> Flags;
  TemplateField<sfSequence> Sequence;
  TemplateField<sfFirstLedgerSequence> FirstLedgerSequence;
  TemplateField<sfLastLedgerSequence> LastLedgerSequence;
  TemplateField<sfFee> Fee;
  TemplateField<sfSigningPubKey> SigningPubKey;
  TemplateField<sfAccount> Account;
  TemplateField<sfEmitDetails> EmitDetails;

  template <typename TemplateType> inline void autofill(TemplateType *txn) {
    // TXN PREPARE: FirstLedgerSequence
    uint32_t fls = (uint32_t)ledger_seq() + 1;
    txn->FirstLedgerSequence.setUint32(fls);

    // TXN PREPARE: LastLedgerSequense
    uint32_t lls = fls + 4;
    txn->LastLedgerSequence.setUint32(lls);

    // TXN PREPARE: Emit Metadata
    etxn_details(uint32_t(&txn->EmitDetails), 116U);

    // TXN PREPARE: Fee
    {
      int64_t fee = etxn_fee_base(uint32_t(txn), sizeof(*txn));
      txn->Fee.setDrops(fee);
    }
  }
};

struct PaymentTemplate : TransactionTemplateBase {
  TemplateField<sfTransactionType> TransactionType = {
      .value = {0x00, ttPAYMENT}};
  TemplateField<sfDestination> Destination;
  TemplateField<sfAmount> Amount;
  inline void autofill() {
    TransactionTemplateBase::autofill<PaymentTemplate>(this);
  }
};

struct AccountSetTemplate : TransactionTemplateBase {
  TemplateField<sfTransactionType> TransactionType = {
      .value = {0x00, ttACCOUNT_SET}};
  inline void autofill() {
    TransactionTemplateBase::autofill<AccountSetTemplate>(this);
  }
};

struct InvokeTemplate : TransactionTemplateBase {
  TemplateField<sfTransactionType> TransactionType = {
      .value = {0x00, ttINVOKE}};
  OptionalTemplateField<sfDestination> Destination;
  inline void autofill() {
    TransactionTemplateBase::autofill<InvokeTemplate>(this);
  }
};
