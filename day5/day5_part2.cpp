#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <climits>
#include <map>

struct Range
{
    long start;
    long length;

    friend std::ostream &operator<<(std::ostream &os, const Range &range)
    {
        os << "(" << range.start << ", " << range.length << ")";
        return os;
    }
};

class RangeMap
{
private:
    std::map<long, long> segments = {{0, 0}};

public:
    void add_segment(long destination, long source, long length)
    {
        if (segments.count(source) > 0)
        {
            // Already have a segment here, need to change value
            segments.at(source) = destination - source;
        }
        else
        {
            segments.insert({source, destination - source});
        }

        // Add zero at the end of the segment if there isn't already something there
        if (segments.count(source + length) == 0)
        {
            segments.insert({source + length, 0});
        }
    }

    long map_val(long val)
    {
        long diff = std::prev(segments.upper_bound(val))->second;
        return val + diff;
    }

    std::vector<Range> map_range(Range range)
    {
        // Iterator before (or equal to) range
        auto curr_itr = std::prev(segments.upper_bound(range.start));

        // Get first range
        std::vector<Range> ranges;
        long start = range.start;
        auto next_itr = std::next(curr_itr);
        long len;
        if (next_itr != segments.end())
        {
            len = std::min(range.length, next_itr->first - start);
        }
        else
        {
            len = range.length;
        }
        Range new_range{start + curr_itr->second, len};
        ranges.push_back(new_range);

        // Loop over remaining segments to accumulate range length
        long accum_len = len;
        auto itr = next_itr;
        while (accum_len < range.length)
        {
            start = itr->first;
            auto next_itr = std::next(itr);
            if (next_itr != segments.end())
            {
                len = std::min(range.length - accum_len, next_itr->first - start);
            }
            else
            {
                len = range.length - accum_len;
            }
            Range new_range{start + itr->second, len};
            ranges.push_back(new_range);

            ++itr;
            accum_len += len;
        }

        return ranges;
    }

    friend std::ostream &operator<<(std::ostream &os, const RangeMap &map)
    {
        for (std::pair<long, long> segment : map.segments)
        {
            os << "(" << segment.first << ", " << segment.second << ")";
        }
        return os;
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

    std::string seed_line;
    std::getline(infile, seed_line);
    std::vector<std::string> seed_strs = split(seed_line.substr(7), " ");
    std::vector<Range> seeds;
    for (int i = 0; i < seed_strs.size(); i += 2)
    {
        // std::cout << "- " << seed_str << "\n";
        seeds.push_back({std::stol(seed_strs.at(i)), std::stol(seed_strs.at(i + 1))});
    }

    std::vector<RangeMap> maps;
    std::string line;
    std::getline(infile, line);
    while (true)
    {
        std::string label_line;
        if (!std::getline(infile, label_line))
        {
            break;
        }
        // std::cout << label_line << "\n";

        RangeMap map;
        std::string data_line;
        std::getline(infile, data_line);
        while (!data_line.empty())
        {
            std::vector<std::string> data = split(data_line, " ");
            map.add_segment(std::stol(data[0]), std::stol(data[1]), std::stol(data[2]));
            std::getline(infile, data_line);
        }
        // std::cout << map << "\n";
        maps.push_back(map);
    }

    // Loop over each seed range looking for the minimum location
    long min_location = LONG_MAX;
    for (Range seed : seeds)
    {
        std::vector<Range> val_ranges;
        val_ranges.push_back(seed);

        // Map to location via all maps
        for (RangeMap map : maps)
        {
            // Loop over every range, getting mapped range(s)
            std::vector<Range> map_ranges;
            for (Range val_range : val_ranges)
            {
                std::vector<Range> ranges = map.map_range(val_range);
                for (Range range : ranges)
                {
                    map_ranges.push_back(range);
                }
            }
            val_ranges = map_ranges;
        }

        // Find minimum location from this seed range
        for (Range val_range : val_ranges)
        {
            min_location = std::min(min_location, val_range.start);
        }
    }
    
    std::cout << min_location << "\n";
}
