#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <string>

struct NpcData
{
    char Name[255];
    char NpcIni[255];
    char FlyIni[255];
    char BodyIni[255];
    char Kind;
    char Relation;
    long Life;
    long LifeMax;
    long Thew;
    long ThewMax;
    long Mana;
    long ManaMax;
    long Attack;
    long Defence;
    long Evade;
    long Exp;
    char WalkSpeed;
    char Dir;
    long Lum;
    char PathFinder;
    char DeathScript[255];
    char ScriptFile[255];
};

void ResetNpc(NpcData *npc);
bool ReadNpcIni(std::string FilePath, NpcData *npc);
std::string FindStandAsf(std::string FilePath);

#endif // NPC_HPP_INCLUDED
