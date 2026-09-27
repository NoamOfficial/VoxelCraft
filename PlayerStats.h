#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

class PlayerStats
{
public:
    float health;
    float maxHealth;

    float hunger;
    float maxHunger;

    float adrenaline;
    float maxAdrenaline;

    float heat;
    float minHeat;
    float maxHeat;

    float highAdrenalineTime;

    bool gameOver;

    PlayerStats();

    void Update(float deltaTime);

    void Damage(float amount);
    void Heal(float amount);

    void Eat(float amount);

    void AddAdrenaline(float amount);
    void ReduceAdrenaline(float amount);

    void ChangeHeat(float amount);

    float GetStrengthMultiplier() const;

    bool IsDead() const;
    bool IsGameOver() const;
};

#endif
