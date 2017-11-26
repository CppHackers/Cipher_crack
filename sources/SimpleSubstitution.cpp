#include "SimpleSubstitution.hpp"
#include "data_alphabet.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

template<typename T>
std::size_t find_first_index(const T* arr, std::size_t len, T value);
std::size_t find_first_index(const std::vector<double>& arr, double value);
std::size_t find_first_index(const std::string& str, char value);

SimpleSubstitution::SimpleSubstitution() : Cipher(),
                                           frequency_table_(ENGLISH_LETTER_FREQUENCIES),
                                           alphabet_(ENGLISH_ALPHABET),
                                           alphabet_len_(ENGLISH_ALPHABET_LEN)
{
    bigrams_freq_table_ = SimpleSubstitution::str_dbl_map();
    current_bigrams_freq_table_ = SimpleSubstitution::str_dbl_map();
}

void SimpleSubstitution::encrypt(const std::string& key)
{
    if (check_key(key)) key_ = key;
    else throw std::invalid_argument("Incorrect key");
    change_text_source();

    for (char c : text_source_)
        text_modified_ += key[find_first_index(alphabet_, alphabet_len_, c)];
}

void SimpleSubstitution::decrypt(const std::string& key)
{
    if (check_key(key)) key_ = key;
    else throw std::invalid_argument("Incorrect key");
    change_text_source();
    text_modified_ = decr(key_);
}

void SimpleSubstitution::crack()
{
    // Primary key calculation
    std::vector<double> current_freqs_table(alphabet_len_, 0.0); // letters' frequencies of cipher text

    for (char c : text_source_)
        current_freqs_table[find_first_index(alphabet_, alphabet_len_, c)] += 1.0;

    for (auto i = 0; i < alphabet_len_; ++i)
    {
        auto l = text_source_.length();
        current_freqs_table[i] = current_freqs_table[i] / text_source_.length() * 100.0;
        for (auto j = 0; j < i; ++j)
            if (current_freqs_table[i] == current_freqs_table[j])
                current_freqs_table[i] -= 0.00000000001; // to exclude equal frequencies
    }

    std::vector<double> current_freqs_sorted(alphabet_len_); // letters' frequencies of cipher text, sorted by descending frequency
    for (auto i = 0; i < alphabet_len_; ++i)
        current_freqs_sorted[i] = current_freqs_table[i];
    std::sort(current_freqs_sorted.begin(), current_freqs_sorted.end(), std::greater<double>());

    std::vector<double> freq_table_sorted(alphabet_len_); // standart frequency table, sorted by descending frequency
    for (auto i = 0; i < alphabet_len_; ++i)
        freq_table_sorted[i] = (double)frequency_table_[i];
    std::sort(freq_table_sorted.begin(), freq_table_sorted.end(), std::greater<double>());

    std::string key(alphabet_len_, ' ');

    // forming the most suitable primary key (based on letters' frequencies of cipher text)
    for (auto i = 0; i < alphabet_len_; ++i)
    {
        auto symb = alphabet_[find_first_index(current_freqs_table, current_freqs_sorted[i])];
        key[find_first_index(frequency_table_, alphabet_len_, static_cast<float>(freq_table_sorted[i]))] = symb;
    }

    // decrypting text_source_ with primary key
    auto text = decr(key);

    // counting bigrams coefficient for text
    load_bigrams_freq();

    double bigrams_rating = count_bigrams_coefficient(text, bigrams_freq_table_);
    auto prev_bigrams_rating = bigrams_rating;
    auto step = 1;

    // trying to find key with the lowest bigrams_rating rearranging 2 letters (using step) in the key
    while (true)
    {
        auto find_good_key = false;

        for (auto i = 0; i < alphabet_len_; ++i)
        {
            if (i + step >= alphabet_len_)
                break;

            auto q = find_first_index(key, alphabet_[find_first_index(current_freqs_table, current_freqs_sorted[i])]);
            auto p = find_first_index(key, alphabet_[find_first_index(current_freqs_table, current_freqs_sorted[i + step])]);
            std::swap(key[q], key[p]);

            text = decr(key);
            bigrams_rating = count_bigrams_coefficient(text, bigrams_freq_table_);

            if (bigrams_rating < prev_bigrams_rating)
            {
                find_good_key = true;
                prev_bigrams_rating = bigrams_rating;
                step = 1;
                break;
            }

            std::swap(key[q], key[p]);
        }

        if (!find_good_key)
        {
            ++step;
            if (step >= alphabet_len_ - 1)
                break;
        }
    }

    text_modified_ = decr(key);
}

