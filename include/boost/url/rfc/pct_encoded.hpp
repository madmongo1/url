//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_PCT_ENCODED_HPP
#define BOOST_URL_RFC_PCT_ENCODED_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error.hpp>
#include <boost/url/string.hpp>

namespace boost {
namespace urls {
namespace rfc {

/** BNF for pct-encoded

    @par BNF
    @code
    pct-encoded   = "%" HEXDIG HEXDIG
    @endcode

    @see
        https://datatracker.ietf.org/doc/html/rfc3986#section-2.1
*/
class pct_encoded
{
public:
    BOOST_URL_DECL
    char const*
    parse(
        char const* const start,
        char const* const end,
        error_code& ec);
};

} // rfc
} // urls
} // boost

#endif
