//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PARTS_HPP
#define BOOST_URL_DETAIL_PARTS_HPP

#include <boost/url/host_type.hpp>
#include <boost/url/ipv4_address.hpp>
#include <boost/url/detail/char_type.hpp>
#include <cstring>
#include <new>

namespace boost {
namespace urls {
namespace detail {

enum part
{
    id_scheme = 0,  // trailing ':'
    id_user,        // leading "//"
    id_pass,        // leading ':', trailing '@'
    id_host,
    id_port,        // leading ':'
    id_path,
    id_query,       // leading '?'
    id_frag,        // leading '#'
    id_end          // one past the end
};

//----------------------------------------------------------

struct parts
{
    std::size_t offset[id_end + 1];
    std::size_t decoded[id_end];
    unsigned char ip_addr[16];
    std::size_t nseg = 0;
    std::size_t nparam = 0;
    std::uint16_t port_number = 0;
    urls::host_type host_type =
        urls::host_type::none;

    parts() noexcept
    {
        clear();
    }

    void
    clear() noexcept
    {
        // VFALCO is this needed?
        host_type = urls::host_type::none;
        for(int i = 0; i <= id_end; ++i)
            offset[i] = 0;
        // VFALCO This isn't really needed
        /*
        for(int i = 0; i < id_end; ++i)
            decoded[i] = 0;
        */
    }

    std::size_t
    length(int id) const noexcept
    {
        return offset[id + 1] -
            offset[id];
    }

    std::size_t
    length(
        int begin,
        int end) const noexcept
    {
        BOOST_ASSERT(begin <= end);
        BOOST_ASSERT(end <= id_end);
        return offset[end] -
            offset[begin];
    }

    string_view
    get(int id,
        char const* s) const noexcept
    {
        return {
            s + offset[id],
            offset[id + 1] -
                offset[id] };
    }

    string_view
    get(int begin,
        int end,
        char const* s) const noexcept
    {
        return {
            s + offset[begin],
            offset[end] -
                offset[begin] };
    }

    void
    resize(
        int id,
        std::size_t n) noexcept
    {
        auto const n0 = length(id);
        auto const d = n - n0;
        for(auto i = id + 1;
            i <= id_end; ++i)
            offset[i] += d;
    }

    void
    split(
        int id,
        std::size_t n) noexcept
    {
        BOOST_ASSERT(id < detail::id_end - 1);
        BOOST_ASSERT(n <= length(id));
        offset[id + 1] = offset[id] +
            static_cast<std::size_t>(n);
    }
};

} // detail
} // urls
} // boost

#endif
