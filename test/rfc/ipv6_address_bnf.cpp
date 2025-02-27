//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/rfc/ipv6_address_bnf.hpp>

#include <boost/url/bnf/parse.hpp>
#include "test_suite.hpp"
#include "test_bnf.hpp"

namespace boost {
namespace urls {

class ipv6_address_bnf_test
{
public:
    static
    std::uint64_t
    get_u64(std::uint8_t const* p)
    {
        return
            (static_cast<std::uint64_t>(p[0]) << 56) +
            (static_cast<std::uint64_t>(p[1]) << 48) +
            (static_cast<std::uint64_t>(p[2]) << 40) +
            (static_cast<std::uint64_t>(p[3]) << 32) +
            (static_cast<std::uint64_t>(p[4]) << 24) +
            (static_cast<std::uint64_t>(p[5]) << 16) +
            (static_cast<std::uint64_t>(p[6]) <<  8) +
             static_cast<std::uint64_t>(p[7]);
    }

    ipv6_address_bnf
    check(
        string_view s,
        std::uint64_t u0,
        std::uint64_t u1)
    {
        error_code ec;
        ipv6_address_bnf t;
        using bnf::parse;
        BOOST_TEST(
            parse(s, ec, t));
        if( ! BOOST_TEST(
            ! ec.failed()))
            return {};
        auto const bytes =
            t.addr.to_bytes();
        BOOST_TEST(get_u64(
            &bytes[0]) == u0);
        BOOST_TEST(get_u64(
            &bytes[8]) == u1);
        return t;
    }

