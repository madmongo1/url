//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_URL_HPP
#define BOOST_URL_URL_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/ipv4_address.hpp>
#include <boost/url/ipv6_address.hpp>
#include <boost/url/path_view.hpp>
#include <boost/url/query_params_view.hpp>
#include <boost/url/detail/char_type.hpp>
#include <boost/url/detail/parts.hpp>
#include <boost/url/storage_ptr.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url_view;
#endif

/** A modifiable container for a URL.

    Objects of this type hold URLs which may be
    inspected and modified. The derived class
    is responsible for providing storage.

    The underlying string stored in the container
    is always null-terminated.

    @par Exception Safety

    @li Functions marked `noexcept` provide the
    no-throw guarantee, otherwise:

    @li Functions which throw offer the strong
    exception safety guarantee.

    @see @li <a href="https://tools.ietf.org/html/rfc3986">Uniform Resource Identifier (URI): Generic Syntax</a>
*/
class url
{
    char* s_ = nullptr;
    detail::parts pt_;
    storage_ptr sp_;
    std::size_t cap_ = 0;

    // VFALCO This has to be kept in
    // sync with other declarations
    enum
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

    class modify;

    // shortcuts
    string_view get(int id) const noexcept;
    string_view get(int id0, int id1) const noexcept;
    std::size_t len(int id) const noexcept;
    std::size_t len(int id0, int id1) const noexcept;

public:
    class params_type;
    class segments_type;

    //--------------------------------------------
    //
    // classification
    //
    //--------------------------------------------

    /** Return true if the URL is empty

        An empty URL is a relative-ref with
        zero path segments.
    */
    BOOST_URL_DECL
    bool
    empty() const noexcept;

    //--------------------------------------------

    /** Return the complete serialized URL
    */
    BOOST_URL_DECL
    string_view
    encoded_url() const;

    /** Return the origin
    */
    BOOST_URL_DECL
    string_view
    encoded_origin() const noexcept;

    //--------------------------------------------
    //
    // scheme
    //
    //--------------------------------------------

    /** Return true if a scheme exists
    */
    BOOST_URL_DECL
    bool
    has_scheme() const noexcept;

    /** Return the scheme
    */
    BOOST_URL_DECL
    string_view
    scheme() const noexcept;

    //--------------------------------------------
    //
    // authority
    //
    //--------------------------------------------

    /** Return true if an authority exists

        This function returns true if an authority is
        present, even if the authority is an empty
        string. Its presence
        in a URL is determined by a leading double-slash
        ("//") in the 
        @par Exception Safety
        No-throw guarantee.
    */
    BOOST_URL_DECL
    bool
    has_authority() const noexcept;

    /** Return the authority if it exists, or an empty string

        @see has_authority
    */
    BOOST_URL_DECL
    string_view
    encoded_authority() const noexcept;

    /** Return true if a userinfo exists

        This function returns true if the authority
        is present and contains an at-sign ('@')
        which is not percent encoded.
    */
    BOOST_URL_DECL
    bool
    has_userinfo() const noexcept;

    /** Return the userinfo if it exists, or an empty string

        Returns the userinfo of the URL as an encoded
        string. The userinfo includes the username and
        password, with a colon separating the components
        if the password is not empty.

        @par Exception Safety

        No-throw guarantee.
    */
    BOOST_URL_DECL
    string_view
    encoded_userinfo() const noexcept;

    /** Return the userinfo if it exists, or an empty string

        This function returns the userinfo part
        of the URL if present, as a decoded string.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    userinfo(
        Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_userinfo(), a);
    }

    /** Return the username if it exists, or an empty string

        This function returns the username portion of
        the userinfo if present, as an encoded string.
        The username portion is defined by all of the
        characters in the userinfo up to but not
        including the first colon (':"), or the
        entire userinfo if no colon is present.

        @par Exception Safety

        No-throw guarantee.
    */
    BOOST_URL_DECL
    string_view
    encoded_username() const noexcept;

    /** Return the username if it exists, or an empty string

        This function returns the username portion of
        the userinfo if present, as a decoded string.
        The username portion is defined by all of the
        characters in the userinfo up to but not
        including the first colon (':"), or the
        entire userinfo if no colon is present.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    username(
        Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_username(), a);
    }

    /** Return true if a password exists
    */
    BOOST_URL_DECL
    bool
    has_password() const noexcept;

