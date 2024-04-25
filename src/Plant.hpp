class Plant {
public:
    float posX;
    float posY;
    float scale;
    float angle;
    bool  oscillationDirection = true;

    // Constructeur avec initialisation par défaut pour oscillationDirection
    Plant(float posX_, float posY_, float scale_, float angle_, bool oscillationDirection_ = true)
        : posX(posX_), posY(posY_), scale(scale_), angle(angle_), oscillationDirection(oscillationDirection_) {}

    float getPlantAngleX() const
    {
        return this->angle;
    }

    float getPlantAngleY() const
    {
        return this->angle;
    }

    void updatePlantAngle(float speedFactor)
    {
        // Calculer l'oscillation de la queue
        float oscillationStep = 0.001f * speedFactor; // Pas d'oscillation
        if (oscillationDirection)
        {
            angle += oscillationStep; // Augmenter l'angle
            if (angle >= 0.1f)
            {
                oscillationDirection = false; // Changer de direction
            }
        }
        else
        {
            angle -= oscillationStep; // Diminuer l'angle
            if (angle <= -0.1f)
            {
                oscillationDirection = true; // Changer de direction
            }
        }
    }
};