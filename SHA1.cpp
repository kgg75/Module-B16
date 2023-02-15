#include "SHA1.h"

// функции структуры SHA1PwdArray
SHA1PwdArray::SHA1PwdArray() {
    memset(data, 0, SHA1_HASH_LENGTH_BYTES);
}


SHA1PwdArray::SHA1PwdArray(const SHA1PwdArray& _otherSHA1PwdArray) {
    memcpy(data, _otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES);	// копируем хэш пароля
}


SHA1PwdArray& SHA1PwdArray::operator = (const SHA1PwdArray& _otherSHA1PwdArray) {
    memcpy(data, _otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES);	// копируем хэш пароля
    return *this;
}


bool SHA1PwdArray::operator == (const SHA1PwdArray& _otherSHA1PwdArray) const {
    return memcmp(data, _otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES) == 0;
}


bool SHA1PwdArray::operator != (const SHA1PwdArray& _otherSHA1PwdArray) const {
    return memcmp(data, _otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES) != 0;
}


uint& SHA1PwdArray::operator[](const int _index) {
    return data[_index];
};
// конец блока функций структуры SHA1PwdArray


// циклический сдвиг влево
uint cycle_shift_left(uint val, int bit_count) {
    return (val << bit_count | val >> (32 - bit_count));
}


// перевернуть представление числа в памяти в человеческий вид
uint bring_to_human_view(uint val) {
    return  ((val & 0x000000FF) << 24) |
        ((val & 0x0000FF00) << 8) |
        ((val & 0x00FF0000) >> 8) |
        ((val & 0xFF000000) >> 24);
}


//void sha1(uint* _shaPwdArray, char* message, uint msize_bytes) {
void sha1(SHA1PwdArray& _shaPwdArray, const char* message, uint msize_bytes) {
    // вывод сообщения побайтово
    /*for (int i = 0; i < msize_bytes; i++) {
        std::cout << std::hex << (uint)message[i] << " ";
    }
    std::cout << std::endl;*/

    //инициализация
    uint A = H[0];
    uint B = H[1];
    uint C = H[2];
    uint D = H[3];
    uint E = H[4];

    // подсчет целого числа блоков
    uint totalBlockCount = msize_bytes / one_block_size_bytes;

    // подсчет, сколько байт нужно, чтобы дополнить последний блок
    uint needAdditionalBytes = one_block_size_bytes - (msize_bytes - totalBlockCount * one_block_size_bytes);

    if (needAdditionalBytes < 8) {
        totalBlockCount += 2;
        needAdditionalBytes += one_block_size_bytes;
    }
    else {
        totalBlockCount += 1;
    }

    // размер дополненного по всем правилам сообщения
    uint extendedMessageSize = msize_bytes + needAdditionalBytes;

    // выделяем новый буфер и копируем в него исходный
    unsigned char* newMessage = new unsigned char[extendedMessageSize];
    memcpy(newMessage, message, msize_bytes);

    // первый бит ставим '1', остальные обнуляем
    newMessage[msize_bytes] = 0x80;
    memset(newMessage + msize_bytes + 1, 0, needAdditionalBytes - 1);

    // задаем длину исходного сообщения в битах
    uint* ptr_to_size = (uint*)(newMessage + extendedMessageSize - 4);
    *ptr_to_size = bring_to_human_view(msize_bytes * 8);

    ExpendBlock exp_block;
    //раунды поехали
    for (int i = 0; i < totalBlockCount; i++) {
        // берем текущий блок и дополняем его
        unsigned char* cur_p = newMessage + one_block_size_bytes * i;
        Block block = (Block)cur_p;

        // первые 16 4байтовых чисел
        for (int j = 0; j < one_block_size_uints; j++) {
            exp_block[j] = bring_to_human_view(block[j]);
        }
        // следующие 64...
        for (int j = one_block_size_uints; j < block_expend_size_uints; j++) {
            exp_block[j] =
                exp_block[j - 3] ^
                exp_block[j - 8] ^
                exp_block[j - 14] ^
                exp_block[j - 16];
            exp_block[j] = cycle_shift_left(exp_block[j], 1);
        }

        // инициализация 
        uint a = H[0];
        uint b = H[1];
        uint c = H[2];
        uint d = H[3];
        uint e = H[4];

        // пересчитываем
        for (int j = 0; j < block_expend_size_uints; j++) {
            uint f;
            uint k;
            // в зависимости от раунда считаем по-разному
            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            // перемешивание
            uint temp = cycle_shift_left(a, 5) + f + e + k + exp_block[j];
            e = d;
            d = c;
            c = cycle_shift_left(b, 30);
            b = a;
            a = temp;
        }

        // пересчитываем
        _shaPwdArray[0] = A + a;
        _shaPwdArray[1] = B + b;
        _shaPwdArray[2] = C + c;
        _shaPwdArray[3] = D + d;
        _shaPwdArray[4] = E + e;
    }
}