    /** Return the password if it exists, or an empty string
    */
    BOOST_URL_DECL
    string_view
    encoded_password() const noexcept;

    /** Return the password if it exists, or an empty string
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    password(
        Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_password(), a);
    }

    //--------------------------------------------

    /** Return the type of host present, if any

        @par Exception Safety

        No-throw guarantee.
    */
    urls::host_type
    host_type() const noexcept
    {
        return pt_.host_type;
    }

    /** Return the host

        This function returns the host portion of
        the authority as an encoded string if present,
        otherwise it returns an empty string.

        @par Exception Safety
        No-throw guarantee.
    */
    BOOST_URL_DECL
    string_view
    encoded_host() const noexcept;

    /** Return the host

        This function returns the host portion of
        the authority as a decoded string if present,
        otherwise it returns an empty string.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    host(
        Allocator const& a = {}) const
    {
        auto const s0 =
            encoded_host();
        if(pt_.host_type !=
            urls::host_type::name)
        {
            // no decoding
            return string_type<Allocator>(
                s0.data(), s0.size(), a);
        }
        return pct_decode_unchecked(
            s0, pt_.decoded[id_host], a);
    }

    /** Return the ipv4 address if it exists, or return the unspecified address (0.0.0.0)
    */
    BOOST_URL_DECL
    urls::ipv4_address
    ipv4_address() const noexcept;

    /** Return the ipv6 address if it exists, or return the unspecified address (0:0:0:0:0:0:0:0)
    */
    BOOST_URL_DECL
    urls::ipv6_address
    ipv6_address() const noexcept;

    /** Return the ipvfuture address if it exists, or return an empty string
    */
    BOOST_URL_DECL
    string_view
    ipv_future() const noexcept;

    /** Return true if the URL contains a port
    */
    BOOST_URL_DECL
    bool
    has_port() const noexcept;

    /** Return the port if it exists, or an empty string

        If the URL contains a port, this function
        returns the entire port string, which may
        or may not be a decimal number.

        @par Exception Safety

        No-throw guarantee.
    */
    BOOST_URL_DECL
    string_view
    port() const noexcept;

    /** Return the integer port number, or 0 if no port

        If the URL contains a port string and the
        string contains a decimal value in the range
        0 to 65535, returns the number. Otherwise,
        returns zero.

        @par Exception Safety

        No-throw guarantee.
    */
    BOOST_URL_DECL
    std::uint16_t
    port_number() const noexcept;

    /** Return the host and port.

        This function returns the encoded host and port,
        or an empty string if there is no host or port.
        The returned value includes both the host if present,
        and a port, with a colon separating the host and port
        if either component is non-empty.

        @par Exception Safety

        No-throw guarantee.
    */
    BOOST_URL_DECL
    string_view
    encoded_host_and_port() const noexcept;

    //--------------------------------------------
    //
    // path
    //
    //--------------------------------------------

    /** Return the encoded path
    */
    BOOST_URL_DECL
    string_view
    encoded_path() const noexcept;

    /** Return the path segments as a read-only forward range
    */
    BOOST_URL_DECL
    path_view
    path() const noexcept;

    //--------------------------------------------
    //
    // query
    //
    //--------------------------------------------

    /** Return true if a query exists

        A query exists if the hash mark ('#') is
        present after the path, even if followed
        by an empty query string.

        @see encoded_query, query
    */
    BOOST_URL_DECL
    bool
    has_query() const noexcept;

    /** Return the query if it exists, or an empty string

        This function returns the percent-encoded
        query if it exists. Otherwise it returns
        an empty string.

        @par Exception Safety
        No-throw guarantee.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.

        @see has_query, query
    */
    BOOST_URL_DECL
    string_view
    encoded_query() const noexcept;

    /** Return the query if it exists, or an empty string

        If the query exists, it is returned as a
        string with percent-decoding applied.
        Otherwise, an empty string is returned.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.

        @see encoded_query
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    query(
        Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_query(), a);
    }

    /** Return the query parameters as a read-only forward range
    */
    BOOST_URL_DECL
    query_params_view
    query_params() const noexcept;

    //--------------------------------------------
    //
    // fragment
    //
    //--------------------------------------------

