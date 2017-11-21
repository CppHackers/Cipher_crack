#ifndef AFFINE_HPP
#define AFFINE_HPP

#include "cipher.hpp"

class Affine : public cipher
{
public:
    Affine();

    void encrypt(const std::string& key) override;
    void decrypt(const std::string& key) override;
    void crack() override;

private:
    struct Key
    {
        int a;
        int b;
        Key(int a = 0, int b = 0);
    };

    float * frequency_table_;
    char * alphabet_;
    std::size_t alphabet_len_;
    Key key_;
    std::string decrypt(const Key& key) const;
    Key parse_key(const std::string& key) noexcept(false);
    void change_text_source();
    bool from_this_alphabet(char letter) const;
    double count_coefficient(const std::string& text) const;
};

#endif // AFFINE_HPP