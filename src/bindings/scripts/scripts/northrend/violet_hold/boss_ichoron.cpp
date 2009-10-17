/* Script Data Start
SDName: Boss ichoron
SDAuthor: LordVanMartin
SD%Complete:
SDComment:
SDCategory:
Script Data End */

/*** SQL START ***
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_DRAINED                                  59820
#define SPELL_FRENZY                                   54312
#define SPELL_PROTECTIVE_BUBBLE                        54306
#define SPELL_WATER_BLAST                              54237
#define SPELL_WATER_BOLT_VOLLEY                        54241

//not in db
//Yells
#define SAY_AGGRO                                   -1608018
#define SAY_SLAY_1                                  -1608019
#define SAY_SLAY_2                                  -1608020
#define SAY_SLAY_3                                  -1608021
#define SAY_DEATH                                   -1608022
#define SAY_SPAWN                                   -1608023
#define SAY_ENRAGE                                  -1608024
#define SAY_SHATTER                                 -1608025
#define SAY_BUBBLE                                  -1608026

struct CW_DLL_DECL boss_ichoronAI : public ScriptedAI
{
    boss_ichoronAI(Creature *c) : ScriptedAI(c) {}

    void Reset() {}
    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
        {
            if (pInstance->GetData(DATA_WAVE_COUNT) == 6)
            {
                pInstance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                pInstance->SetData(DATA_WAVE_COUNT, 7);
            }
            else if (pInstance->GetData(DATA_WAVE_COUNT) == 12)
            {
                pInstance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                pInstance->SetData(DATA_WAVE_COUNT, 13);
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;
        DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3), m_creature);
    }
};

CreatureAI* GetAI_boss_ichoron(Creature* pCreature)
{
    return new boss_ichoronAI (pCreature);
}

void AddSC_boss_ichoron()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_ichoron";
    newscript->GetAI = &GetAI_boss_ichoron;
    newscript->RegisterSelf();
}
