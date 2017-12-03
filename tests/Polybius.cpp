#include "Polybius.hpp"
#include "catch.hpp"

SCENARIO("Polybius: decrypted text and cracked text must be equal")
{
    GIVEN("encrypted text")
    {
        Polybius a;
        a.set_text_source("The current severe economic downturn in America affects people on every level. Rising fuel costs have impacted everything from the price of groceries to the cost of getting to work everyday. Due to this economy issue, there are the changes in the way people spend their leisure time. There are media reports that tell about the trend away from vacations and toward \"staycations\", where people stay at home instead of travelling. At the same time, there's a significant trend toward finding recreational activities. They are called crafts for kids, quilting, and gardening.\n"
                                  "Parents and grandparents are always on the lookout for kids' craft projects that will engage children and keep those little hands busy. Thankfully there are many inexpensive books that are filled with ideas. For example, you can find books that outline crafts using play dough, how to make flower figures, and even Christmas crafts. In fact, you can even find books that provide ideas for making Christmas gifts that fit in a jar. From crafts for preschool-age children to pastimes tor older kids these idea books provide a wealth of options for engaging children's imaginations."
                                  "As for adults, two trends are emerging. Knitting and sewing were popular during the past decade. The latest pastime appreciated by American adults is quilting. Hobbyists are bringing it back tо lite and enjoying the detail and craft involved in making a quilt. There is a wealth of helpful new books on quilting projects, including how-to books, quilting patterns, and quilting designs.");
        a.encrypt("qwertyuiopasdfghklzxcvbnm");
        auto enc_text = a.get_text_modified();
        WHEN("decrypt encrypted text and crack encrypted text")
        {
            a.set_text_source(enc_text);
            a.decrypt("qwertyuiopasdfghklzxcvbnm");
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