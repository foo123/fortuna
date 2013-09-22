/*
Copyright (C) 2013 Adam Mizerski <adam@mizerski.pl>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
*/

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
