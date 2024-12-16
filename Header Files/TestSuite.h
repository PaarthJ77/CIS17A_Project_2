#ifndef TESTSUITE_H
#define TESTSUITE_H

#include <string>
#include "common.h"

class TestSuite {
public:
    // Constructors
    TestSuite();
    TestSuite(const TestSuite& other); 

    // Destructor
    ~TestSuite();

    // Assignment Operator
    TestSuite& operator=(const TestSuite& other);

    // Overloaded Operators
    bool operator==(const TestSuite& other) const;

    // Member functions
    void runAllTests();
private:
    void testShip();
    void testBoard();
    void testPlayer();
    void testHighScore();
};

#endif // TESTSUITE_H
