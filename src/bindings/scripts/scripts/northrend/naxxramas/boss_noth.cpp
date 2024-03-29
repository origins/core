/* Copyright (C) 2009 CWCore <http://www.wow-extrem.de/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "precompiled.h"
#include "naxxramas.h"

#define SAY_AGGRO               RAND(-1533075,-1533076,-1533077)
#define SAY_SUMMON              -1533078
#define SAY_SLAY                RAND(-1533079,-1533080)
#define SAY_DEATH               -1533081

#define SOUND_DEATH      8848

#define SPELL_CURSE_PLAGUEBRINGER       HEROIC(29213,54835)
#define SPELL_BLINK                     RAND(29208,29209,29210,29211)
#define SPELL_CRIPPLE                   HEROIC(29212,54814)
#define SPELL_TELEPORT                  29216

#define MOB_WARRIOR         16984
#define MOB_CHAMPION        16983
#define MOB_GUARDIAN        16981

// Teleport position of Noth on his balcony
#define TELE_X 2631.370
#define TELE_Y -3529.680
#define TELE_Z 274.040
#define TELE_O 6.277

#define MAX_SUMMON_POS 5

const float SummonPos[MAX_SUMMON_POS][4] =
{
    {2728.12, -3544.43, 261.91, 6.04},
    {2729.05, -3544.47, 261.91, 5.58},
    {2728.24, -3465.08, 264.20, 3.56},
    {2704.11, -3456.81, 265.53, 4.51},
    {2663.56, -3464.43, 262.66, 5.20},
};

enum Events
{
    EVENT_BERSERK   = 1,
    EVENT_CURSE,
    EVENT_BLINK,
    EVENT_WARRIOR,
    EVENT_BALCONY,
    EVENT_WAVE,
    EVENT_GROUND,
};

struct CW_DLL_DECL boss_nothAI : public BossAI
{
    boss_nothAI(Creature *c) : BossAI(c, BOSS_NOTH) {}

    uint32 waveCount, balconyCount;

    void Reset()
    {
        me->SetReactState(REACT_AGGRESSIVE);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        _Reset();
    }

    void EnterCombat(Unit *who)
    {
        _EnterCombat();
        DoScriptText(SAY_AGGRO, me);
        balconyCount = 0;
        EnterPhaseGround();
    }

    void EnterPhaseGround()
    {
        me->SetReactState(REACT_AGGRESSIVE);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoZoneInCombat();
        if (me->getThreatManager().isThreatListEmpty())
            EnterEvadeMode();
        else
        {
            events.ScheduleEvent(EVENT_BALCONY, 110000);
            events.ScheduleEvent(EVENT_CURSE, 20000+rand()%10000);
            events.ScheduleEvent(EVENT_WARRIOR, 30000);
            if (HeroicMode)
                events.ScheduleEvent(EVENT_BLINK, 20000+rand()%10000);
        }
    }

    void KilledUnit(Unit* victim)
    {
        if (!(rand()%5))
            DoScriptText(SAY_SLAY, me);
    }

    void JustSummoned(Creature *summon)
    {
        summons.Summon(summon);
        summon->setActive(true);
        summon->AI()->DoZoneInCombat();
    }

    void JustDied(Unit* Killer)
    {
        _JustDied();
        DoScriptText(SAY_DEATH, me);
    }

    void SummonUndead(uint32 entry, uint32 num)
    {
        for (uint32 i = 0; i < num; ++i)
        {
            uint32 pos = rand()%MAX_SUMMON_POS;
            me->SummonCreature(entry, SummonPos[pos][0], SummonPos[pos][1], SummonPos[pos][2],
                SummonPos[pos][3], TEMPSUMMON_CORPSE_DESPAWN, 60000);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateCombatState() || !CheckInRoom())
            return;

        events.Update(diff);

        while(uint32 eventId = events.ExecuteEvent())
        {
            switch(eventId)
            {
                case EVENT_CURSE:
                    DoCastAOE(SPELL_CURSE_PLAGUEBRINGER);
                    events.ScheduleEvent(EVENT_CURSE, 20000+rand()%10000);
                    return;
                case EVENT_WARRIOR:
                    DoScriptText(SAY_SUMMON, me);
                    SummonUndead(MOB_WARRIOR, HEROIC(2,3));
                    events.ScheduleEvent(EVENT_WARRIOR, 30000);
                    return;
                case EVENT_BLINK:
                    DoCastAOE(SPELL_CRIPPLE, true);
                    DoCastAOE(SPELL_BLINK);
                    DoResetThreat();
                    events.ScheduleEvent(EVENT_BLINK, 20000+rand()%10000);
                    return;
                case EVENT_BALCONY:
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->AttackStop();
                    me->RemoveAllAuras();
                    me->NearTeleportTo(TELE_X, TELE_Y, TELE_Z, TELE_O);
                    events.Reset();
                    events.ScheduleEvent(EVENT_WAVE, 2000);
                    waveCount = 0;
                    return;
                case EVENT_WAVE:
                    DoScriptText(SAY_SUMMON, me);
                    switch(balconyCount)
                    {
                        case 0: SummonUndead(MOB_CHAMPION, HEROIC(2,4)); break;
                        case 1: SummonUndead(MOB_CHAMPION, HEROIC(1,2));
                                SummonUndead(MOB_GUARDIAN, HEROIC(1,2)); break;
                        case 2: SummonUndead(MOB_GUARDIAN, HEROIC(2,4)); break;
                        default:SummonUndead(MOB_CHAMPION, HEROIC(5,10));
                                SummonUndead(MOB_GUARDIAN, HEROIC(5,10));break;
                    }
                    ++waveCount;
                    events.ScheduleEvent(waveCount < 2 ? EVENT_WAVE : EVENT_GROUND, 34000);
                    return;
                case EVENT_GROUND:
                {
                    ++balconyCount;
                    float x, y, z, o;
                    me->GetHomePosition(x, y, z, o);
                    me->NearTeleportTo(x, y, z, o);
                    EnterPhaseGround();
                    return;
                }
            }
        }

        if (me->HasReactState(REACT_AGGRESSIVE))
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI (pCreature);
}

void AddSC_boss_noth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_noth";
    newscript->GetAI = &GetAI_boss_noth;
    newscript->RegisterSelf();
}
