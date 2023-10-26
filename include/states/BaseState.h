#include <iostream>
#ifndef BASESTATE_H
#define BASESTATE_H

class BaseState {
public:
    virtual ~BaseState() {}
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;
    virtual std::string getDescription() = 0;
};
#endif