    /** Return true if a fragment is present
    */
    BOOST_URL_DECL
    bool
    has_fragment() const noexcept;

    /** Return the fragment if present, otherwise return an empty string

        This function returns the fragment as a
        percent-encoded string if present, otherwise
        returns an empty string. The returned string
        does not include the leading hash mark ('#').

        @par Exception Safety

        No-throw guarantee.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.

        @see fragment
    */
    BOOST_URL_DECL
    string_view
    encoded_fragment() const noexcept;

    /** Return the fragment.

        This function returns the fragment of the URL:

        * If a fragment is present, it is returned in
        decoded form without a leading hash mark ('#'),
        otherwise:

        * If there is no fragment, an empty string is
        returned.

        Note that if the URL contains a hash mark
        followed by an empty query string, this
        function still returns an empty string.
        To detect this case, use @ref fragment_bnf
        instead.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.

        @see encoded_fragment, fragment_bnf
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    fragment(
        Allocator const& a = {}) const
    {
        return pct_decode_unchecked(
            encoded_fragment(),
            pt_.decoded[id_frag], a);
    }

    //--------------------------------------------

    /** Destructor
    */
    BOOST_URL_DECL
    ~url();

    /** Constructor
    */
    BOOST_URL_DECL
    url() noexcept;

    /** Construct an empty URL with the specified storage.
    */
    explicit
    url(
        storage_ptr sp) noexcept
        : sp_(std::move(sp))
    {
    }

    /** Construct a parsed URL with the specified storage.

        If `s` is not a valid URL (a <em>URI-absolute</em>
        or a <em>relative-ref</em>), an exception is thrown.

        @param a The storage to use.

        @param s The URL to parse.

        @throw std::exception parse error.
    */
    BOOST_URL_DECL
    url(
        storage_ptr sp,
        string_view s);

    explicit
    url(
        string_view s)
        : url(storage_ptr(), s)
    {
    }

    /** Return the number of characters in the URL
    */
    // VFALCO do we need this?
    BOOST_URL_DECL
    std::size_t
    size() const noexcept;

    /** Return the number of characters that may be stored without a reallocation.

        This function returns the maximum number of
        characters, excluding the null terminator,
        which may be stored in the container before
        a reallocation is necessary.
    */
    std::size_t
    capacity() const noexcept
    {
        return cap_;
    }

    /** Clear the contents.
    
        @par Exception Safety
        No-throw guarantee.
    */
    BOOST_URL_DECL
    void
    clear() noexcept;

    //------------------------------------------------------

    /** Set the URL.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The URL to set. The contents must
        meet the syntactic requirements of a
        <em>URI-reference</em>.

        @throw std::exception parsing error.
    */
    BOOST_URL_DECL
    url&
    set_encoded_url(
        string_view s);

    /** Set the origin to the specified value.

        The origin consists of the everything from the
        beginning of the URL up to but not including
        the path.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The origin to set. Special characters
        must be percent-encoded, or an exception is
        thrown.
    */
    BOOST_URL_DECL
    url&
    set_encoded_origin(
        string_view s);

    //------------------------------------------------------
    //
    // scheme
    //
    //------------------------------------------------------

    /** Set the scheme.

        This function sets the scheme to the specified
        string:

        @li If the string is empty, any existing scheme
        is removed along with the trailing colon (':'),
        otherwise:

        @li The scheme is set to the string, which must
        contain a valid scheme. A trailing colon is
        automatically added.

        @par ABNF
        @code
        scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The scheme to set. This string must
        not include a trailing colon, otherwise an
        exception is thrown.

        @throw std::exception invalid scheme.
    */
    BOOST_URL_DECL
    url&
    set_scheme(string_view s);

    //------------------------------------------------------
    //
    // authority
    //
    //------------------------------------------------------

    /** Set the authority.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The authority to set. This string
        must meed the syntactic requirements for
        the components of the authority, otherwise
        an exception is thrown.

        @throw std::exception invalid authority.
    */
    BOOST_URL_DECL
    url&
    set_encoded_authority(
        string_view s);

    //------------------------------------------------------
    //
    // userinfo
    //
    //------------------------------------------------------

