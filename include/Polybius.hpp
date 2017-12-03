#ifndef POLYBIUS_HPP
#define POLYBIUS_HPP

#include <unordered_map>
#include <vector>
#include "Cipher.hpp"

class Polybius : public Cipher
{
public:
    Polybius(unsigned log_level = Log::Debug);

    void encrypt(const std::string& key) override;
    void decrypt(const std::string& key) override;
    void crack() override;

private:
    typedef std::unordered_map<std::string, double> str_dbl_map;

    struct Key
    {
        std::vector<std::vector<char>> matrix;
        std::size_t rows;
        std::size_t cols;
        Key(std::size_t rows = 0, std::size_t cols = 0);
    };

    std::vector<double> frequency_table_;
    std::vector<char> alphabet_;
    std::size_t alphabet_len_;
    Key key_;
    str_dbl_map bigrams_freq_table_;
    str_dbl_map current_bigrams_freq_table_;

    bool check_key(const std::string& key) const noexcept;
    Key parse_key(const std::string& key) const noexcept;
    void change_text_source() noexcept;
    bool from_this_alphabet(char letter) const noexcept;
    double count_bigrams_coefficient(const std::string& text) noexcept;
    std::string decr(const std::string& key) const noexcept;
    void load_bigrams_freq() noexcept;
};

#endif // POLYBIUS_HPP
