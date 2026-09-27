#include "PlayerStats.h"

PlayerStats::PlayerStats()
{
    maxHealth = 20.0f;
    health = maxHealth;

    maxHunger = 20.0f;
    hunger = maxHunger;

    maxAdrenaline = 100.0f;
    adrenaline = 0.0f;

    minHeat = 0.0f;
    maxHeat = 100.0f;

    // Start at a normal temperature.
    heat = 37.0f;
}

void PlayerStats::Update(float deltaTime)
{
    // ----------------------------------------
    // Hunger
    // ----------------------------------------

    if (hunger < 0.0f)
        hunger = 0.0f;

    if (hunger > maxHunger)
        hunger = maxHunger;


    // ----------------------------------------
    // Health
    // ----------------------------------------

    if (health < 0.0f)
        health = 0.0f;

    if (health > maxHealth)
        health = maxHealth;


    // ----------------------------------------
    // Adrenaline
    // ----------------------------------------

    float adrenalineIncrease = 0.0f;

    // Extremely low hunger
    if (hunger <= 4.0f)
        adrenalineIncrease += 4.0f;

    // Extremely low health
    if (health <= 5.0f)
        adrenalineIncrease += 6.0f;

    // Increase adrenaline from stressful conditions.
    adrenaline += adrenalineIncrease * deltaTime;

    // Naturally decrease when safe.
    if (hunger > 4.0f && health > 5.0f)
        adrenaline -= 3.0f * deltaTime;

    if (adrenaline < 0.0f)
        adrenaline = 0.0f;

    if (adrenaline > maxAdrenaline)
        adrenaline = maxAdrenaline;


    // ----------------------------------------
    // Heat
    // ----------------------------------------

    if (heat < minHeat)
        heat = minHeat;

    if (heat > maxHeat)
        heat = maxHeat;
}

void PlayerStats::Damage(float amount)
{
    health -= amount;

    if (health < 0.0f)
        health = 0.0f;
}

void PlayerStats::Heal(float amount)
{
    health += amount;

    if (health > maxHealth)
        health = maxHealth;
}

void PlayerStats::Eat(float amount)
{
    hunger += amount;

    if (hunger > maxHunger)
        hunger = maxHunger;
}

void PlayerStats::AddAdrenaline(float amount)
{
    adrenaline += amount;

    if (adrenaline > maxAdrenaline)
        adrenaline = maxAdrenaline;
}

void PlayerStats::ReduceAdrenaline(float amount)
{
    adrenaline -= amount;

    if (adrenaline < 0.0f)
        adrenaline = 0.0f;
}

void PlayerStats::ChangeHeat(float amount)
{
    heat += amount;

    if (heat < minHeat)
        heat = minHeat;

    if (heat > maxHeat)
        heat = maxHeat;
}

bool PlayerStats::IsDead() const
{
    return health <= 0.0f;
}
