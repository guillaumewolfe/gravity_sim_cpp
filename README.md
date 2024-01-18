# gravity_sim_cpp

Color used : 

Green : 
    Padding : (146, 230,211 , 255)
    Click : (32, 247, 173, 255)
    "On" : (76,230,107,50)
    "Off": (255,0,0,50)



Échelle Celestials Objects : 


distanceTerreSoleil = 149_597_870e3
facteur = 600
SCALE = distanceTerreSoleil/facteur


class SimulationScale:
    DISTANCE_SCALE = SCALE
    SIZE_MIN = 1  # Rayon minimum dans la simulation
    SIZE_MAX = 15  # Rayon maximum (pour le Soleil)

    # ... Vos autres méthodes ici ...

    @classmethod
    def to_distance(cls, real_distance):
        return real_distance / cls.DISTANCE_SCALE

    @classmethod
    def to_size(cls, real_radius):
        if real_radius == 696_340e3:
            return 20
        if real_radius == 1737.5e3:
            return 0.01
       
        # Mise à l'échelle linéaire
        max_radius_real = 69911e3  # Rayon de Jupiter
        min_radius_real = 2439.7e3  # Supposons qu'il s'agisse d'un rayon minimum réaliste pour un petit objet