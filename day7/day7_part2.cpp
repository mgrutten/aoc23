#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

/*
enum class Card : char
{
    A = 'A',
    K = 'K',
    Q = 'Q',
    J = 'J',
    T = 'T',
    nine = '9',
    eight = '8',
    seven = '7',
    six = '6',
    five = '5',
    four = '4',
    three = '3',
    two = '2'
};
*/

static const std::map<char, int> card_values = {
    {'A', 12},
    {'K', 11},
    {'Q', 10},
    {'J', 0},
    {'T', 9},
    {'9', 8},
    {'8', 7},
    {'7', 6},
    {'6', 5},
    {'5', 4},
    {'4', 3},
    {'3', 2},
    {'2', 1}};

/*
std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << static_cast<char>(card);
    return os;
}

std::istream &operator>>(std::istream &is, Card &card)
{
    char cardc;
    is >> cardc;
    card = static_cast<Card>(cardc);
    return is;
}

bool operator<(const Card l, const Card r)
{
    return card_values.at(static_cast<char>(l)) < card_values.at(static_cast<char>(r));
}
*/

struct Hand
{
    std::vector<char> cards{'2', '2', '2', '2', '2'};

    Hand() {}

    Hand(std::string input)
    {
        for (int i = 0; i < 5; ++i)
        {
            cards.at(i) = input.at(i);
        }
    }