    void
    run()
    {
        using T = ipv6_address_bnf;

        bad<T>("");
        bad<T>("0");
        bad<T>("0:1.2.3.4");
        bad<T>("0:0:0:0:0:0:0::1.2.3.4");
        bad<T>("0:0:0:0:0:0:0:1.2.3.4");
        bad<T>("::FFFF:999.2.3.4");
        bad<T>("0");
        bad<T>(":");
        bad<T>("::0::");
        bad<T>(":0::");
        bad<T>("0::0:x");
        bad<T>("x::");
        bad<T>("0:12");
        bad<T>("0:123");
        bad<T>("::1.");
        bad<T>("::1.2");
        bad<T>("::1.2");
        bad<T>("::1.2x");
        bad<T>("::1.2.");
        bad<T>("::1.2.3");
        bad<T>("::1.2.3");
        bad<T>("::1.2.3x");
        bad<T>("::1.2.3.");
        bad<T>("::1.2.3.4x");

        check("1:2:3:4:5:6:7:8", 0x0001000200030004, 0x0005000600070008);
        check("::2:3:4:5:6:7:8", 0x0000000200030004, 0x0005000600070008);
        check("1::3:4:5:6:7:8",  0x0001000000030004, 0x0005000600070008);
        check("1:2::4:5:6:7:8",  0x0001000200000004, 0x0005000600070008);
        check("1:2:3::5:6:7:8",  0x0001000200030000, 0x0005000600070008);
        check("1:2:3:4::6:7:8",  0x0001000200030004, 0x0000000600070008);
        check("1:2:3:4:5::7:8",  0x0001000200030004, 0x0005000000070008);
        check("1:2:3:4:5:6::8",  0x0001000200030004, 0x0005000600000008);
        check("1:2:3:4:5:6:7::", 0x0001000200030004, 0x0005000600070000);
        check("::3:4:5:6:7:8", 0x0000000000030004, 0x0005000600070008);
        check("1::4:5:6:7:8",  0x0001000000000004, 0x0005000600070008);
        check("1:2::5:6:7:8",  0x0001000200000000, 0x0005000600070008);
        check("1:2:3::6:7:8",  0x0001000200030000, 0x0000000600070008);
        check("1:2:3:4::7:8",  0x0001000200030004, 0x0000000000070008);
        check("1:2:3:4:5::8",  0x0001000200030004, 0x0005000000000008);
        check("1:2:3:4:5:6::", 0x0001000200030004, 0x0005000600000000);
        check("::4:5:6:7:8", 0x0000000000000004, 0x0005000600070008);
        check("1::5:6:7:8",  0x0001000000000000, 0x0005000600070008);
        check("1:2::6:7:8",  0x0001000200000000, 0x0000000600070008);
        check("1:2:3::7:8",  0x0001000200030000, 0x0000000000070008);
        check("1:2:3:4::8",  0x0001000200030004, 0x0000000000000008);
        check("1:2:3:4:5::", 0x0001000200030004, 0x0005000000000000);
        check("::5:6:7:8", 0x0000000000000000, 0x0005000600070008);
        check("1::6:7:8",  0x0001000000000000, 0x0000000600070008);
        check("1:2::7:8",  0x0001000200000000, 0x0000000000070008);
        check("1:2:3::8",  0x0001000200030000, 0x0000000000000008);
        check("1:2:3:4::", 0x0001000200030004, 0x0000000000000000);
        check("::6:7:8", 0x0000000000000000, 0x0000000600070008);
        check("1::7:8",  0x0001000000000000, 0x0000000000070008);
        check("1:2::8",  0x0001000200000000, 0x0000000000000008);
        check("1:2:3::", 0x0001000200030000, 0x0000000000000000);
        check("::7:8", 0x0000000000000000, 0x0000000000070008);
        check("1::8",  0x0001000000000000, 0x0000000000000008);
        check("1:2::", 0x0001000200000000, 0x0000000000000000);
        check("::8", 0x0000000000000000, 0x0000000000000008);
        check("1::", 0x0001000000000000, 0x0000000000000000);

        check("::0", 0, 0);
        check("::1", 0, 1);
        check("0:0:0::1", 0, 1);
        check("0:0:0:0:0:0:0:0", 0, 0);
        check("0:0:0:0:0:0:0.0.0.0", 0, 0);
        check("::1.2.3.4", 0, 0x01020304);
        check("::1234:5678", 0, 0x0000000012345678);
        check("::FFFF:1.2.3.4", 0, 0x0000ffff01020304);
        check("1:2::3:4:5", 0x0001000200000000, 0x0000000300040005);
        check("::1:2:3:4:5", 0x0000000000000001, 0x0002000300040005);
        check("1:2:3:4:5::", 0x0001000200030004, 0x0005000000000000);
        check("1:2:0:0:0:3:4:5", 0x0001000200000000, 0x0000000300040005);
        check("1:2:3:4:5:0:0:0", 0x0001000200030004, 0x0005000000000000);
        check("0:0:0:1:2:3:4:5", 0x0000000000000001, 0x0002000300040005);
        check("0:0:0:0:0:FFFF:102:405", 0, 0x0000ffff01020405);
        check("0000:0000:0000:0000:0000:0000:0000:0000", 0, 0);
        check("1234:5678:9ABC:DEF0:0000:0000:0000:0000", 0x123456789abcdef0, 0);
        check("2001:0DB8:0A0B:12F0:0000:0000:0000:0001", 0x20010db80a0b12f0, 0x0000000000000001);
        check("2001:DB8:3333:4444:5555:6666:7777:8888", 0x20010db833334444, 0x5555666677778888);
        check("2001:DB8:3333:4444:CCCC:DDDD:EEEE:FFFF", 0x20010db833334444, 0xccccddddeeeeffff);
        check("2001:db8::", 0x20010db800000000, 0);
        check("2001:DB8::", 0x20010db800000000, 0);
        check("2001:DB8:A0B:12F0::1", 0x20010db80a0b12f0, 1);
        check("2001:db8::1234:5678", 0x20010db800000000, 0x12345678);
        check("2001:DB8::1234:5678", 0x20010db800000000, 0x0000000012345678);
        check("2001:DB8:1::AB9:C0A8:102", 0x20010db800010000, 0x00000ab9c0a80102);
        check("2001:db8:1::ab9:C0A8:102", 0x20010db800010000, 0x00000ab9c0a80102);
        check("2001:0DB8:0A0B:12F0:0:0:0:1", 0x20010db80a0b12f0, 1);
        check("2001:db8:3333:4444:5555:6666:7777:8888", 0x20010db833334444, 0x5555666677778888);
        check("2001:db8:3333:4444:CCCC:DDDD:EEEE:FFFF", 0x20010db833334444, 0xccccddddeeeeffff);
        check("2001:0DB8:0001:0000:0000:0AB9:C0A8:0102", 0x20010db800010000, 0x00000ab9c0a80102);
        check("2001:0db8:0a0b:12f0:0000:0000:0000:0001", 0x20010db80a0b12f0, 1);
        check("2001:0db8:0001:0000:0000:0ab9:C0A8:0102", 0x20010db800010000, 0x0ab9c0a80102);
        check("3FFE:1900:4545:3:200:F8FF:FE21:67CF", 0x3ffe190045450003, 0x0200f8fffe2167cf);
        check("684D:1111:222:3333:4444:5555:6:77", 0x684d111102223333, 0x4444555500060077);
        check("fe80:0:0:0:200:f8ff:fe21:67cf", 0xfe80000000000000, 0x0200f8fffe2167cf);
        check("FE80:0:0:0:200:F8FF:FE21:67CF", 0xfe80000000000000, 0x0200f8fffe2167cf);
        check("FFFF:0:0:0:0:0:0:1", 0xffff000000000000, 1);
        check("FFFF::1", 0xffff000000000000, 1);
    }
};

TEST_SUITE(
    ipv6_address_bnf_test,
    "boost.url.ipv6_address_bnf");

} // urls
} // boost
