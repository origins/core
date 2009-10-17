/* Script Data Start
SDName: Boss zuramat
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
#define SPELL_SHROUD_OF_DARKNESS                       54524
#define SPELL_SUMMON_VOID_SENTRY                       54369
#define SPELL_VOID_SHIFT                               54361

#define NPC_VOID_SENTRY                                29364

//not in db
//Yells
#define SAY_AGGRO                                   -1608037
#define SAY_SLAY_1                                  -1608038
#define SAY_SLAY_2                                  -1608039
#define SAY_SLAY_3                                  -1608040
#define SAY_DEATH                                   -1608041
#define SAY_SPAWN                                   -1608042
#define SAY_SHIELD                                  -1608043
#define SAY_WHISPER                                 -1608044

struct CW_DLL_DECL boss_zuramatAI : public ScriptedAI
{
    boss_zuramatAI(Creature *c) : ScriptedAI(c) {}

    uint32 void_shift;

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

CreatureAI* GetAI_boss_zuramat(Creature* pCreature)
{
    return new boss_zuramatAI (pCreature);
}

void AddSC_boss_zuramat()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_zuramat";
    newscript->GetAI = &GetAI_boss_zuramat;
    newscript->RegisterSelf();
}
