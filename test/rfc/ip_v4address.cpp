//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/rfc/ip_v4address.hpp>

#include "test_suite.hpp"
#include "test_bnf.hpp"

namespace boost {
namespace urls {
namespace rfc {

class ip_v4address_test
{
public:
    void
    check(string_view s, std::uint32_t v)
    {
        error_code ec;
        ip_v4address p;
        auto const end =
            s.data() + s.size();
        auto it = p.parse(
            s.data(), end, ec);
        if(! BOOST_TEST(! ec))
            return;
        if(! BOOST_TEST(it == end))
            return;
        std::uint32_t v1 =
            (static_cast<std::uint32_t>(
                (*p)[0]) << 24) |
            (static_cast<std::uint32_t>(
                (*p)[1]) << 16) |
            (static_cast<std::uint32_t>(
                (*p)[2]) <<  8) |
            (static_cast<std::uint32_t>(
                (*p)[3])      );
        BOOST_TEST(v1 == v);
    }

    void
    run()
    {
        bad <ip_v4address>("0");
        bad <ip_v4address>("0.");
        bad <ip_v4address>("0.0");
        bad <ip_v4address>("0.0.");
        bad <ip_v4address>("0.0.0");
        bad <ip_v4address>("0.0.0.");
        bad <ip_v4address>("0.0.0.256");
        bad <ip_v4address>("1.2.3.4.");
        bad <ip_v4address>("1.2.3.4x");
        bad <ip_v4address>("1.2.3.300");

        good<ip_v4address>("0.0.0.0");
        good<ip_v4address>("1.2.3.4");

        check("0.0.0.0", 0x00000000);
        check("1.2.3.4", 0x01020304);
        check("32.64.128.1", 0x20408001);
        check("255.255.255.255", 0xffffffff);
    }
};

TEST_SUITE(
    ip_v4address_test,
    "boost.url.ip_v4address");

} // rfc
} // urls
} // boost
