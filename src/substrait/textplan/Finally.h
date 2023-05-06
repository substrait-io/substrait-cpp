/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

namespace io::substrait::textplan {

template <typename F>
struct FinalAction {
  explicit FinalAction(F f) : clean_{f} {}

  ~FinalAction() {
    clean_();
  }

 private:
  F clean_;
};

template <typename F>
FinalAction<F> finally(F f) {
  return FinalAction<F>(f);
}

} // namespace io::substrait::textplan
