/* Script Data Start
SDName: Boss xevozz
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
#define SPELL_ARCANE_BARRAGE_VOLLEY                    54202
#define SPELL_ARCANE_BUFFET                            54226
#define SPELL_SUMMON_ETHEREAL_SPHERE_1                 54102
#define SPELL_SUMMON_ETHEREAL_SPHERE_2                 54137
#define SPELL_SUMMON_ETHEREAL_SPHERE_3                 54138

//not in db
//Yells
#define SAY_AGGRO                                   -1608027
#define SAY_SLAY_1                                  -1608028
#define SAY_SLAY_2                                  -1608029
#define SAY_SLAY_3                                  -1608030
#define SAY_DEATH                                   -1608031
#define SAY_SPAWN                                   -1608032
#define SAY_CHARGED                                 -1608033
#define SAY_REPEAT_SUMMON_1                         -1608034
#define SAY_REPEAT_SUMMON_2                         -1608035
#define SAY_SUMMON_ENERGY                           -1608036

struct CW_DLL_DECL boss_xevozzAI : public ScriptedAI
{
    boss_xevozzAI(Creature *c) : ScriptedAI(c) {}

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
                pInstance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
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

CreatureAI* GetAI_boss_xevozz(Creature* pCreature)
{
    return new boss_xevozzAI (pCreature);
}

void AddSC_boss_xevozz()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_xevozz";
    newscript->GetAI = &GetAI_boss_xevozz;
    newscript->RegisterSelf();
}
