//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_FRAGMENT_BNF_HPP
#define BOOST_URL_FRAGMENT_BNF_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error.hpp>
#include <boost/url/rfc/pct_encoded_bnf.hpp>
#include <boost/optional.hpp>

namespace boost {
namespace urls {

/** BNF for fragment

    @par BNF
    @code
    fragment        = *( pchar / "/" / "?" )
    @endcode

    @see
        https://datatracker.ietf.org/doc/html/rfc3986#section-3.5
*/
struct fragment_bnf
{
    pct_encoded_str& v;

    BOOST_URL_DECL
    friend
    bool
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        fragment_bnf const& t);
};

} // urls
} // boost

#endif
