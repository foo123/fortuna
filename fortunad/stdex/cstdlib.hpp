
//          Copyright Adam Mizerski 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STDEX_CSTDLIB_HPP
#define STDEX_CSTDLIB_HPP

#include <cstdlib>
#include <iosfwd>
#include <iostream>


namespace stdex {


/**
 * If you're using it in place other than top-level catch block,
 * then probably you're doing something wrong.
 */
inline
void die(const std::string& msg, int status_code = 1)
{
    std::cerr << "error: " << msg << std::endl;
    std::exit(status_code);
}


} // namespace stdex

#endif // STDEX_CSTDLIB_HPP
