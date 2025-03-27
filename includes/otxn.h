#include "hookapi.h"

struct U16Field {
  uint16_t value;
  U16Field() {}
  U16Field(uint32_t slot_no) { value = slot(0, 0, slot_no); }
};

struct U32Field {
  uint32_t value;
  U32Field() {}
  U32Field(uint32_t slot_no) { value = slot(0, 0, slot_no); }
};

struct U64Field {
  uint64_t value;
  U64Field() {}
  U64Field(uint32_t slot_no) { value = slot(0, 0, slot_no); }
};

struct U128Field {
  uint8_t value[16];
  U128Field() {}
  U128Field(uint32_t slot_no) { slot(SBUF(value), slot_no); }
};

struct U256Field {
  uint8_t value[32];
  U256Field() {}
  U256Field(uint32_t slot_no) { slot(SBUF(value), slot_no); }
};

struct AmountField {
  uint8_t value[8];
  uint8_t currency[20];
  uint8_t issuer[20];
  AmountField() {}
  AmountField(uint32_t slot_no) {
    if (slot_type(slot_no, 1) == 1) {
      // Native
      slot(SBUF(value), slot_no);
    } else {
      // IOU
      slot(SBUF(value), slot_no);
    }
  }
};

struct AccountField {
  uint8_t value[20];
  AccountField() {}
  AccountField(uint32_t slot_no) { slot(SBUF(value), slot_no); }
};

struct U8Field {
  uint8_t value;
  U8Field(uint32_t slot_no) { value = slot(0, 0, slot_no); }
};

struct OtxnTransaction {
  OtxnTransaction() { otxn_slot(255); }

  AmountField getFieldAmount(uint32_t sfcode) {
    return AmountField(slot_subfield(255, sfcode, 0));
  }

  AccountField getFieldAccount(uint32_t sfcode) {
    return AccountField(slot_subfield(255, sfcode, 0));
  }

  U32Field getFieldU32(uint32_t sfcode) {
    return U32Field(slot_subfield(255, sfcode, 0));
  }

  U64Field getFieldU64(uint32_t sfcode) {
    return U64Field(slot_subfield(255, sfcode, 0));
  }

  U128Field getFieldU128(uint32_t sfcode) {
    return U128Field(slot_subfield(255, sfcode, 0));
  }

  U256Field getFieldU256(uint32_t sfcode) {
    return U256Field(slot_subfield(255, sfcode, 0));
  }
};
