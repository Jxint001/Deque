#include <iostream>

class Integer {
private:
    int data;
public:
    Integer(const int &value) : data(value) {}
    Integer(const Integer &other) : data(other.data) {}
    bool operator==(const Integer &t)
    {
        //std::cout << data << " " << t.data << std::endl;
        return data == t.data;
    }
    friend std::ostream& operator<<(std::ostream &os, const Integer &obj) {
        os << obj.data;
        return os;
    }
};
