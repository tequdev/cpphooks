#include "hookapi.h"

InvokeTemplate tx_buffer = {};

extern "C" int64_t hook(uint32_t reserved) {
  _g(1, 1);
  etxn_reserve(1);

  hook_account(uint32_t(&tx_buffer.Account.value), 20);

  otxn_field(uint32_t(&tx_buffer.Destination.value), 20, sfAccount);
  tx_buffer.Destination.useField();

  tx_buffer.autofill();

  trace(SBUF("tx_buffer"), uint32_t(&tx_buffer), sizeof(tx_buffer), 1);

  uint8_t hash[32];
  ASSERT(emit(SBUF(hash), uint32_t(&tx_buffer), sizeof(tx_buffer)) == 32);

  return accept(SBUF("success!"), 0);
}
