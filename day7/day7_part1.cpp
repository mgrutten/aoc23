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
    {'J', 9},
    {'T', 8},
    {'9', 7},
    {'8', 6},
    {'7', 5},
    {'6', 4},
    {'5', 3},
    {'4', 2},
    {'3', 1},
    {'2', 0}};

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

    bool has_card_count(int count) const
    {
        for (std::pair<char, int> val : card_values)
        {
            if (std::count(cards.cbegin(), cards.cend(), val.first) == count)
            {
                return true;
            }
        }
        return false;
    }

    bool has_two_pairs() const
    {
        int pair_count = 0;
        for (std::pair<char, int> val : card_values)
        {
            if (std::count(cards.cbegin(), cards.cend(), val.first) == 2)
            {
                pair_count += 1;
            }
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
        if (l.has_card_count(5) && r.has_card_count(5))
        {
            // std::cout << "two fives" << std::endl;
            return ordered_hands(l, r);
        }
        else if (l.has_card_count(5))
        {
            // std::cout << "l five" << std::endl;
            return false;
        }
        else if (r.has_card_count(5))
        {
            // std::cout << "r five" << std::endl;
            return true;
        }
        else if (l.has_card_count(4) && r.has_card_count(4))
        {
            // std::cout << "two fours" << std::endl;
            return ordered_hands(l, r);
        }
        else if (l.has_card_count(4))
        {
            // std::cout << "l four" << std::endl;
            return false;
        }
        else if (r.has_card_count(4))
        {
            // std::cout << "r four" << std::endl;
            return true;
        }
        else if (l.has_card_count(3) && l.has_card_count(2) && r.has_card_count(3) && r.has_card_count(2))
        {
            // std::cout << "two full house" << std::endl;
            return ordered_hands(l, r);
        }
        else if (l.has_card_count(3) && l.has_card_count(2))
        {
            // std::cout << "l full house" << std::endl;
            return false;
        }
        else if (r.has_card_count(3) && r.has_card_count(2))
        {
            // std::cout << "r full house" << std::endl;
            return true;
        }
        else if (l.has_card_count(3) && r.has_card_count(3))
        {
            // std::cout << "two threes" << std::endl;
            return ordered_hands(l, r);
        }
        else if (l.has_card_count(3))
        {
            // std::cout << "l threes" << std::endl;
            return false;
        }
        else if (r.has_card_count(3))
        {
            // std::cout << "r threes" << std::endl;
            return true;
        }
        else if (l.has_two_pairs() && r.has_two_pairs())
        {
            // std::cout << "two two pairs" << std::endl;
            return ordered_hands(l, r);
        }
        else if (l.has_two_pairs())
        {
            // std::cout << "l two pairs" << std::endl;
            return false;
        }
        else if (r.has_two_pairs())
        {
            // std::cout << "r two pairs" << std::endl;
            return true;
        }
        else if (l.has_card_count(2) && r.has_card_count(2))
        {
            // std::cout << "two pairs" << std::endl;
            return ordered_hands(l, r);
        }
        else if (l.has_card_count(2))
        {
            // std::cout << "l pair" << std::endl;
            return false;
        }
        else if (r.has_card_count(2))
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
        //std::cout << lin << std::endl;
        std::vector<std::string> game_str = split(line, " ");
        std::istringstream istream(game_str[0]);
        Hand hand;
        istream >> hand;
        int bid = std::stoi(game_str[1]);
        // std::cout << game_str[0] << " " << game_str[1] << "\n";
        //std::cout << hand << " - " << bid << std::endl;

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
        //std::cout << hand.first << " - " << index << " - " << hand.second << std::endl;
        count += hand.second * index;
        index += 1;
    }
    std::cout << count << "\n";
}
