#pragma once
#include <iostream>
#include <cstdint>

typedef unsigned int uint;

#define one_block_size_bytes 64 // количество байб в блоке
#define one_block_size_uints 16 // количество 4байтовых  в блоке
#define block_expend_size_uints 80 // количество 4байтовых в дополненном блоке

#define SHA1_HASH_LENGTH_BYTES 20
#define SHA1_HASH_LENGTH_UINTS 5

typedef uint* Block;
typedef uint ExpendBlock[block_expend_size_uints];

const uint H[SHA1_HASH_LENGTH_UINTS] = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
}; // константы, согласно стандарту


struct SHA1PwdArray {
	//private:
		uint data[SHA1_HASH_LENGTH_UINTS];	// массив с хэшем пароля
	//public:
		SHA1PwdArray();
		SHA1PwdArray(const SHA1PwdArray& _otherSHA1PwdArray);
		SHA1PwdArray& operator = (const SHA1PwdArray& _otherSHA1PwdArray);
		bool operator == (const SHA1PwdArray& _otherSHA1PwdArray) const;
		bool operator != (const SHA1PwdArray& _otherSHA1PwdArray) const;
		uint& operator[](const int _index);
};


uint cycle_shift_left(uint val, int bit_count);
uint bring_to_human_view(uint val);
void sha1(SHA1PwdArray& _shaPwdArray, const char* message, uint msize_bytes);

