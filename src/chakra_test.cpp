#include "chakra.h"

namespace ocn::chakra
{
  void test()
  {
    value v;

    value v2{3};

    int i2 = v2;

    char s2 = v2;


    value obj;
    std::string prop("test_prop");

    obj[prop] = 3;

    obj["prop2"] = 32;

    obj["empty_string"] = "";
  }
}