    /** Set the userinfo.

        Sets the userinfo of the URL to the given
        encoded string:

        @li If the string is empty, the userinfo is
        cleared, else

        @li If the string is not empty, then the userinfo
        is set to the given string. The user is set to
        the characters up to the first colon if any,
        while the password is set to the remaining
        characters if any.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>userinfo</em> otherwise an exception is
        thrown.

        @par ABNF
        @code
        userinfo      = [ [ user ] [ ':' password ] ]
        user          = *( unreserved / pct-encoded / sub-delims )
        password      = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_userinfo(
        string_view s);

    /** Set the userinfo.

        Sets the userinfo of the URL to the given
        encoded string:

        @li If the string is empty, the userinfo is
        cleared, else

        @li If the string is not empty, then the userinfo
        is set to the given string. The user is set to
        the characters up to the first colon if any,
        while the password is set to the remaining
        characters if any.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>userinfo-part</em> otherwise an exception
        is thrown.

        @par ABNF
        @code
        userinfo-part = [ [ user ] [ ':' password ] '@' ]
        user          = *( unreserved / pct-encoded / sub-delims )
        password      = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_userinfo_part(
        string_view s);

    /** Set the user.

        The user is set to the specified string,
        replacing any previous user:

        @li If the string is empty, the user is cleared.

        @li If the string is not empty then the
        user is set to the new string.
        Any special or reserved characters in the
        string are automatically percent-encoded.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.
    */
    BOOST_URL_DECL
    url&
    set_user(
        string_view s);

    /** Set the user.

        The user is set to the specified encoded
        string, replacing any previous user:

        @li If the string is empty, the user is cleared.

        @li If the string is not empty then the
        user is set to the given string.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>user</em> otherwise an exception is
        thrown.

        @li
        @par ABNF
        @code
        user          = *( unreserved / pct-encoded / sub-delims )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_user(
        string_view s);

    /** Set the password.

        This function sets the password to the specified
        string, replacing any previous password:

        @li If the string is empty, the password is
        cleared, and the first occurring colon (':') is
        removed from the userinfo if present, otherwise

        @li If ths string is not empty then the password
        is set to the new string.
        Any special or reserved characters in the
        string are automatically percent-encoded.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.
    */
    BOOST_URL_DECL
    url&
    set_password(
        string_view s);

    /** Set the password.

        The password is set to the encoded string `s`,
        replacing any previous password:

        @li If the string is empty, the password is
        cleared, and the first occurring colon (':') is
        removed from the userinfo if present, otherwise

        @li If ths string is not empty then the password
        is set to the new string.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>password</em> otherwise an exception is
        thrown.

        @par ANBF
        @code
        password      = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_password(
        string_view s);

    /** Set the password.

        The password part is set to the encoded string
        `s`, replacing any previous password:

        @li If the string is empty, the password is
        cleared, and the first occurring colon (':') is
        removed from the userinfo if present, otherwise

        @li If ths string is not empty then the password
        is set to the new string, which must include a
        leading colon.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>password-part</em> otherwise an exception is
        thrown.

        @par ANBF
        @code
        password-part = [ ':' *( unreserved / pct-encoded / sub-delims / ":" ) ]
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_password_part(
        string_view s);

    //------------------------------------------------------
    //
    // host
    //
    //------------------------------------------------------

    /** Set the host.

        The host is set to the specified string,
        replacing any previous host:

        @li If the string is empty, the host is cleared.
        If there are no more remaining elements in the
        authority, then the authority is removed including
        the leading double slash ("//"), else

        @li If the string is a valid <em>IPv4Address</em>,
        the host is set to the new string and
        @ref host_type will return
        @ref host_type::ipv4, otherwise

        @li If the string is a valid <em>IPv6Address</em>,
        the host is set to the new string and
        @ref host_type will return
        @ref host_type::ipv6, else

        @li If the string is a valid <em>IPv6Future</em>,
        the host is set to the new string and
        @ref host_type will return
        @ref host_type::ipvfuture, else

        @li The host is set to the new string.
        Any special or reserved characters in the
        string are automatically percent-encoded.

        In all cases where the string is valid and not empty,
        if the URL previously did not contain an
        authority (@ref has_authority returns `false`),
        and `s` is not empty, then the authority is added
        including a leading double slash ("//").

        @par ABNF
        @code
        IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet

        IPv6address   =                            6( h16 ":" ) ls32
                      /                       "::" 5( h16 ":" ) ls32
                      / [               h16 ] "::" 4( h16 ":" ) ls32
                      / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                      / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                      / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                      / [ *4( h16 ":" ) h16 ] "::"              ls32
                      / [ *5( h16 ":" ) h16 ] "::"              h16
                      / [ *6( h16 ":" ) h16 ] "::"

        IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.
    */
    BOOST_URL_DECL
    url&
    set_host(
        string_view s);

