#include "HUD.h"

#include <GL/gl.h>

static void DrawBar(
    float x,
    float y,
    float width,
    float height,
    float value,
    float maxValue)
{
    float percentage = value / maxValue;

    if (percentage < 0.0f)
        percentage = 0.0f;

    if (percentage > 1.0f)
        percentage = 1.0f;

    // Background
    glBegin(GL_QUADS);

    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);

    glEnd();

    // Filled portion
    glBegin(GL_QUADS);

    glVertex2f(x, y);
    glVertex2f(x + width * percentage, y);
    glVertex2f(x + width * percentage, y + height);
    glVertex2f(x, y + height);

    glEnd();
}

void DrawPlayerStatsHUD(const PlayerStats& stats)
{
    const float x = 20.0f;
    const float width = 180.0f;
    const float height = 14.0f;
    const float spacing = 25.0f;

    // Health
    DrawBar(
        x,
        20.0f,
        width,
        height,
        stats.health,
        stats.maxHealth
    );

    // Hunger
    DrawBar(
        x,
        20.0f + spacing,
        width,
        height,
        stats.hunger,
        stats.maxHunger
    );

    // Adrenaline
    DrawBar(
        x,
        20.0f + spacing * 2,
        width,
        height,
        stats.adrenaline,
        stats.maxAdrenaline
    );

    // Heat: 0-100°C
    DrawBar(
        x,
        20.0f + spacing * 3,
        width,
        height,
        stats.heat,
        100.0f
    );
}
