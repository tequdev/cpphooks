#include "macro.h"

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
  case STI_VL:
    // TODO
    return 1;
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

template <uint32_t sfcode> struct Field {};
template <uint32_t sfcode> struct OptionalField {};

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
//   template <> struct Field<sfcode> {                                           \
//     uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
//     uint8_t value[field_size_by_sto(sfcode)];                                  \
//   };                                                                           \
//   template <> struct OptionalField<sfcode> {                                   \
//     uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_2_##TYPE(sfcode);    \
//     uint8_t value[field_size_by_sto(sfcode)] = {[0 ... (field_size_by_sto(sfcode)-1)] = 0x99};                   \
//   };

#define DEFINE_UINT8_FIELD(sfcode, TYPE)                                       \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
    void setTT(uint16_t tt) {}                                                 \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_UINT64_FIELD(sfcode, TYPE)                                      \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_UINT128_FIELD(sfcode, TYPE)                                     \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t value[8] = {0x40};                                                 \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

// TODO: length
#define DEFINE_VL_FIELD(sfcode, TYPE)                                          \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t length[1] = {0x00};                                                \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t length[1] = {0x14};                                                \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t prefix[1] = {0x7c};                                                \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
    uint8_t postfix[1] = {0xe1};                                               \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
#define DEFINE_ARRAY_FIELD(sfcode, TYPE)                                       \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);             \
    uint8_t prefix[1] = {0xf4};                                                \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
    uint8_t postfix[1] = {0xf1};                                               \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t prefix[1] = {0x99};                                                \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
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
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

// TODO: length
#define DEFINE_VECTOR256_FIELD(sfcode, TYPE)                                   \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };                                                                           \
  template <> struct OptionalField<sfcode> {                                   \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_OPTIONAL_VALUE_1_##TYPE(sfcode);    \
    uint8_t value[field_size_by_sto(sfcode)] = {                               \
        [0 ...(field_size_by_sto(sfcode) - 1)] = 0x99};                        \
    inline void useField() {                                                   \
      uint8_t code_value[CODE_SIZE(sfcode)] = CODE_VALUE_1_##TYPE(sfcode);     \
      for (int i = 0; i < CODE_SIZE(sfcode); i++)                              \
        code[i] = code_value[i];                                               \
    }                                                                          \
  };

#define DEFINE_UINT96_FIELD(sfcode, TYPE)                                      \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

#define DEFINE_UINT192_FIELD(sfcode, TYPE)                                     \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

#define DEFINE_UINT384_FIELD(sfcode, TYPE)                                     \
  template <> struct Field<sfcode> {                                           \
    uint8_t code[CODE_SIZE(sfcode)] = CODE_VALUE_2_##TYPE(sfcode);             \
    uint8_t value[field_size_by_sto(sfcode)];                                  \
  };

#define DEFINE_UINT512_FIELD(sfcode, TYPE)                                     \
  template <> struct Field<sfcode> {                                           \
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

DEFINE_VECTOR256_FIELD(sfIndexes, 1)
DEFINE_VECTOR256_FIELD(sfHashes, 1)
DEFINE_VECTOR256_FIELD(sfAmendments, 1)
DEFINE_VECTOR256_FIELD(sfNFTokenOffers, 1)
DEFINE_VECTOR256_FIELD(sfHookNamespaces, 1)
DEFINE_VECTOR256_FIELD(sfURITokenIDs, 2)

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

DEFINE_ARRAY_FIELD(sfSigners, 1)
DEFINE_ARRAY_FIELD(sfSignerEntries, 1)
DEFINE_ARRAY_FIELD(sfTemplate, 1)
DEFINE_ARRAY_FIELD(sfNecessary, 1)
DEFINE_ARRAY_FIELD(sfSufficient, 1)
DEFINE_ARRAY_FIELD(sfAffectedNodes, 1)
DEFINE_ARRAY_FIELD(sfMemos, 1)
DEFINE_ARRAY_FIELD(sfNFTokens, 1)
DEFINE_ARRAY_FIELD(sfHooks, 1)
DEFINE_ARRAY_FIELD(sfMajorities, 2)
DEFINE_ARRAY_FIELD(sfDisabledValidators, 2)
DEFINE_ARRAY_FIELD(sfHookExecutions, 2)
DEFINE_ARRAY_FIELD(sfHookParameters, 2)
DEFINE_ARRAY_FIELD(sfHookGrants, 2)
DEFINE_ARRAY_FIELD(sfGenesisMints, 2)
DEFINE_ARRAY_FIELD(sfActiveValidators, 2)
DEFINE_ARRAY_FIELD(sfImportVLKeys, 2)
DEFINE_ARRAY_FIELD(sfHookEmissions, 2)
DEFINE_ARRAY_FIELD(sfAmounts, 2)

struct TransactionTemplateBase {
  // Field<sfTransactionType> TransactionType; specify in each template
  Field<sfFlags> Flags;
  Field<sfSequence> Sequence;
  Field<sfFirstLedgerSequence> FirstLedgerSequence;
  Field<sfLastLedgerSequence> LastLedgerSequence;
  Field<sfFee> Fee;
  Field<sfSigningPubKey> SigningPubKey;
  Field<sfAccount> Account;
  Field<sfEmitDetails> EmitDetails;

  template <typename TemplateType> inline void autofill(TemplateType *txn) {
    // TXN PREPARE: FirstLedgerSequence
    uint32_t fls = (uint32_t)ledger_seq() + 1;
    *(uint32_t *)(txn->FirstLedgerSequence.value) = FLIP_ENDIAN_32(fls);

    // TXN PREPARE: LastLedgerSequense
    uint32_t lls = fls + 4;
    *(uint32_t *)(txn->LastLedgerSequence.value) = FLIP_ENDIAN_32(lls);

    // TXN PREPARE: Emit Metadata
    etxn_details(uint32_t(&txn->EmitDetails), 116U);

    // TXN PREPARE: Fee
    {
      int64_t fee = etxn_fee_base(uint32_t(txn), sizeof(*txn));

      uint8_t *b = txn->Fee.value;
      *b++ = 0b01000000 + ((fee >> 56) & 0b00111111);
      *b++ = (fee >> 48) & 0xFFU;
      *b++ = (fee >> 40) & 0xFFU;
      *b++ = (fee >> 32) & 0xFFU;
      *b++ = (fee >> 24) & 0xFFU;
      *b++ = (fee >> 16) & 0xFFU;
      *b++ = (fee >> 8) & 0xFFU;
      *b++ = (fee >> 0) & 0xFFU;
    }
  }
};

struct PaymentTemplate : TransactionTemplateBase {
  Field<sfTransactionType> TransactionType = {.value = {0x00, ttPAYMENT}};
  Field<sfDestination> Destination;
  Field<sfAmount> Amount;
  inline void autofill() {
    TransactionTemplateBase::autofill<PaymentTemplate>(this);
  }
};

struct InvokeTemplate : TransactionTemplateBase {
  Field<sfTransactionType> TransactionType = {.value = {0x00, ttINVOKE}};
  OptionalField<sfDestination> Destination;
  inline void autofill() {
    TransactionTemplateBase::autofill<InvokeTemplate>(this);
  }
};
