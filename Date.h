#include <iostream>

class Date {
public:
    // Constructor with default values
    Date(int day = 1, int month = 1, int year = 1900) : day_(day), month_(month), year_(year) {}

    // Copy constructor
    Date(const Date& other) : day_(other.day_), month_(other.month_), year_(other.year_) {}

    // Destructor
    ~Date() {}

    // Overloaded assignment operator
    Date& operator=(const Date& other) {
        if (this != &other) {
            day_ = other.day_;
            month_ = other.month_;
            year_ = other.year_;
        }
        return *this;
    }

    // Constant methods
    int getDay() const { return day_; }
    int getMonth() const { return month_; }
    int getYear() const { return year_; }

    // Overloaded output operator
    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << date.day_ << "/" << date.month_ << "/" << date.year_;
        return os;
    }

    // Overloaded [] operator
    int operator[](int index) const {
        if (index == 0) {
            return day_;
        } else if (index == 1) {
            return month_;
        } else if (index == 2) {
            return year_;
        } else {
            // Index out of range
            return -1;
        }
    }

private:
    int day_;
    int month_;
    int year_;
};