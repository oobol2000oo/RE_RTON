#pragma once

#include <string>
#include <vector>

using namespace std;

int write_RTON(json js);
int get_utf8_size(string q);
int write_unsigned_RTON_num(vector <uint8_t> a);
int rton_encode();
int write_RTON_block(json js);