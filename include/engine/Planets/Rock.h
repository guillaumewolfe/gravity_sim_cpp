// Rock.h
#ifndef Rock_H
#define Rock_H

#include "engine/CelestialObject.h"  // Inclure la définition de CelestialObject

class Rock : public CelestialObject {
public:
    Rock();  // Déclarer le constructeur spécifique à la Terre
};

#endif