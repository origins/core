/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>.sourceforge.net/>
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

/* ScriptData
SDName: Blades_Edge_Mountains
SD%Complete: 90
SDComment: Quest support: 10503, 10504, 10556, 10609, 10682, 10821, 10980. Ogri'la->Skettis Flight. (npc_daranelle needs bit more work before consider complete)
SDCategory: Blade's Edge Mountains
EndScriptData */

/* ContentData
mobs_bladespire_ogre
mobs_nether_drake
npc_daranelle
npc_overseer_nuaar
npc_saikkal_the_elder
go_legion_obelisk
EndContentData */

#include "precompiled.h"

//Support for quest: You're Fired! (10821)
bool     obelisk_one, obelisk_two, obelisk_three, obelisk_four, obelisk_five;

#define LEGION_OBELISK_ONE           185193
#define LEGION_OBELISK_TWO           185195
#define LEGION_OBELISK_THREE         185196
#define LEGION_OBELISK_FOUR          185197
#define LEGION_OBELISK_FIVE          185198

/*######
## mobs_bladespire_ogre
######*/

//TODO: add support for quest 10512 + Creature abilities
struct CW_DLL_DECL mobs_bladespire_ogreAI : public ScriptedAI
{
    mobs_bladespire_ogreAI(Creature *c) : ScriptedAI(c) {}

    void Reset() { }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mobs_bladespire_ogre(Creature* pCreature)
{
    return new mobs_bladespire_ogreAI (pCreature);
}

/*######
## mobs_nether_drake
######*/

enum eNetherdrake
{
    SAY_NIHIL_1                 = -1000396, //signed for 5955
    SAY_NIHIL_2                 = -1000397, //signed for 5955
    SAY_NIHIL_3                 = -1000398, //signed for 5955
    SAY_NIHIL_4                 = -1000399, //signed for 20021, used by 20021,21817,21820,21821,21823
    SAY_NIHIL_INTERRUPT         = -1000400, //signed for 20021, used by 20021,21817,21820,21821,21823

    ENTRY_WHELP                 = 20021,
    ENTRY_PROTO                 = 21821,
    ENTRY_ADOLE                 = 21817,
    ENTRY_MATUR                 = 21820,
    ENTRY_NIHIL                 = 21823,

    SPELL_T_PHASE_MODULATOR     = 37573,

    SPELL_ARCANE_BLAST          = 38881,
    SPELL_MANA_BURN             = 38884,
    SPELL_INTANGIBLE_PRESENCE   = 36513
};

struct CW_DLL_DECL mobs_nether_drakeAI : public ScriptedAI
{
    mobs_nether_drakeAI(Creature *c) : ScriptedAI(c) {}

    bool IsNihil;
    uint32 NihilSpeech_Timer;
    uint32 NihilSpeech_Phase;

    uint32 ArcaneBlast_Timer;
    uint32 ManaBurn_Timer;
    uint32 IntangiblePresence_Timer;

    void Reset()
    {
        IsNihil = false;
        NihilSpeech_Timer = 3000;
        NihilSpeech_Phase = 0;

        ArcaneBlast_Timer = 7500;
        ManaBurn_Timer = 10000;
        IntangiblePresence_Timer = 15000;
    }

    void EnterCombat(Unit* who) { }

