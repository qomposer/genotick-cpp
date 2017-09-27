
// Apparently Microsoft forgot to define a symbol for codecvt.

#include <locale>

#if (_MSC_VER >= 1900 /* VS 2015*/) && (_MSC_VER <= 1911 /* VS 2017 */)
std::locale::id std::codecvt<char16_t, char, _Mbstatet>::id;
#endif
