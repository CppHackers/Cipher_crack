#include "SimpleSubstitution.hpp"

int main(int argc, char* argv[])
{
    SimpleSubstitution ss;
    std::string text = "I am always both amused and annoyed when I hear foreign people criticize British food. \"It's unimaginative,\" they say. \"It's boring, it's tasteless, and it's chips with everything and totally overcooked vegetables.\"\n"
            "\n"
            "I have a theory about British cooking, and I was interested to read that several famous cookery writers agree with me. My theory is this. Our basic ingredients, when fresh, are so full of flavor that we haven't had to invent sauces and complex recipes to disguise their natural taste. What can compare with fresh peas or new potatoes just boiled (not over boiled) and served with butter? Why drown spring lamb a wine or cream or yoghurt and spices, when with just one or two herbs it is absolutely delicious?\n"
            "\n"
            "If you ask foreigners to name some typically English dishes, they will probably say \"fish and chips\" and then stop. It is disappointing, but true, that there is no tradition in Britain of eating in restaurants, because our food doesn’t lend itself to such preparation. British cooking is found in the home, where it is possible to time the dishes to perfection. So it is difficult to find a good English restaurant with reasonable prices.\n"
            "\n"
            "It is for these reasons that we haven’t exported our dishes, but we have imported a surprising number from all over the world. In most cities in Britain you’ll also find Indian, Chinese, French and Italian restaurants. In London you’ll also find Indonesian, Lebanese, Iranian, German, Spanish, Mexican, Greek… Cynics will say that this is because we have no \"cuisine\" ourselves, but, well, you know what I think!";
    ss.set_text_source(text);
    try
    {
        ss.encrypt("poiuytrewqasdfghjklmnbvcxz");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    ss.text_modified_out(std::cout);
    std::cout << std::endl;

    ss.set_text_source(ss.get_text_modified());
    try
    {
        ss.decrypt("poiuytrewqasdfghjklmnbvcxz");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    ss.text_modified_out(std::cout);
    std::cout << std::endl;

    ss.set_text_source(text);
    try
    {
        ss.encrypt("poiuytrewqasdfghjklmnbvcxz");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    ss.set_text_source(ss.get_text_modified());
    ss.set_bigrams_or_trigrams(true);
    ss.crack();
    ss.text_modified_out(std::cout);
    std::cout << std::endl;
    return 0;
}