    /** Set the host.

        The host is set to the specified encoded string,
        replacing any previous host:

        @li If the string is empty, the host is cleared.
        If there are no more remaining elements in the
        authority, then the authority is removed including
        the leading double slash ("//"), else

        @li If the string is not empty, the host
        is set to the new string.
        The string must meet the syntactic requirements
        of <em>host</em> otherwise an exception is
        thrown.
        If the URL previously did not contain an
        authority (@ref has_authority returns `false`),
        then the authority is added including the
        leading double slash ("//").

        @par ABNF
        @code
        host          = IP-literal / IPv4address / reg-name

        reg-name      = *( unreserved / pct-encoded / sub-delims )

        IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"

        IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet

        IPv6address   =                            6( h16 ":" ) ls32
                      /                       "::" 5( h16 ":" ) ls32
                      / [               h16 ] "::" 4( h16 ":" ) ls32
                      / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                      / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                      / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                      / [ *4( h16 ":" ) h16 ] "::"              ls32
                      / [ *5( h16 ":" ) h16 ] "::"              h16
                      / [ *6( h16 ":" ) h16 ] "::"

        IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_host(
        string_view s);

    /** Set the port.

        The port of the URL is set to the specified
        integer, replacing any previous port.

        If the URL previously did not contain an
        authority (@ref has_authority returns `false`),
        then the authority is added including the
        leading double slash ("//").

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param n The port number to set.
    */
    BOOST_URL_DECL
    url&
    set_port(unsigned n);

    /** Set the port.

        The port of the URL is set to the specified string.

        @li If the string is empty, the port is
        cleared including the leading colon (':').
        If there are no more remaining elements in the
        authority, then the authority is removed including
        the leading double slash ("//"), else

        @li If the string is not empty then the port
        is set to the given string, with a leading
        colon added.
        If the URL previously did not contain an
        authority (@ref has_authority returns `false`),
        then the authority is added including the
        leading double slash ("//").
        The string must meet the syntactic requirements
        of <em>port</em> otherwise an exception is
        thrown.

        @par ABNF
        @code
        port          = *DIGIT
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_port(string_view s);

    /** Set the port.

        The port of the URL is set to the specified string.

        @li If the string is empty, the port is
        cleared including the leading colon (':').
        If there are no more remaining elements in the
        authority, then the authority is removed including
        the leading double slash ("//"), else

        @li If the string is not empty then the port
        is set to the given string, which must have
        a starting colon.
        If the URL previously did not contain an
        authority (@ref has_authority returns `false`),
        then the authority is added including the
        leading double slash ("//").
        The string must meet the syntactic requirements
        of <em>port-part</em> otherwise an exception is
        thrown.

        @par ABNF
        @code
        port-part     = [ ':' *DIGIT ]
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_port_part(string_view s);

    //------------------------------------------------------
    //
    // path
    //
    //------------------------------------------------------

    /** Set the path.

        Sets the path of the URL to the specified
        encoded string. If this string is empty,
        any existing path is removed.

        The string must meet the syntactic requirements,
        which vary depending on the existing contents
        of the URL:

        @li If an authority is present (@ref has_authority
        returns `true`), the path syntax must match
        <em>path-abempty</em>, else

        @li If the new path starts with a forward
        slash ('/'), the path syntax must match
        <em>path-absolute</em>, else

        @li If a scheme is present, the path syntax
        must match <em>path-rootless</em>, otherwise

        @li The path syntax must match <em>path-noscheme</em>.

        If the path does not meet the syntactic
        requirements, an exception is thrown.

        @par ABNF
        @code
        path          = path-abempty    ; begins with "/" or is empty
                      / path-absolute   ; begins with "/" but not "//"
                      / path-noscheme   ; begins with a non-colon segment
                      / path-rootless   ; begins with a segment
                      / path-empty      ; zero characters

        path-abempty  = *( "/" segment )
        path-absolute = "/" [ segment-nz *( "/" segment ) ]
        path-noscheme = segment-nz-nc *( "/" segment )
        path-rootless = segment-nz *( "/" segment )
        path-empty    = 0<pchar>
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::exception invalid path.

    */
    BOOST_URL_DECL
    url&
    set_encoded_path(
        string_view s);

