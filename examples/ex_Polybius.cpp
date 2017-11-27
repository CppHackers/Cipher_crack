#include "Polybius.hpp"

int main()
{
    Polybius p;
    std::string text = "If you read this story you'll be able to find a common language with any American. All of them are very proud of their Constitution.\n"
            "The USA Constitution was written by fifty-five men who met at Philadelphia Convention in 1787. In four months they wrote the Constitution which has lasted over 200 years!\n"
            "All were white men. The average age was forty-two. Many of these men had been leaders during the American Revolution. About three-fourths of them had served in Congress. Most were leaders in their states. Some were rich, but most were not. None were poor.\n"
            "There were no native Americans among the delegates. There were no women. There were no black men or slaves. Poor farmers were not present either.\n"
            "George Washington, James Madison, and Benjamin Franklin were the three important delegates to the Convention.\n"
            "George Washington came from Virginia. He was probably the most respected man in the country. As the commander-in-chief of the American army during the Revolution, he was a great hero to most people.\n"
            "Then he had retired to his plantation and would have liked to remain there. However, his friends told him he should attend the convention. They said his support was necessary to get a new constitution accepted by the people. Since Washington thought a stronger national government was necessary, he came to Philadelphia.\n"
            "James Madison is often called the \"Father of the Constitution\". His ideas about government greatly influenced the other delegates. He had already developed a written plan for the new government which he brought to Philadelphia. It was known as the Virginia plan and it called for a strong national government. Madison took notes during the meetings. Much of what we know about the Philadelphia Convention is based on his notes.\n"
            "Benjamin Franklin attended the convention as a delegate from Pennsylvania. He was 81 years old and in poor health. Like Washington, he was highly respected by the Americans. He had been a printer, inventor and writer. He had also helped the country develop good relations with other nations. At the convention, he encouraged the delegates to cooperate with each other and work hard to settle their differences. His support of the Constitution was important to the other delegates.\n"
            "Once the Framers reached these agreements, it was time to get down to work and create a constitution.\n"
            "Delegates from states with large populations believed that a state with more people should have more votes in Congress.\n"
            "During the long debates, the Framers could not reach a decision on this issue. Neither side was willing to give in. The delegates were almost ready to quit and go home. A special committee of one delegate from each state was formed to try and find a solution. The members of the committee worked hard to find a compromise a majority of the delegates would accept.";

    p.set_text_source(text);
    try
    {
        p.encrypt("poiuytrewqasdfghklmnbvcxz");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    p.text_modified_out(std::cout);
    std::cout << std::endl;

    p.set_text_source(p.get_text_modified());
    try
    {
        p.decrypt("poiuytrewqasdfghklmnbvcxz");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    p.text_modified_out(std::cout);
    std::cout << std::endl;

    p.set_text_source(text);
    try
    {
        p.encrypt("poiuytrewqasdfghklmnbvcxz");
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    p.set_text_source(p.get_text_modified());
    p.crack();
    p.text_modified_out(std::cout);
    std::cout << std::endl;

    return 0;
}