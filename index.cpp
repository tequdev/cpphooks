#include "hookapi.h"

InvokeTemplate invoke_buffer = {};
AccountSetTemplate account_set_buffer = {};

inline void account_set() {
  hook_account(SBUF(account_set_buffer.Account.value));

  account_set_buffer.autofill();

  trace(SBUF("account_set_buffer"), SBUF(account_set_buffer), 1);

  uint8_t hash[32];
  ASSERT(emit(SBUF(hash), SBUF(account_set_buffer)) == 32);
}

inline void invoke() {
  hook_account(SBUF(invoke_buffer.Account.value));

  otxn_field(SBUF(invoke_buffer.Destination.value), sfAccount);
  invoke_buffer.Destination.useField();

  invoke_buffer.autofill();

  trace(SBUF("invoke_buffer"), SBUF(invoke_buffer), 1);

  uint8_t hash[32];
  ASSERT(emit(SBUF(hash), SBUF(invoke_buffer)) == 32);
}

extern "C" int64_t hook(uint32_t reserved) {
  _g(1, 1);
  etxn_reserve(1);

  OtxnTransaction tx;
  trace(SBUF("Account"), SBUF(tx.getFieldAccount(sfAccount)), 1);
  trace_num(SBUF("Sequence"), tx.getFieldU32(sfSequence).value);

  uint8_t hook_acc[20];
  hook_account(SBUF(hook_acc));

  uint8_t otxn_acc[20];
  otxn_field(SBUF(otxn_acc), sfAccount);

  if (BUFFER_EQUAL_20(hook_acc, otxn_acc))
    account_set();
  else
    invoke();

  return accept(SBUF("success!"), 0);
}