    /** Return the path.

        This function returns the path segments
        as a lightweight, non-owning reference to
        the existing data, with the interface of
        a modifiable container.

        @par Exception Safety

        No-throw guarantee.

        @see segments_type
    */
    BOOST_URL_DECL
    segments_type
    path() noexcept;

    //------------------------------------------------------
    //
    // query
    //
    //------------------------------------------------------

    /** Set the query.

        Sets the query of the URL to the specified
        plain string:

        @li If the string is empty, the query is
        cleared including the leading question
        mark ('?'), otherwise:

        @li If the string is not empty, the query
        is set to the given string, with a leading
        question mark added.
        Any special or reserved characters in the
        string are automatically percent-encoded.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.
    */
    BOOST_URL_DECL
    url&
    set_query(
        string_view s);

    /** Set the query.

        Sets the query of the URL to the specified
        encoded string:

        @li If the string is empty, the query is
        cleared including the leading question mark ('?'),
        otherwise:

        @li If the string is not empty, the fragment
        is set to given string, with a leading question
        mark added.
        The string must meet the syntactic requirements
        of <em>query</em> otherwise an exception is
        thrown.

        @par ABNF
        @code
        query         = *( pchar / "/" / "?" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::exception invalid string.
    */
    BOOST_URL_DECL
    url&
    set_encoded_query(
        string_view s);

    /** Set the query.

        Sets the query of the URL to the specified
        encoded string.

        @li If the string is empty, the query is
        cleared including the leading question
        mark ('?'), otherwise:

        @li If the string is not empty, the query
        is set to given string.
        The string must meet the syntactic requirements
        of <em>query-part</em> otherwise an exception
        is thrown.

        @par ABNF
        @code
        query-part    = [ "#" *( pchar / "/" / "?" ) ]
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::exception invalid string.
    */
    BOOST_URL_DECL
    url&
    set_query_part(
        string_view s);

    /** Return the query.

        This function returns the query parameters
        as a lightweight, non-owning reference to
        the existing data, with the interface of
        a modifiable associative container.

        @par Exception Safety

        No-throw guarantee.
    */
    BOOST_URL_DECL
    params_type
    query_params() noexcept;

    //------------------------------------------------------
    //
    // fragment
    //
    //------------------------------------------------------

    /** Set the fragment.

        Sets the fragment of the URL to the specified
        plain string:

        @li If the string is empty, the fragment is
        cleared including the leading hash mark ('#'),
        otherwise:

        @li If the string is not empty, the fragment
        is set to given string, with a leading hash
        mark added.
        Any special or reserved characters in the
        string are automatically percent-encoded.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.

        @see set_encoded_fragment, set_fragment_part
    */
    BOOST_URL_DECL
    url&
    set_fragment(
        string_view s);

    /** Set the fragment.

        Sets the fragment of the URL to the specified
        encoded string:

        @li If the string is empty, the fragment is
        cleared including the leading hash mark ('#'),
        otherwise:

        @li If the string is not empty, the fragment
        is set to given string, with a leading hash
        mark added.
        The string must meet the syntactic requirements
        of <em>fragment</em> otherwise an exception is
        thrown.

        @par ABNF
        @code
        fragment      = *( pchar / "/" / "?" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::exception invalid string.

        @see set_fragment, set_fragment_part
    */
    BOOST_URL_DECL
    url&
    set_encoded_fragment(
        string_view s);

    /** Set the fragment.

        Sets the fragment of the URL to the specified
        encoded string.

        @li If the string is empty, the fragment is
        cleared including the leading hash mark ('#'),
        otherwise:

        @li If the string is not empty, the fragment
        is set to given string.
        The string must meet the syntactic requirements
        of <em>fragment-part</em> otherwise an exception
        is thrown.

        @par ABNF
        @code
        fragment-part = [ "#" *( pchar / "/" / "?" ) ]
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::exception invalid string.

        @see set_fragment, set_encoded_fragment
    */
    BOOST_URL_DECL
    url&
    set_fragment_part(
        string_view s);

