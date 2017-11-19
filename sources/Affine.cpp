#include "Affine.hpp"
#include "data_alpha.hpp"
#include <sstream>

int count_reverse_number(int num, unsigned int mode);
std::size_t count_remainder(int a, std::size_t m);
int count_GCD(int firstNumber, int secondNumber);

Affine::Affine() : cipher(),
                   frequency_table_(ENGLISH_LETTER_FREQUENCIES),
                   alphabet_(ENGLISH_ALPHABET),
                   alphabet_len_(ENGLISH_ALPHABET_LEN),
                   key_()
{}

void Affine::encrypt(const std::string& key) try // key = "a,b"
{
    key_ = parse_key(key);
    change_text_source();

    for (char c : text_source_)
        text_modified_+= static_cast<char>((key_.a * (c - alphabet_[0]) + key_.b) % alphabet_len_ + alphabet_[0]);
}
catch(const std::exception& ex)
{
    std::cout << ex.what();
}

void Affine::decrypt(const std::string& key) try
{
    key_ = parse_key(key);
    change_text_source();
    text_modified_ = decrypt(key_);
}
catch(const std::exception& ex)
{
    std::cout << ex.what();
}

void Affine::crack()
{
    change_text_source();
    double max = 0.0;
    std::string current;

    for (std::size_t i = 1; i < alphabet_len_; i += 2)
    {
        if (count_GCD(i, alphabet_len_) != 1)
            continue;

        for (std::size_t j = 0; j < alphabet_len_; j++)
        {
            current = decrypt(Key(i, j));
            auto val = count_coefficient(current);
            if (val > max)
            {
                max = val;
                text_modified_ = current;
            }
        }
    }
}

Affine::Key::Key(int a, int b) : a(a), b(b) {}

std::string Affine::decrypt(const Key& key) const
{
    std::string res;
    auto rev_a = count_reverse_number(key.a, alphabet_len_);

    for (char c : text_source_)
        res += static_cast<char>(count_remainder(rev_a * (c - alphabet_[0] - key.b), alphabet_len_) + alphabet_[0]);

    return res;
}

Affine::Key Affine::parse_key(const std::string& key)
{
    auto found = key.find(',');
    if (found == std::string::npos)
        throw std::invalid_argument("Invalid key");

    std::istringstream iss(key.substr(0, found));
    int a;
    if (!(iss >> a))
        throw std::invalid_argument("Invalid key");

    if (count_GCD(a, static_cast<int>(alphabet_len_)) != 1)
        throw std::invalid_argument("Invalid key: a and alphabet length must be comprime integers");

    iss =  std::istringstream(key.substr(found + 1, key.length() - found - 1));
    int b;
    if (!(iss >> b))
        throw std::invalid_argument("Invalid key");

    return {a, b};
}

void Affine::change_text_source()
{
    std::string new_text_source;

    for (char c : text_source_)
        if (from_this_alphabet(static_cast<char>(tolower(c))))
            new_text_source += static_cast<char>(tolower(c));

    text_source_ = new_text_source;
}

bool Affine::from_this_alphabet(char letter) const
{
    for (std::size_t i = 0; i < alphabet_len_; ++i)
        if (letter == alphabet_[i])
            return true;

    return false;
}

double Affine::count_coefficient(const std::string& text) const
{
    auto frequencies = new double[alphabet_len_];
    for (std::size_t i = 0; i < alphabet_len_; i++)
        frequencies[i] = 0;

    for (char c : text)
        ++frequencies[c - alphabet_[0]];

    for (auto i = 0; i < alphabet_len_; i++)
        frequencies[i] /= text.length();

    double res = 0.0;
    for (std::size_t i = 0; i < alphabet_len_; i++)
        res += frequency_table_[i] * frequencies[i];

    delete[] frequencies;
    return res;
}

void Extended_euclid(int a, int b, int& x, int& y, int& d)
{
    int q, r, x1, x2, y1, y2;

    if (b == 0)
    {
        d = a; x = 1; y = 0;
        return;
    }

    x2 = 1; x1 = 0; y2 = 0; y1 = 1;

    while (b > 0)
    {
        q = a / b;
        r = a - q * b;
        x = x2 - q * x1;
        y = y2 - q * y1;
        a = b;
        b = r;
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }

    d = a;
    x = x2;
    y = y2;
}

int count_reverse_number(int num, unsigned int mode)
{
    int d, x, y;

    Extended_euclid(num, mode, x, y, d);

    if (d == 1)
        return (x >= 0) ? x : x + mode;

    return 0;
}

std::size_t count_remainder(int a, std::size_t m)
{
    int res = a % static_cast<int>(m);
    return (res >= 0) ? res : res + m;
}

int count_GCD(int firstNumber, int secondNumber)
{
    while (firstNumber != 0 && secondNumber != 0)
        if (firstNumber > secondNumber)
            firstNumber = firstNumber % secondNumber;
        else
            secondNumber = secondNumber % firstNumber;

    return firstNumber + secondNumber;
}