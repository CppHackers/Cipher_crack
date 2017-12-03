#ifndef SIMPLESUBSTITUTION_HPP
#define SIMPLESUBSTITUTION_HPP

#include <unordered_map>
#include "Cipher.hpp"

class SimpleSubstitution : public Cipher
{
public:
    SimpleSubstitution(unsigned log_level = Log::Debug);

    void encrypt(const std::string& key) override;
    void decrypt(const std::string& key) override;
    void crack() override;

private:
    typedef std::unordered_map<std::string, double> str_dbl_map;

    float * frequency_table_;
    char * alphabet_;
    std::size_t alphabet_len_;
    std::string key_;
    str_dbl_map bigrams_freq_table_;
    str_dbl_map current_bigrams_freq_table_;

    bool check_key(const std::string& key) const noexcept;
    void change_text_source() noexcept;
    bool from_this_alphabet(char letter) const noexcept;
    double count_bigrams_coefficient(const std::string& text) noexcept;
    std::string decr(const std::string& key) const noexcept;
    void load_bigrams_freq() noexcept;
};

#endif // SIMPLESUBSTITUTION_HPP