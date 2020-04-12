// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_INCLUDE_ACCOUNT_H_
#define FT_INCLUDE_ACCOUNT_H_

#include <string>

#include "Common.h"

namespace ft {

struct Account {
  std::string account_id;
  double      balance;
  double      frozen;
};

}  // namespace ft

#endif  // FT_INCLUDE_ACCOUNT_H_
