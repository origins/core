/*
 * Copyright (C) 2009 CWCore <http://www.wow-extrem.de/>
 *
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

/* Script Data Start
SDName: Boss hadronox
SDAuthor: WarHead
SD%Complete: 50
SDComment: No Waves atm and the doors spells are crazy...
SDCategory:
Script Data End */

//Phase 0 => Gauntlet Event described below
//Phase 1 => Boss Encounter

/*When your group enters the main room (the one after the bridge), you will notice a group of 3 Nerubians.
When you engage them, 2 more groups like this one spawn behind the first one - it is important to pull the first group back,
so you don't aggro all 3. Hadronox will be under you, fighting Nerubians.

This is the timed gauntlet - waves of non-elite spiders
 will spawn from the 3 doors located a little above the main room, and will then head down to fight Hadronox. After clearing the
 main room, it is recommended to just stay in it, kill the occasional non-elites that will attack you instead of the boss, and wait for
 Hadronox to make his way to you. When Hadronox enters the main room, she will web the doors, and no more non-elites will spawn.*/

struct CW_DLL_DECL boss_hadronoxAI : public ScriptedAI
{
    boss_hadronoxAI(Creature *c) : ScriptedAI(c) {}

    uint32 phase;

    void Reset() {}
    void EnterCombat(Unit* who) {}
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        phase =1;

        DoMeleeAttackIfReady();
    }
    void JustDied(Unit* killer)  {}

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

    //when Hadronox kills any enemy (that includes a party member) she will regain 10% of her HP if the target had Leech Poison on
    }
};

CreatureAI* GetAI_boss_hadronox(Creature* pCreature)
{
    return new boss_hadronoxAI (pCreature);
}

void AddSC_boss_hadronox()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_hadronox";
    newscript->GetAI = &GetAI_boss_hadronox;
    newscript->RegisterSelf();
}
