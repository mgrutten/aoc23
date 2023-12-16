#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <map>

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}

long hash(const std::string &step)
{
    long curr = 0;
    for (char c : step)
    {
        curr += c;
        curr *= 17;
        curr %= 256;
    }

    return curr;
}

void vec_remove(std::vector<std::pair<std::string, int>> &vec, const std::string &key)
{
    auto itr = std::remove_if(vec.begin(), vec.end(), [=](auto el)
                              { return el.first == key; });
    vec.erase(itr, vec.end());
}

std::vector<std::pair<std::string, int>>::iterator vec_find(std::vector<std::pair<std::string, int>> &vec, const std::string &key)
{
    return std::find_if(vec.begin(), vec.end(), [=](auto el)
                        { return el.first == key; });
}

void print_boxes(const std::map<long, std::vector<std::pair<std::string, int>>> &boxes)
{
    for (auto box : boxes)
    {
        std::cout << box.first << " - ";
        for (auto lens : box.second)
        {
            std::cout << lens.first << "=" << lens.second << ", ";
        }
        std::cout << "\n";
    }
}

int main()
{
    std::ifstream infile("input.txt");
    std::string line;
    std::getline(infile, line);

    std::vector<std::string> steps = split(line, ",");
    std::map<long, std::vector<std::pair<std::string, int>>> boxes;
    for (std::string step : steps)
    {
        std::size_t eq_ind = step.find("=");
        std::size_t minus_ind = step.find("-");
        std::string key = step.substr(0, std::min(eq_ind, minus_ind));

        long h = hash(key);

        auto box_itr = boxes.find(h);
        if (minus_ind != std::string::npos)
        {
            if (box_itr != boxes.end())
            {
                // Remove label from box
                vec_remove(box_itr->second, key);

                // Remove box if it's empty
                if (box_itr->second.empty())
                {
                    boxes.erase(h);
                }
            }
        }
        else
        {
            // op == '='
            char val = std::stoi(step.substr(std::min(eq_ind, minus_ind) + 1));
            if (box_itr != boxes.end())
            {
                // Box exists
                auto lens_itr = vec_find(box_itr->second, key);
                if (lens_itr != box_itr->second.end())
                {
                    // Replace lens in box
                    lens_itr->second = val;
                }
                else
                {
                    // Add new lens
                    box_itr->second.push_back({key, val});
                }
            }
            else
            {
                // Add new box with lens
                std::vector<std::pair<std::string, int>> new_vec;
                new_vec.push_back({key, val});
                boxes.insert({h, new_vec});
            }
        }
    }

    // print_boxes(boxes);
    long count = 0;
    for (auto box : boxes)
    {
        for (int lens = 0; lens < box.second.size(); ++lens)
        {
            count += (box.first + 1) * (lens + 1) * box.second.at(lens).second;
        }
    }
    std::cout << count << "\n";
}