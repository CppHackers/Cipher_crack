#ifndef SIMPLESUBSTITUTION_HPP
#define SIMPLESUBSTITUTION_HPP

#include <unordered_map>
#include "Cipher.hpp"

class SimpleSubstitution : public Cipher
{
public:
    SimpleSubstitution();

    void encrypt(const std::string& key) override;
    void decrypt(const std::string& key) override;
    void crack() override;
    void set_bigrams_or_trigrams(bool with_bigrams) noexcept;

private:
    typedef std::unordered_map<std::string, double> str_dbl_map;

    float * frequency_table_;
    char * alphabet_;
    std::size_t alphabet_len_;
    std::string key_;
    bool with_bigrams_;
    str_dbl_map ngrams_freq_table_;
    str_dbl_map current_ngrams_freq_table_;

    bool check_key(const std::string& key) noexcept;
    void change_text_source();
    bool from_this_alphabet(char letter) const noexcept;
    double count_ngrams_coefficient(const std::string& text, unsigned int n, const SimpleSubstitution::str_dbl_map& ngrams_freq_table);
    std::string decr(const std::string& key) const;
    void load_ngrams_freq(unsigned int n);
};

#endif // SIMPLESUBSTITUTION_HPP