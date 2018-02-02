#ifndef JADE_JADEPLATFORM_HH
#define JADE_JADEPLATFORM_HH

#include <cstddef>
#include <cstdint>
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

// using std::char16_t;
// using std::char32_t;

#ifdef _WIN32
#include <crtdefs.h>
#include <winsock2.h>
#include <ws2tcpip.h>
using be16toh = ntohs;
using be32toh = ntohl;
using be64toh = ntohll;
#ifndef JADE_DLL_EXPORT
#define DLLEXPORT __declspec(dllimport)
#else
#define DLLEXPORT __declspec(dllexport)
#endif

#else
using std::size_t;
#include <endian.h>
#define DLLEXPORT
#endif

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#endif
