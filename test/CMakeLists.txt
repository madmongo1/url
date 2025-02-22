#
# Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
# Copyright (c) 2021 DMitry Arkhipov (grisumbras@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/CPPAlliance/url
#

if(NOT TARGET tests)
    add_custom_target(tests)
    set_property(TARGET tests PROPERTY FOLDER _deps)
endif()

set(BOOST_URL_TESTS_FILES
    CMakeLists.txt
    Jamfile
    include/main.cpp
    include/test_suite.hpp
    include/test_bnf.hpp
    _detail_char_type.cpp
    _detail_parse.cpp
    error.cpp
    host_type.cpp
    ipv4_address.cpp
    ipv6_address.cpp
    path_view.cpp
    query_params_view.cpp
    sandbox.cpp
    scheme.cpp
    static_pool.cpp
    storage_ptr.cpp
    string.cpp
    url.cpp
    url_view.cpp
    urls.cpp
    bnf/char_set.cpp
    bnf/parse.cpp
    bnf/range.cpp
    bnf/repeat.cpp
    bnf/token.cpp
    rfc/absolute_uri_bnf.cpp
    rfc/authority_bnf.cpp
    rfc/char_sets.cpp
    rfc/fragment_bnf.cpp
    rfc/hier_part_bnf.cpp
    rfc/host_bnf.cpp
    rfc/ip_literal_bnf.cpp
    rfc/ipv_future_bnf.cpp
    rfc/ipv4_address_bnf.cpp
    rfc/ipv6_address_bnf.cpp
    rfc/pct_encoded_bnf.cpp
    rfc/pct_encoding.cpp
    rfc/port_bnf.cpp
    rfc/query_bnf.cpp
    rfc/relative_part_bnf.cpp
    rfc/scheme_bnf.cpp
    rfc/uri_bnf.cpp
    rfc/userinfo_bnf.cpp
    )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${BOOST_URL_TESTS_FILES})
add_executable(boost_url_tests ${BOOST_URL_TESTS_FILES})
target_include_directories(boost_url_tests PRIVATE . ./include)
target_link_libraries(boost_url_tests PRIVATE Boost::url)
add_test(NAME boost_url_tests COMMAND boost_url_tests)
add_dependencies(tests boost_url_tests)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../src PREFIX "" FILES ../src/src.cpp)
