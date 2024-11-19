#ifndef PLACEMENT_ROW_H
#define PLACEMENT_ROW_H

#include "globals.h"
#include "cell.h"

using namespace boost::numeric;
using Interval = interval<size_t>;
template <typename T>
std::ostream& operator<<(std::ostream& os, const boost::numeric::interval<T>& i) {
    os << "[" << lower(i) << ", " << upper(i) << "]\n";
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


    void insert(Cell& cell) {
        size_t site_start = static_cast<size_t>(std::floor(cell.x / site_width));
        size_t site_end = static_cast<size_t>(std::ceil((cell.x + cell.width) / site_width));

        Interval new_interval(site_start, site_end);  // Correct way to create the interval
        // Find the interval to remove using lower_bound
        auto it_ref = free_intervals.lower_bound(new_interval);
        assert(it_ref != free_intervals.end() && it_ref->lower() < site_end && "Interval to remove does not exist or is invalid");
        free_intervals.erase(it_ref);
        auto it = *it_ref;

        Interval split_space;
        if (it.lower() < site_start) {
            split_space = Interval(it.lower(), site_start);
            free_intervals.insert(split_space);
        }
        if (it.upper() > site_end) {
            split_space = Interval(site_end, it.upper());
            free_intervals.insert(split_space);
        }
    }

    // Constructor
    PlacementRow(double sx, double sy, double sw, double sh, int sites)
        : start_x(sx), start_y(sy), site_width(sw), site_height(sh), total_num_of_sites(sites) {
        this->free_intervals.insert(Interval(0, std::numeric_limits<size_t>::max()));
        // cout << 
    }

    // Copy constructor
    PlacementRow(const PlacementRow& other)
        : start_x(other.start_x), start_y(other.start_y), site_width(other.site_width),
          site_height(other.site_height), total_num_of_sites(other.total_num_of_sites),
          free_intervals(other.free_intervals) {}

    // Overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const PlacementRow& row) {
        os << "Start Position: (" << row.start_x << ", " << row.start_y << ")"
           << ", Site Width: " << row.site_width
           << ", Site Height: " << row.site_height
           << ", Total Number of Sites: " << row.total_num_of_sites;
        return os;
    }
};

#endif