bool SimpleSubstitution::check_key(const std::string& key) noexcept
{
    for (auto i = 1; i < key.length(); ++i)
        for (auto j = 0; j < i; ++j)
            if (key[i] == key[j])
                return false;
    return true;
}

void SimpleSubstitution::change_text_source()
{
    std::string new_text_source;

    for (char c : text_source_)
        if (from_this_alphabet(static_cast<char>(tolower(c))))
            new_text_source += static_cast<char>(tolower(c));

    text_source_ = new_text_source;
}

bool SimpleSubstitution::from_this_alphabet(char letter) const noexcept
{
    for (std::size_t i = 0; i < alphabet_len_; ++i)
        if (letter == alphabet_[i])
            return true;
    return false;
}

std::string SimpleSubstitution::decr(const std::string& key) const
{
    std::string res;
    for (char c : text_source_)
        res += alphabet_[find_first_index(key.c_str(), key.length(), c)];
    return res;
}

void SimpleSubstitution::load_bigrams_freq()
{
    std::ifstream ifs("../sources/english_bigrams.txt");
    std::string str;

    while(std::getline(ifs, str))
    {
        double key = 0.0;
        std::istringstream sstream(str.substr(3, str.length() - 3));
        sstream >> key;
        bigrams_freq_table_.insert(std::pair<std::string,double>(str.substr(0, 2), key));
        current_bigrams_freq_table_.insert(std::pair<std::string,double>(str.substr(0, 2), 0.0));
    }

    ifs.close();
}

double SimpleSubstitution::count_bigrams_coefficient(const std::string& text, const SimpleSubstitution::str_dbl_map& bigrams_freq_table)
{
    for (std::size_t i = 1; i < text.length(); ++i)
    {
        std::string temp = std::string(1, text[i - 1]) + std::string(1, text[i]);
        if (current_bigrams_freq_table_.find(temp) == current_bigrams_freq_table_.end())
            current_bigrams_freq_table_.insert(std::pair<std::string,double>(temp, 1.0));
        else
            current_bigrams_freq_table_[temp] += 1.0;
    }

    double bigrams_rating = 0.0;

    for (auto& current_bigram_freq : current_bigrams_freq_table_)
    {
        current_bigram_freq.second = (current_bigram_freq.second / (text.length() - 1)) * 100.0;
        if (bigrams_freq_table.find(current_bigram_freq.first) == bigrams_freq_table.end())
            bigrams_rating += pow(current_bigram_freq.second, 2.0);
        else
            bigrams_rating += pow(current_bigram_freq.second - bigrams_freq_table.at(current_bigram_freq.first), 2.0);
    }

    return bigrams_rating;
}

template<typename T>
std::size_t find_first_index(const T* arr, std::size_t len, T value)
{
    for (std::size_t i = 0; i < len; ++i)
        if (arr[i] == value)
            return i;
    return len;
}

std::size_t find_first_index(const std::vector<double>& arr, double value)
{
    for (std::size_t i = 0; i < arr.size(); ++i)
        if (arr[i] == value)
            return i;
    return arr.size();
}

std::size_t find_first_index(const std::string& str, char value)
{
    for (std::size_t i = 0; i < str.length(); ++i)
        if (str[i] == value)
            return i;
    return str.length();
}