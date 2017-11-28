#include "Polybius.hpp"
#include "data_alphabet.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

Polybius::Polybius() : Cipher(),
                       alphabet_len_(ENGLISH_ALPHABET_LEN - 1),
                       key_(5, 5),
                       bigrams_freq_table_(),
                       current_bigrams_freq_table_()
{
    alphabet_ = std::vector<char>(alphabet_len_);
    int j = 0;
    for (auto i = 0; i < alphabet_len_; ++i) // for english alphabet
    {
        if (ENGLISH_ALPHABET[i] == 'j') j = 1;
        alphabet_[i] = ENGLISH_ALPHABET[i + j];
    }

    frequency_table_ = std::vector<double>(alphabet_len_);
    j = 0;
    for (auto i = 0; i < alphabet_len_; ++i) // for english alphabet
    {
        if (i == 9) j = 1;
        frequency_table_[i] = ENGLISH_LETTER_FREQUENCIES[i + j];
    }
}

void Polybius::encrypt(const std::string& key)
{
    if (!check_key(key)) throw std::invalid_argument("Incorrect key");
    key_ = parse_key(key);
    change_text_source();

    for (char c : text_source_)
    {
        int row = 0;
        int col = 0;
        for (auto j = 0; j < key_.rows; ++j)
        {
            for (auto k = 0; k < key_.cols; ++k)
            {
                if (key_.matrix[j][k] == c)
                {
                    row = j;
                    col = k;
                    break;
                }
            }
        }
        text_modified_ += alphabet_[row];
        text_modified_ += alphabet_[col];
    }
}

void Polybius::decrypt(const std::string& key)
{
    if (!check_key(key)) throw std::invalid_argument("Incorrect key");
    key_ = parse_key(key);
    change_text_source();
    auto length = text_source_.length() / 2;
    for (auto i = 0; i < length; ++i)
        text_modified_ += key_.matrix[text_source_[2 * i] - alphabet_[0]][text_source_[2 * i + 1] - alphabet_[0]];
}

void Polybius::crack() // for english alphabet
{
    // Primary key calculation
    std::vector<double> current_freqs_table(alphabet_len_, 0.0); // letters' frequencies of cipher text (0 - aa, 1 - ab, 2 - ac...)

    auto l = text_source_.length() / 2;
    for (auto i = 0; i < l; ++i)
    {
        auto index = (text_source_[2 * i] - alphabet_[0]) * 5 + text_source_[2 * i + 1] - alphabet_[0];
        current_freqs_table[index] += 1.0;
    }

    for (auto i = 0; i < alphabet_len_; ++i)
    {
        current_freqs_table[i] = current_freqs_table[i] / l * 100.0;
        for (auto j = 0; j < i; ++j)
            if (current_freqs_table[i] == current_freqs_table[j])
                current_freqs_table[i] -= 0.00000000001; // to exclude equal frequencies
    }

    auto current_freqs_sorted = current_freqs_table; // letters' frequencies of cipher text, sorted by descending frequency
    std::sort(current_freqs_sorted.begin(), current_freqs_sorted.end(), std::greater<double>());

    auto freq_table_sorted = frequency_table_; // standart frequency table, sorted by descending frequency
    std::sort(freq_table_sorted.begin(), freq_table_sorted.end(), std::greater<double>());

    std::string key(alphabet_len_, ' ');

    // forming the most suitable primary key (based on letters' frequencies of cipher text)
    for (auto i = 0; i < alphabet_len_; ++i)
    {
        auto symb = alphabet_[std::find(frequency_table_.begin(), frequency_table_.end(), freq_table_sorted[i]) - frequency_table_.begin()];
        key[std::find(current_freqs_table.begin(), current_freqs_table.end(), current_freqs_sorted[i]) - current_freqs_table.begin()] = symb;
    }

    // decrypting text_source_ with primary key
    auto text = decr(key);

    // counting bigrams coefficient for text
    load_bigrams_freq();

    auto bigrams_rating = count_bigrams_coefficient(text);
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

            auto q = key.find_first_of(alphabet_[std::find(current_freqs_table.begin(), current_freqs_table.end(), current_freqs_sorted[i]) - current_freqs_table.begin()]);
            auto p = key.find_first_of(alphabet_[std::find(current_freqs_table.begin(), current_freqs_table.end(), current_freqs_sorted[i + step]) - current_freqs_table.begin()]);
            std::swap(key[q], key[p]);

            text = decr(key);
            bigrams_rating = count_bigrams_coefficient(text);

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

Polybius::Key::Key(std::size_t r, std::size_t c)
{
    rows = r;
    cols = c;
    matrix = std::vector<std::vector<char>>(rows);
    for (auto i = 0; i < rows; ++i)
        matrix[i] = std::vector<char>(cols);
}

bool Polybius::check_key(const std::string& key) const noexcept
{
    if (key.length() != alphabet_len_ || !from_this_alphabet(key[0]))
        return false;

    for (auto i = 1; i < key.length(); ++i)
    {
        if (!from_this_alphabet(key[i])) return false;
        for (auto j = 0; j < i; ++j)
            if (key[i] == key[j]) return false;
    }

    return true;
}

Polybius::Key Polybius::parse_key(const std::string& key) const noexcept
{
    Key res(5, 5);

    for (auto i = 0; i < res.rows; i++)
        for (auto j = 0; j < res.cols; j++)
            res.matrix[i][j] = key[i * res.rows + j];

    return res;
}

void Polybius::change_text_source() noexcept
{
    std::string new_text_source;

    for (char c : text_source_)
        if (from_this_alphabet((char)tolower(c)))
            new_text_source += (char)tolower(c);
        else if (tolower(c) == 'j') new_text_source += 'i';

    text_source_ = new_text_source;
}

bool Polybius::from_this_alphabet(char letter) const noexcept
{
    for (std::size_t i = 0; i < alphabet_len_; ++i)
        if (letter == alphabet_[i])
            return true;
    return false;
}

std::string Polybius::decr(const std::string& key) const noexcept
{
    std::string res;
    Key k = parse_key(key);
    auto length = text_source_.length() / 2;
    for (auto i = 0; i < length; ++i)
        res += k.matrix[text_source_[2 * i] - alphabet_[0]][text_source_[2 * i + 1] - alphabet_[0]];
    return res;
}

void Polybius::load_bigrams_freq() noexcept
{
    std::ifstream ifs("../sources/english_bigrams.txt");
    std::string str;

    while(std::getline(ifs, str))
    {
        double key = 0.0;
        std::istringstream sstream(str.substr(3, str.length() - 3));
        sstream >> key;
        std::string bigram = str.substr(0, 2);
        if (bigram[0] != 'j' && bigram[1] != 'j')
        {
            bigrams_freq_table_.insert(std::pair<std::string,double>(bigram, key));
            current_bigrams_freq_table_.insert(std::pair<std::string,double>(bigram, 0.0));
        }
    }

    ifs.close();
}

double Polybius::count_bigrams_coefficient(const std::string& text) noexcept
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
        if (bigrams_freq_table_.find(current_bigram_freq.first) == bigrams_freq_table_.end())
            bigrams_rating += pow(current_bigram_freq.second, 2.0);
        else
            bigrams_rating += pow(current_bigram_freq.second - bigrams_freq_table_.at(current_bigram_freq.first), 2.0);
    }

    return bigrams_rating;
}