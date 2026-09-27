#include "Item.h"
#include "PlayerStats.h"

#include <fstream>
#include <sstream>
#include <string>

Item* itemRegistry[256] = {};


// ============================================================
// Item
// ============================================================

Item::Item()
{
    id = 0;
    name = "";
    maxStack = 64;
    textureName = "";
}

Item::~Item()
{
}

void Item::OnUse()
{
}

void Item::OnBreak()
{
}

void Item::OnPlace()
{
}


// ============================================================
// BlockItem
// ============================================================

BlockItem::BlockItem()
{
    maxStack = 64;
}

void BlockItem::OnUse()
{
}

void BlockItem::OnBreak()
{
}

void BlockItem::OnPlace()
{
}


// ============================================================
// WeaponItem
// ============================================================

WeaponItem::WeaponItem()
{
    maxStack = 1;
    damage = 1;
}

void WeaponItem::OnUse()
{
}

void WeaponItem::OnBreak()
{
}

void WeaponItem::OnPlace()
{
}

float WeaponItem::CalculateDamage(
    PlayerStats& target,
    float strengthMultiplier)
{
    float finalDamage = damage * strengthMultiplier;

    // Apply the calculated damage to the target.
    target.Damage(finalDamage);

    // Return the amount of damage dealt.
    return finalDamage;
}


// ============================================================
// ToolItem
// ============================================================

ToolItem::ToolItem()
{
    maxStack = 1;
    durability = 0;
}

void ToolItem::OnUse()
{
}

void ToolItem::OnBreak()
{
}

void ToolItem::OnPlace()
{
}


// ============================================================
// FoodItem
// ============================================================

FoodItem::FoodItem()
{
    maxStack = 64;
    hunger = 0;
}

void FoodItem::OnUse()
{
}

void FoodItem::OnBreak()
{
}

void FoodItem::OnPlace()
{
}


// ============================================================
// Item Reader
// ============================================================
//
// Format:
//
// ID MaxStack Name Texture Class [Damage]
//
// Classes:
//
// i = Item
// b = Block
// w = Weapon
// t = Tool
// f = Food
//
// Examples:
//
// 1 64 Stone st b
// 2 64 Dirt di b
// 10 1 IronSword is w 6
// 11 1 DiamondSword ds w 8
//
// Textures:
//
// textures/items/<Texture>.png
//
// ============================================================

bool LoadItems(const char* filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        return false;

    std::string line;

    while (std::getline(file, line))
    {
        // Ignore empty lines
        if (line.empty())
            continue;

        // Ignore comments
        if (line[0] == '#')
            continue;

        std::stringstream stream(line);

        int id;
        int maxStack;

        std::string name;
        std::string textureName;

        char classType;

        // Read basic item information
        if (!(stream >> id
                    >> maxStack
                    >> name
                    >> textureName
                    >> classType))
        {
            continue;
        }

        // Item IDs must fit in the registry.
        if (id < 0 || id >= 256)
            continue;

        // Texture name must be exactly two characters.
        if (textureName.length() != 2)
            continue;

        Item* item = nullptr;

        // Create the appropriate item class.
        switch (classType)
        {
            case 'i':
                item = new Item();
                break;

            case 'b':
                item = new BlockItem();
                break;

            case 'w':
                item = new WeaponItem();
                break;

            case 't':
                item = new ToolItem();
                break;

            case 'f':
                item = new FoodItem();
                break;

            default:
                continue;
        }

        // Basic properties
        item->id = id;
        item->name = name;
        item->maxStack = maxStack;
        item->textureName = textureName;

        // Weapon-specific properties
        if (classType == 'w')
        {
            int damage;

            if (!(stream >> damage))
            {
                delete item;
                continue;
            }

            WeaponItem* weapon =
                static_cast<WeaponItem*>(item);

            weapon->damage = damage;
        }

        // Delete an old definition if this ID
        // was already registered.
        if (itemRegistry[id] != nullptr)
            delete itemRegistry[id];

        itemRegistry[id] = item;
    }

    file.close();

    return true;
}
