#include "AI_Position.h"

AI_Position::AI_Position()
{

}

bool AI_Position::equals(AI_Position other)
{
    if(
        this->p.equals(other.p) &&
        this->map==other.map &&
        this->blueprint==other.blueprint &&
        this->approach_option==other.approach_option &&
        this->step_history==other.step_history
    )return true;
    return false;
}
