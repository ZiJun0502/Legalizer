#ifndef PLACEMENT_ROW_H
#define PLACEMENT_ROW_H

#include "globals.h"
#include "cell.h"

using namespace boost::numeric;
using Interval = interval<size_t>;
template <typename T>
std::ostream& operator<<(std::ostream& os, const boost::numeric::interval<T>& i) {
    os << "[" << lower(i) << ", " << upper(i) << "]";
    return os;
}
struct IntervalComparator {
    bool operator()(const Interval& lhs, const Interval& rhs) const {
        // Compare intervals by their starting points
        return lhs.upper() < rhs.upper();
    }
};
class PlacementRow {
public:
    double start_x, start_y; // Positions
    double site_width, site_height; // Dimensions
    size_t total_num_of_sites;
    std::set<Interval, IntervalComparator> free_intervals;  

    // Constructor
    PlacementRow(double sx, double sy, double sw, double sh, int sites)
        : start_x(sx), start_y(sy), site_width(sw), site_height(sh), total_num_of_sites(sites) {
        // this->free_intervals.insert(Interval(0, std::numeric_limits<size_t>::max()));
        this->free_intervals.insert(Interval(0, die_upper_right_x));
        // cout << 
    }

    // Copy constructor
    PlacementRow(const PlacementRow& other)
        : start_x(other.start_x), start_y(other.start_y), site_width(other.site_width),
          site_height(other.site_height), total_num_of_sites(other.total_num_of_sites),
          free_intervals(other.free_intervals) {}

    std::vector<Interval> filterIntervalsByWidth(size_t min_width) {
        std::vector<Interval> result;
    
        for (const auto& interval : free_intervals) {
            size_t width = interval.upper() - interval.lower(); // Calculate the width of the interval
            if (width > min_width) {
                result.push_back(interval);
            }
        }
        return result;
    }

    bool isOccupied(std::pair<size_t, size_t> cols) {
        auto interval = Interval(cols.first, cols.second);
        auto it_ref = free_intervals.lower_bound(interval);
        // if the free interval's left bound does not contain the block
        // it means the position (block_left_bound, free_interval_left_bound) is occupied
        return it_ref->lower() > interval.lower();
    }
    void insert(Interval new_interval) {
        // Find the interval to remove using lower_bound
        auto it_ref = free_intervals.lower_bound(new_interval);
        if (!(it_ref != free_intervals.end() && it_ref->lower() < new_interval.upper())) {
            std::cout << *this << '\n' << new_interval << '\n';
        }
        assert(it_ref != free_intervals.end() && it_ref->lower() < new_interval.upper() && "Interval to remove does not exist or is invalid");
        free_intervals.erase(it_ref);
        auto it = *it_ref;

        Interval split_space;
        if (it.lower() < new_interval.lower()) {
            split_space = Interval(it.lower(), new_interval.lower());
            free_intervals.insert(split_space);
        }
        if (it.upper() > new_interval.upper()) {
            split_space = Interval(new_interval.upper(), it.upper());
            free_intervals.insert(split_space);
        }
    }
    void remove(Interval remove_interval) {
        // Find the interval to remove using lower_bound
        free_intervals.insert(remove_interval);
        auto it = free_intervals.insert(remove_interval).first;
        if (it != free_intervals.begin()) {
            auto prev_it = std::prev(it);
            // std::cout << *prev_it << ' ' << *it << '\n';
            assert(prev_it->upper() <= it->lower() && "Free interval and remove interval should not overlap\n");
            if (prev_it->upper() == it->lower()) {  
                Interval merged = hull(*prev_it, *it);
                free_intervals.erase(prev_it);  
                free_intervals.erase(it);      
                free_intervals.insert(merged); 
                it = free_intervals.find(merged);  
            }
        }

        // Check the next neighbor
        auto next_it = std::next(it);
        if (next_it != free_intervals.end()) {
            // std::cout << *it << ' ' << *next_it << '\n';;
            assert(it->upper() <= next_it->lower() && "Free interval and remove interval should not overlap\n");
            if (it->upper() == next_it->lower()) {
                Interval merged = hull(*it, *next_it);
                free_intervals.erase(it);         
                free_intervals.erase(next_it);    
                free_intervals.insert(merged);   
            }
        }
    }

    // Overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const PlacementRow& row) {
        os << "Start Position: (" << row.start_x << ", " << row.start_y << ")\n"
           << "Site Width: " << row.site_width << '\n'
           << "Site Height: " << row.site_height << '\n'
           << "Total Number of Sites: " << row.total_num_of_sites << '\n';
        for (auto interval : row.free_intervals) {
            os << interval << ' ';
        }
        return os;
    }
};

#endif
