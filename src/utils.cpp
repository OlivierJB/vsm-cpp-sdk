// Copyright (c) 2018, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

#include <ugcs/vsm/utils.h>
#include <random>
#include <algorithm>
#include <locale>

uint64_t
ugcs::vsm::Get_random_seed()
{
    // Obvious way would be to just use std::random_device.
    // Unfortunately it is not enough, so we use other sources of entropy, too.
    // Intel arch has __rdtsc() but we must be able to run on ARM.
    // Mingw implements random_device as PRNG which produces the same sequence on each instantiation.
    // high_resolution_clock is not guaranteed to have "high" resolution.
    // Stack address does not have much entropy if ASLR is not used.
    // But all put together should give enough entropy.
    std::random_device rd;
    auto random_number = rd();
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    uintptr_t address_on_stack = reinterpret_cast<uintptr_t>(&address_on_stack);
    return address_on_stack + time + random_number;
}

uint32_t
ugcs::vsm::Get_application_instance_id()
{
    // We want to generate unique ID on each program launch.
    static uint32_t my_id(Get_random_seed());
    return my_id;
}

std::string&
ugcs::vsm::Ltrim(std::string& str)
{
  auto it = std::find_if(str.begin(), str.end(),
      [](char c)
      {
          return !std::isspace<char>(c, std::locale::classic());
      });
  str.erase(str.begin(), it);
  return str;
}

std::string&
ugcs::vsm::Rtrim(std::string& str)
{
  auto it = std::find_if(str.rbegin(), str.rend(),
    [](char c)
    {
        return !std::isspace<char>(c, std::locale::classic());
    });
  str.erase(it.base(), str.end());
  return str;
}

std::string&
ugcs::vsm::Trim(std::string& str)
{
   return Ltrim(Rtrim(str));
}
