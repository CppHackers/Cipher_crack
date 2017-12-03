#include "Caesar.hpp"
#include "catch.hpp"

SCENARIO("Caesar: decrypted text and cracked text must be equal")
{
    GIVEN("encrypted text")
    {
        Caesar a;
        a.set_text_source("Rising fuel costs have impacted everything from the price of groceries to the cost of getting to work everyday.");
        a.encrypt("7");
        auto enc_text = a.get_text_modified();
        WHEN("decrypt encrypted text and crack encrypted text")
        {
            a.set_text_source(enc_text);
            a.decrypt("7");
            auto dec_text = a.get_text_modified();
            a.crack();
            auto cracked_text = a.get_text_modified();
            THEN("texts must be equal")
            {
                REQUIRE(dec_text == cracked_text);
            }
        }
    }
}