    bool has_card_count(int count) const
    {
        for (std::pair<char, int> val : card_values)
        {
            if (val.first != 'J')
            {
                if (std::count(cards.cbegin(), cards.cend(), val.first) == count)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool has_card_count(int count, char &card) const
    {
        for (std::pair<char, int> val : card_values)
        {
            if (val.first != 'J')
            {
                if (std::count(cards.cbegin(), cards.cend(), val.first) == count)
                {
                    card = val.first;
                    return true;
                }
            }
        }
        return false;
    }

    bool has_two_pairs() const
    {
        int pair_count = 0;
        for (std::pair<char, int> val : card_values)
        {
            if (val.first != 'J')
            {
                if (std::count(cards.cbegin(), cards.cend(), val.first) == 2)
                {
                    pair_count += 1;
                }
            }
        }
        return pair_count == 2;
    }

    bool has_two_pairs(char &card) const
    {
        int pair_count = 0;
        char max_card;
        int max_val = -1;
        for (std::pair<char, int> pair : card_values)
        {
            if (pair.first != 'J')
            {
                if (std::count(cards.cbegin(), cards.cend(), pair.first) == 2)
                {
                    int val = card_values.at(pair.first);
                    if (val > max_val)
                    {
                        max_card = pair.first;
                        max_val = val;
                    }
                    pair_count += 1;
                }
            }
        }

        if (pair_count == 2)
        {
            card = max_card;
        }

        return pair_count == 2;
    }

    int card_val(int index) const
    {
        return card_values.at(cards[index]);
    }

    friend bool ordered_hands(const Hand &l, const Hand &r)
    {
        std::vector<int> lcard_vals;
        std::transform(l.cards.begin(), l.cards.end(), std::back_inserter(lcard_vals), [](char c)
                       { return card_values.at(c); });

        std::vector<int> rcard_vals;
        std::transform(r.cards.begin(), r.cards.end(), std::back_inserter(rcard_vals), [](char c)
                       { return card_values.at(c); });

        return lcard_vals < rcard_vals;
    }

    Hand fill_jacks(char fill_card) const
    {
        Hand hand(*this);
        for (char &card : hand.cards)
        {
            if (card == 'J')
            {
                card = fill_card;
            }
        }
        return hand;
    }

    char highest() const
    {
        // Find non-J
        char non_j;
        int non_j_val = -1;
        for (char card : cards)
        {
            if (card != 'J')
            {
                int val = card_values.at(card);
                if (val > non_j_val)
                {
                    non_j = card;
                    non_j_val = val;
                }
            }
        }
        return non_j;
    }

    Hand jack_substitute() const
    {
        int num_jacks = std::count(cards.cbegin(), cards.cend(), 'J');
        if (num_jacks == 5)
        {
            // std::cout << "5 jacks"
            //           << "\n";
            Hand hand("AAAAA");
            return hand;
        }
        else if ((num_jacks == 4) || (num_jacks == 3))
        {
            // std::cout << "4 or 3 jacks"
            //           << "\n";
            return fill_jacks(highest());
        }
        else if (num_jacks == 2)
        {
            // std::cout << "2 jacks"
            //           << "\n";
            char match;
            if (has_card_count(3, match) || has_card_count(2, match))
            {
                return fill_jacks(match);
            }
            else
            {
                return fill_jacks(highest());
            }
        }
        else
        {
            // std::cout << "1 jack"
            //           << "\n";
            char match;
            if (has_card_count(4, match) || has_card_count(3, match) || has_two_pairs(match) || has_card_count(2, match))
            {
                return fill_jacks(match);
            }
            else
            {
                return fill_jacks(highest());
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Hand &hand)
    {
        for (char card : hand.cards)
        {
            os << card;
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Hand &hand)
    {
        for (char &card : hand.cards)
        {
            is >> card;
        }
        return is;
    }

    friend bool operator<(const Hand &l, const Hand &r)
    {
        // std::cout << "sub l"
        //           << "\n";
        Hand subs_l(l.jack_substitute());
        // std::cout << "sub r"
        //           << "\n";
        std::cout << l << " - " << subs_l << "\n";
        Hand subs_r(r.jack_substitute());
        // std::cout << "compare"
        //           << "\n";
        std::cout << r << " - " << subs_r << "\n";

        if (subs_l.has_card_count(5) && subs_r.has_card_count(5))
        {
            // std::cout << "two fives" << std::endl;
            return ordered_hands(l, r);
        }
        else if (subs_l.has_card_count(5))
        {
            // std::cout << "l five" << std::endl;
            return false;
        }
        else if (subs_r.has_card_count(5))
        {
            // std::cout << "r five" << std::endl;
            return true;
        }
        else if (subs_l.has_card_count(4) && subs_r.has_card_count(4))
        {
            // std::cout << "two fours" << std::endl;
            return ordered_hands(l, r);
        }
        else if (subs_l.has_card_count(4))
        {
            // std::cout << "l four" << std::endl;
            return false;
        }
        else if (subs_r.has_card_count(4))
        {
            // std::cout << "r four" << std::endl;
            return true;
        }
        else if (subs_l.has_card_count(3) && subs_l.has_card_count(2) && subs_r.has_card_count(3) && subs_r.has_card_count(2))
        {
            // std::cout << "two full house" << std::endl;
            return ordered_hands(l, r);
        }
        else if (subs_l.has_card_count(3) && subs_l.has_card_count(2))
        {
            // std::cout << "l full house" << std::endl;
            return false;
        }
        else if (subs_r.has_card_count(3) && subs_r.has_card_count(2))
        {
            // std::cout << "r full house" << std::endl;
            return true;
        }
        else if (subs_l.has_card_count(3) && subs_r.has_card_count(3))
        {
            // std::cout << "two threes" << std::endl;
            return ordered_hands(l, r);
        }
        else if (subs_l.has_card_count(3))
        {
            // std::cout << "l threes" << std::endl;
            return false;
        }
        else if (subs_r.has_card_count(3))
        {
            // std::cout << "r threes" << std::endl;
            return true;
        }
        else if (subs_l.has_two_pairs() && subs_r.has_two_pairs())
        {
            // std::cout << "two two pairs" << std::endl;
            return ordered_hands(l, r);
        }
        else if (subs_l.has_two_pairs())
        {
            // std::cout << "l two pairs" << std::endl;
            return false;
        }
        else if (subs_r.has_two_pairs())
        {
            // std::cout << "r two pairs" << std::endl;
            return true;
        }
        else if (subs_l.has_card_count(2) && subs_r.has_card_count(2))
        {
            // std::cout << "two pairs" << std::endl;
            return ordered_hands(l, r);
        }
        else if (subs_l.has_card_count(2))
        {
            // std::cout << "l pair" << std::endl;
            return false;
        }
        else if (subs_r.has_card_count(2))
        {
            // std::cout << "r pair" << std::endl;
            return true;
        }
        else
        {
            // std::cout << "both all different" << std::endl;
            return ordered_hands(l, r);
        }
    }
};

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}

int main()
{
    std::ifstream infile("input.txt");

    std::map<Hand, int> hands;
    std::string line;
    while (std::getline(infile, line))
    {
        // std::cout << lin << std::endl;
        std::vector<std::string> game_str = split(line, " ");
        std::istringstream istream(game_str[0]);
        Hand hand;
        istream >> hand;
        int bid = std::stoi(game_str[1]);
        // std::cout << game_str[0] << " " << game_str[1] << "\n";
        std::cout << hand << " - " << bid << std::endl;

        // std::cout << hand.has_card_count(3) << std::endl;
        // std::cout << hand.has_two_pairs() << std::endl;

        hands.insert({hand, bid});
    }
    std::cout << std::endl;

    /*
        std::istringstream h1stream("AAA22");
        Hand hand1;
        h1stream >> hand1;

        std::istringstream h2stream("AA22B");
        Hand hand2;
        h2stream >> hand2;

        std::cout << (hand1 < hand2) << "\n";
    */

    int index = 1;
    long count = 0;
    for (std::pair<Hand, int> hand : hands)
    {
        // std::cout << hand.first << " - " << index << " - " << hand.second << std::endl;
        count += hand.second * index;
        index += 1;
    }
    std::cout << count << "\n";
}
