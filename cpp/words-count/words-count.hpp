#ifndef PROJECT_WORDS_COUNT_HPP
#define PROJECT_WORDS_COUNT_HPP

#include <immintrin.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cctype>

size_t CountWordsNaive(const char *data, size_t length) {
    size_t count = isalpha(data[0]) ? 1 : 0;
    for (size_t i = 1; i < length; i++) {
        if (isalpha(data[i]) && !isalpha(data[i - 1])) {
            count++;
        }
    }
    return count;
}

int GetBlockSum(const __m128i &block) {
    auto elements = std::vector<int>{
        _mm_extract_epi8(block, 0),  _mm_extract_epi8(block, 1),  _mm_extract_epi8(block, 2),  _mm_extract_epi8(block, 3),
        _mm_extract_epi8(block, 4),  _mm_extract_epi8(block, 5),  _mm_extract_epi8(block, 6),  _mm_extract_epi8(block, 7),
        _mm_extract_epi8(block, 8),  _mm_extract_epi8(block, 9),  _mm_extract_epi8(block, 10), _mm_extract_epi8(block, 11),
        _mm_extract_epi8(block, 12), _mm_extract_epi8(block, 13), _mm_extract_epi8(block, 14), _mm_extract_epi8(block, 15)};
    auto sum = 0;
    for (auto && element : elements) {
        if (element != 0) {
            sum += 256 - element;
        }
    }
    return sum;
}

int CountWordsSimd(const char *data, size_t length) {
    auto count_block = _mm_setzero_si128();
    auto to_upper_mask = _mm_set1_epi8(' ');
    auto min_letter_mask = _mm_set1_epi8('a' - 1);
    auto max_letter_mask = _mm_set1_epi8('z' + 1);
    int count = 0;
    for (size_t i = 0; i + 16 <= length; i += 16) {

        auto block = _mm_loadu_si128((const __m128i *)(data + i));
        auto upper_block = _mm_or_si128(block, to_upper_mask);
        auto mask = _mm_and_si128(_mm_cmplt_epi8(min_letter_mask, upper_block), _mm_cmplt_epi8(upper_block, max_letter_mask));
        mask = _mm_xor_si128(mask, _mm_bslli_si128(mask, 1));

        count_block = _mm_add_epi8(count_block, mask);
        if (i > 0 && isalpha(data[i - 1])) {
            count += isalpha(data[i]) ? -1 : 1;
        }
        if ((i + 16) % 2048 == 0) {
            count += GetBlockSum(count_block);
            count_block = _mm_setzero_si128();
        }
    }
    for (size_t i = length - length % 16; i < length; i++) {
        if (isalpha(data[i]) != isalpha(data[i - 1])) {
            count++;
        }
    }
    if (isalpha(data[length - 1])) {
        count++;
    }
    return (count + GetBlockSum(count_block)) / 2;
}

#endif  // PROJECT_WORDS_COUNT_HPP
