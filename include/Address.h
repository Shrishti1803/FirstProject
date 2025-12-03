#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
using namespace std;

class Address {
private:
    int id;
    string line;
    string city;
    string state;
    string postalCode;
    string country;
    int isDefaultFlag;

public:
    Address() {}

    Address(int _id, string _line, string _city, string _state,
            string _postalCode, string _country, int _isDefault)
        : id(_id), line(_line), city(_city), state(_state),
          postalCode(_postalCode), country(_country),
          isDefaultFlag(_isDefault) {}

    int getId() const { return id; }
    string getLine() const { return line; }
    string getCity() const { return city; }
    string getState() const { return state; }
    string getPostalCode() const { return postalCode; }
    string getCountry() const { return country; }

    // NEW CLEAN NAME
    bool isDefault() const { return isDefaultFlag == 1; }
};

#endif