    //------------------------------------------------------
    //
    // normalization
    //
    //------------------------------------------------------

    /** Normalize everything.
    */
    BOOST_URL_DECL
    url&
    normalize();

    BOOST_URL_DECL
    url&
    normalize_scheme() noexcept;

private:
    inline void resize_impl(
        std::size_t new_size);
    inline char* resize_impl(
        int id, std::size_t new_size);
    inline char* resize_impl(
        int first, int last, std::size_t new_size);
};

//----------------------------------------------------------

/** A read/write view to the path segments.
*/
class url::segments_type
{
    url* v_;

public:
    class value_type;
    class iterator;
    using const_iterator = iterator;

private:
    BOOST_URL_DECL
    iterator
    insert_encoded_impl(
        iterator pos, string_view s );

    BOOST_URL_DECL
    iterator
    insert_impl(
        iterator pos, string_view s, std::size_t ns );

public:
    segments_type() = delete;

    segments_type(segments_type const&) = default;
    segments_type& operator=(
        segments_type const&) = default;

    explicit
    segments_type(url& v)
        : v_(&v)
    {
    }

    bool
    empty() const noexcept
    {
        return size() == 0;
    }

    std::size_t
    size() const noexcept
    {
        return (v_ == nullptr) ? 0 :
            v_->pt_.nseg;
    }

    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    BOOST_URL_DECL
    iterator
    end() const noexcept;

    /** Erase the specified sequence of path segments.

        @par Exception Safety

        No-throw guarantee.

        @param first The first segment to be erased.

        @param last One past the last segment to be erased.
    */
    BOOST_URL_DECL
    iterator
    erase( iterator first, iterator last ) noexcept;

    /** Erase the specified path segment

        @param pos The path segment to erase

        This function is equivalent to:
        @code
        auto last = pos;
        ++last;
        erase(pos, last);
        @endcode
    */
    BOOST_URL_DECL
    iterator
    erase( iterator pos ) noexcept;

    /** Insert an encoded path segment at the specified position.

        @par Exception Safety

        Strong guarantee. Calls to allocate may throw.

        @param pos The new path segment is inserted before this position.

        @param s The encoded path segment to be inserted.
    */
    BOOST_URL_DECL
    iterator
    insert_encoded( iterator pos, string_view s );

    /** Encode an unencoded path segment and insert it at the specified position.

        @par Exception Safety

        Strong guarantee. Calls to allocate may throw.

        @param pos The new path segment is inserted before this position.

        @param s The unencoded path segment to be inserted.
    */
    BOOST_URL_DECL
    iterator
    insert( iterator pos, string_view s );

    /** Replace the path segment at the specified position with the specified encoded path segment.

        @par Exception Safety

        Strong guarantee. Calls to allocate may throw.

        @param pos The path segment to be replaced.

        @param s The encoded to replace the segment at pos.
    */
    BOOST_URL_DECL
    iterator
    replace_encoded( iterator pos, string_view s );

    /** Replace the path segment at the specified position with the specified unencoded path segment.

        @par Exception Safety

        Strong guarantee. Calls to allocate may throw.

        @param pos The path segment to be replaced.

        @param s The unencoded segment to replace the segment at pos.
    */
    BOOST_URL_DECL
    iterator
    replace( iterator pos, string_view s );
};

//----------------------------------------------------------

class url::segments_type::value_type
{
    string_view s_;

    friend class segments_type;

    explicit
    value_type(
        string_view s) noexcept
        : s_(s)
    {
    }

public:
    value_type() = delete;
    value_type& operator=(
        value_type const&) = delete;

    value_type(
        value_type const&) = default;

    string_view
    encoded_string() const noexcept
    {
        return s_;
    }

