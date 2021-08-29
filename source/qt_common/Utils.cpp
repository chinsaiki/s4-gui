#include "qt_common/Utils.h"

namespace S4
{

#define DOT '.'
#define COMMA ','
#define MAX 50

std::string fmtStringNumComma(const std::string& num)   //TODO: not ready
{
    if (num.size() < 3) return num;

    char commas[MAX];                    // Where the result is
    char *dot = strchr((char*)num.data(), DOT); // do we have a DOT?
    char *src, *dst;                     // source, dest
    size_t dot_len = 0;
    if (dot)
    {                                         // Yes
        dot_len = strlen(dot);
        dst = commas + MAX - dot_len - 1; // set dest to allow the fractional part to fit
        strcpy(dst, dot);                     // copy that part
        //*dot = 0;                             // 'cut' that frac part in tmp
        src = --dot;                          // point to last non frac char in tmp
        dst--;                                // point to previous 'free' char in dest
    }
    else
    {                                              // No
        src = (char*)num.data() + num.size() - 1; // src is last char of our float string
        dst = commas + MAX - 1;                    // dst is last char of commas
        *dst-- = '\0';
    }

    size_t len = num.size() - dot_len;      // len is the mantissa size
    int cnt = 0;                  // char counter

    do
    {
        if (*src <= '9' && *src >= '0')
        { // add comma is we added 3 digits already
            if (cnt && !(cnt % 3))
                *dst-- = COMMA;
            cnt++; // mantissa digit count increment
        }
        *dst-- = *src--;
    } while (--len);
    dst++;
    return std::string(dst);
}

} // namespace S4