    void MoveInLineOfSight(Unit *who)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    //in case Creature was not summoned (not expected)
    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 0)
        {
            m_creature->setDeathState(JUST_DIED);
            m_creature->RemoveCorpse();
            m_creature->SetHealth(0);
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_T_PHASE_MODULATOR && caster->GetTypeId() == TYPEID_PLAYER)
        {
            const uint32 entry_list[4] = {ENTRY_PROTO, ENTRY_ADOLE, ENTRY_MATUR, ENTRY_NIHIL};
            int cid = rand()%(4-1);

            if (entry_list[cid] == m_creature->GetEntry())
                ++cid;

            //we are nihil, so say before transform
            if (m_creature->GetEntry() == ENTRY_NIHIL)
            {
                DoScriptText(SAY_NIHIL_INTERRUPT, m_creature);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                IsNihil = false;
            }

            if (m_creature->UpdateEntry(entry_list[cid]))
            {
                if (entry_list[cid] == ENTRY_NIHIL)
                {
                    EnterEvadeMode();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    IsNihil = true;
                }else
                    AttackStart(caster);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (IsNihil)
        {
            if (NihilSpeech_Timer <= diff)
            {
                switch(NihilSpeech_Phase)
                {
                    case 0:
                        DoScriptText(SAY_NIHIL_1, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 1:
                        DoScriptText(SAY_NIHIL_2, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 2:
                        DoScriptText(SAY_NIHIL_3, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 3:
                        DoScriptText(SAY_NIHIL_4, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 4:
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        //take off to location above
                        m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()+50.0f, m_creature->GetPositionY(), m_creature->GetPositionZ()+50.0f);
                        ++NihilSpeech_Phase;
                        break;
                }
                NihilSpeech_Timer = 5000;
            }else NihilSpeech_Timer -=diff;

            //anything below here is not interesting for Nihil, so skip it
            return;
        }

        if (!UpdateVictim())
            return;

        if (IntangiblePresence_Timer <= diff)
        {
            DoCast(m_creature->getVictim(),SPELL_INTANGIBLE_PRESENCE);
            IntangiblePresence_Timer = 15000+rand()%15000;
        }else IntangiblePresence_Timer -= diff;

        if (ManaBurn_Timer <= diff)
        {
            Unit* target = m_creature->getVictim();
            if (target && target->getPowerType() == POWER_MANA)
                DoCast(target,SPELL_MANA_BURN);
            ManaBurn_Timer = 8000+rand()%8000;
        }else ManaBurn_Timer -= diff;

        if (ArcaneBlast_Timer <= diff)
        {
            DoCast(m_creature->getVictim(),SPELL_ARCANE_BLAST);
            ArcaneBlast_Timer = 2500+rand()%5000;
        }else ArcaneBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mobs_nether_drake(Creature* pCreature)
{
    return new mobs_nether_drakeAI (pCreature);
}

/*######
## npc_daranelle
######*/

enum eDaranelle
{
    SAY_SPELL_INFLUENCE     = -1000174,
    SPELL_LASHHAN_CHANNEL   = 36904
};

struct CW_DLL_DECL npc_daranelleAI : public ScriptedAI
{
    npc_daranelleAI(Creature *c) : ScriptedAI(c) {}

    void Reset() { }

    void EnterCombat(Unit* who) { }

    void MoveInLineOfSight(Unit *who)
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            if (who->HasAura(SPELL_LASHHAN_CHANNEL) && m_creature->IsWithinDistInMap(who, 10.0f))
            {
                DoScriptText(SAY_SPELL_INFLUENCE, m_creature, who);
                //TODO: Move the below to updateAI and run if this statement == true
                m_creature->CastSpell(who,37028,true);
            }
        }

        ScriptedAI::MoveInLineOfSight(who);
    }
};

CreatureAI* GetAI_npc_daranelle(Creature* pCreature)
{
    return new npc_daranelleAI (pCreature);
}

/*######
## npc_overseer_nuaar
######*/

#define GOSSIP_HELLO_ON "Overseer, I am here to negotiate on behalf of the Cenarion Expedition."

bool GossipHello_npc_overseer_nuaar(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(10682) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_ON, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(10532, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_overseer_nuaar(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->SEND_GOSSIP_MENU(10533, pCreature->GetGUID());
        pPlayer->AreaExploredOrEventHappens(10682);
    }
    return true;
}

/*######
## npc_saikkal_the_elder
######*/

#define GOSSIP_HELLO_STE    "Yes... yes, it's me."
#define GOSSIP_SELECT_STE   "Yes elder. Tell me more of the book."

bool GossipHello_npc_saikkal_the_elder(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(10980) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_STE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(10794, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_saikkal_the_elder(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT_STE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(10795, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->TalkedToCreature(pCreature->GetEntry(), pCreature->GetGUID());
            pPlayer->SEND_GOSSIP_MENU(10796, pCreature->GetGUID());
            break;
    }
    return true;
}

/*######
## go_legion_obelisk
######*/

bool GOHello_go_legion_obelisk(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(10821) == QUEST_STATUS_INCOMPLETE)
    {
        switch(pGo->GetEntry())
        {
            case LEGION_OBELISK_ONE:
                  obelisk_one = true;
                 break;
            case LEGION_OBELISK_TWO:
                  obelisk_two = true;
                 break;
            case LEGION_OBELISK_THREE:
                  obelisk_three = true;
                 break;
            case LEGION_OBELISK_FOUR:
                  obelisk_four = true;
                 break;
            case LEGION_OBELISK_FIVE:
                  obelisk_five = true;
                 break;
        }

        if (obelisk_one == true && obelisk_two == true && obelisk_three == true && obelisk_four == true && obelisk_five == true)
        {
            pGo->SummonCreature(19963,2943.40f,4778.20f,284.49f,0.94f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,120000);
            //reset global var
            obelisk_one = false;
            obelisk_two = false;
            obelisk_three = false;
            obelisk_four = false;
            obelisk_five = false;
        }
    }

    return true;
}

/*######
## AddSC
######*/

void AddSC_blades_edge_mountains()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mobs_bladespire_ogre";
    newscript->GetAI = &GetAI_mobs_bladespire_ogre;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mobs_nether_drake";
    newscript->GetAI = &GetAI_mobs_nether_drake;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_daranelle";
    newscript->GetAI = &GetAI_npc_daranelle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_overseer_nuaar";
    newscript->pGossipHello = &GossipHello_npc_overseer_nuaar;
    newscript->pGossipSelect = &GossipSelect_npc_overseer_nuaar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_saikkal_the_elder";
    newscript->pGossipHello = &GossipHello_npc_saikkal_the_elder;
    newscript->pGossipSelect = &GossipSelect_npc_saikkal_the_elder;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_legion_obelisk";
    newscript->pGOHello =           &GOHello_go_legion_obelisk;
    newscript->RegisterSelf();
}

