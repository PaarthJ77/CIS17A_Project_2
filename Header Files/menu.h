#ifndef MENU_H
#define MENU_H

class Menu {
public:
    // Constructors
    Menu();
    Menu(const Menu& other);

    // Destructor
    ~Menu();

    // Assignment Operator
    Menu& operator=(const Menu& other);

    // Overloaded Operators
    bool operator==(const Menu& other) const;

    // Member functions
    int displayMainMenu() const;
};

#endif // MENU_H
