
#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:

    explicit GameException(const std::string& msg) 
        : std::runtime_error(msg) {}
};

#endif // GAMEEXCEPTION_H
