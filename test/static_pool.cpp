//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/static_pool.hpp>

#include "test_suite.hpp"

#include <iostream>

namespace boost {
namespace urls {

class static_pool_test
{
public:
    void
    run()
    {
    }
};

TEST_SUITE(static_pool_test, "boost.url.static_pool");

} // urls
} // boost
