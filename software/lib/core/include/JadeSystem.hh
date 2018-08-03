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
#define BE16TOH ntohs
#define BE32TOH ntohl
#define BE64TOH ntohll
// Windows desktop is only running on little-endian
#define LE16TOH
#define LE32TOH
#define LE64TOH

#ifndef JADE_DLL_EXPORT
#define DLLEXPORT __declspec(dllimport)
#else
#define DLLEXPORT __declspec(dllexport)
#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#endif

#else
//TODO: does it work for MacOS, remove?
#ifdef __APPLE__
#include <machine/endian.h>
#if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
#define BE16TOH NTOHS
#define BE32TOH NTOHL
#define BE64TOH NTOHLL
#define LE16TOH
#define LE32TOH
#define LE64TOH
#endif
#else

#include <endian.h>
#define BE16TOH be16toh
#define BE32TOH be32toh
#define BE64TOH be64toh
#define LE16TOH le16toh
#define LE32TOH le32toh
#define LE64TOH le64toh
#endif

using std::size_t;
#define DLLEXPORT
#endif

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#endif
