#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

inline uint32_t leftRotate(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32 - n));
}


string md5(const string& input) {
    string mutableInput = input;
 
    uint32_t s[] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
    uint32_t k[] = {
        0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
        0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
        0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
        0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
        0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
        0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
        0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
        0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
        0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
        0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
        0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
        0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
        0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
        0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
        0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
        0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
    };

  
    uint64_t ml = mutableInput.length() * 8;
    mutableInput.push_back((char)0x80); 
    while ((mutableInput.length() * 8) % 512 != 448) {
        mutableInput.push_back((char)0x00); 
    }


    for (int i = 0; i < 8; i++) {
        mutableInput.push_back((ml >> (i * 8)) & 0xFF);
    }

 
    uint32_t a0 = s[0], b0 = s[1], c0 = s[2], d0 = s[3];

    for (size_t i = 0; i < mutableInput.length(); i += 64) {
        uint32_t M[16];
        for (int j = 0; j < 16; j++) {
            M[j] = (mutableInput[i + j * 4] & 0xFF) |
                   ((mutableInput[i + j * 4 + 1] & 0xFF) << 8) |
                   ((mutableInput[i + j * 4 + 2] & 0xFF) << 16) |
                   ((mutableInput[i + j * 4 + 3] & 0xFF) << 24);
        }


        uint32_t A = a0, B = b0, C = c0, D = d0;


        for (int j = 0; j < 64; j++) {
            uint32_t F, g;
            if (j < 16) {
                F = (B & C) | ((~B) & D);
                g = j;
            } else if (j < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * j + 1) % 16;
            } else if (j < 48) {
                F = B ^ C ^ D;
                g = (3 * j + 5) % 16;
            } else {
                F = C ^ (B | (~D));
                g = (7 * j) % 16;
            }

            uint32_t temp = D;
            D = C;
            C = B;
            B = B + leftRotate((A + F + k[j] + M[g]), 7);
            A = temp;
        }


        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }

    stringstream ss;
    ss << hex << setw(8) << setfill('0') << a0
       << setw(8) << setfill('0') << b0
       << setw(8) << setfill('0') << c0
       << setw(8) << setfill('0') << d0;
    return ss.str();
}