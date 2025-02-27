//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IP_LITERAL_BNF_HPP
#define BOOST_URL_IP_LITERAL_BNF_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error.hpp>
#include <boost/url/ipv6_address.hpp>
#include <boost/url/string.hpp>

namespace boost {
namespace urls {

/** BNF for IP-literal

    @par BNF
    @code
    IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
    @endcode

    @see
        https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2
*/
struct ip_literal_bnf
{
    string_view str;
    bool is_ipv6;
    ipv6_address ipv6;
    string_view fut_str;

    BOOST_URL_DECL
    friend
    bool
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        ip_literal_bnf& t);
};

} // urls
} // boost

#endif