    /** Return the segment string.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    string(Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_string(), a);
    }

    value_type const*
    operator->() const noexcept
    {
        return this;
    }
};

//----------------------------------------------------------

class url::segments_type::iterator
{
    friend segments_type;

    url* v_;
    std::size_t off_;
    std::size_t n_;

    BOOST_URL_DECL
    iterator(
        url* v,
        bool end) noexcept;

    inline
    void
    check(url const* v) noexcept;

public:
    using iterator_category =
        std::bidirectional_iterator_tag;

    using value_type =
        segments_type::value_type;

    /// A pointer to an element
    using pointer = value_type const*;

    /// A reference to an element
    using reference = value_type const&;

    /// The difference_type for this iterator
    using difference_type = std::ptrdiff_t;

    BOOST_URL_DECL
    iterator() noexcept;

    BOOST_URL_DECL
    value_type
    operator*() const noexcept;

    value_type
    operator->() const noexcept
    {
        return operator*();
    }

    inline
    bool
    operator==(
    iterator other) const noexcept;

    bool
    operator!=(
        iterator other) const noexcept
    {
        return !(*this == other);
    }

    BOOST_URL_DECL
    iterator&
    operator++() noexcept;

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    BOOST_URL_DECL
    iterator&
    operator--() noexcept;

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

private:
    inline
    void
    parse() noexcept;
};

//----------------------------------------------------------

/** A modifiable view to the URL query parameters.
*/
class url::params_type
{
    url* v_ = nullptr;

public:
    class value_type;
    class iterator;
    using const_iterator = iterator;

    params_type() = delete;

    params_type(
        params_type const&) = default;
    params_type& operator=(
        params_type const&) = default;

    explicit
    params_type(url& v)
        : v_(&v)
    {
    }

    bool
    empty() const noexcept
    {
        return size() == 0;
    }

    std::size_t
    size() const noexcept
    {
        return (v_ == nullptr) ? 0 :
            v_->pt_.nparam;
    }

    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    BOOST_URL_DECL
    iterator
    end() const noexcept;

    BOOST_URL_DECL
    bool
    contains(string_view key) const noexcept;

    BOOST_URL_DECL
    std::size_t
    count(string_view key) const noexcept;

    BOOST_URL_DECL
    iterator
    find(string_view key) const noexcept;

    BOOST_URL_DECL
    std::string
    operator[](string_view key) const;

    /** Return the param matching the given key.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<class Allocator =
        std::allocator<char>>
    string_type<Allocator>
    at( string_view key,
        Allocator const& a = {}) const;
};

//----------------------------------------------------------

class url::params_type::value_type
{
    string_view k_;
    string_view v_;

    friend class params_type;

    value_type(
        string_view k,
        string_view v) noexcept
        : k_(k)
        , v_(v)
    {
    }

public:
    value_type() = delete;
    value_type& operator=(
        value_type const&) = delete;

    value_type(
        value_type const&) = default;

    string_view
    encoded_key() const noexcept
    {
        return k_;
    }

    string_view
    encoded_value() const noexcept
    {
        return v_;
    }

    /** Return the key.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    key(Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_key(), a);
    }

    /** Return the value.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A `std::basic_string` using the
        specified allocator.
    */
    template<
        class Allocator =
            std::allocator<char>>
    string_type<Allocator>
    value(Allocator const& a = {}) const
    {
        return detail::decode(
            encoded_value(), a);
    }

    value_type const*
    operator->() const noexcept
    {
        return this;
    }

    operator
    std::pair<
        std::string const,
        std::string>() const
    {
        return { key(), value() };
    }
};

//----------------------------------------------------------

class url::params_type::iterator
{
    friend params_type;

    url* v_;
    std::size_t off_;
    std::size_t nk_;
    std::size_t nv_;

    BOOST_URL_DECL
    iterator(
        url* v,
        bool end) noexcept;

public:
    using value_type =
        params_type::value_type;

    BOOST_URL_DECL
    iterator() noexcept;

    BOOST_URL_DECL
    value_type
    operator*() const noexcept;

    value_type
    operator->() const noexcept
    {
        return operator*();
    }

    inline
    bool
    operator==(
        iterator other) const noexcept;

    bool
    operator!=(
        iterator other) const noexcept
    {
        return !(*this == other);
    }

    BOOST_URL_DECL
    iterator&
    operator++() noexcept;

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    BOOST_URL_DECL
    iterator&
    operator--() noexcept;

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

private:
    inline
    void
    parse() noexcept;
};

using url = url;

} // urls
} // boost

#include <boost/url/impl/url.hpp>

#endif
