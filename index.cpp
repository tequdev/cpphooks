#include "hookapi.h"

InvokeTemplate tx_buffer = {};

extern "C" int64_t hook(uint32_t reserved) {
  _g(1, 1);
  etxn_reserve(1);

  hook_account(SBUF(tx_buffer.Account.value));

  otxn_field(SBUF(tx_buffer.Destination.value), sfAccount);
  tx_buffer.Destination.useField();

  tx_buffer.autofill();

  trace(SBUF("tx_buffer"), SBUF(tx_buffer), 1);

  uint8_t hash[32];
  ASSERT(emit(SBUF(hash), SBUF(tx_buffer)) == 32);

  return accept(SBUF("success!"), 0);
}
