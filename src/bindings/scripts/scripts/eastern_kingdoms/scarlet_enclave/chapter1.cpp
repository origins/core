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

#include "precompiled.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "escort_ai.h"

/*######
##Quest 12848
######*/

#define GCD_CAST    1

enum eDeathKnightSpells
{
    SPELL_SOUL_PRISON_CHAIN_SELF    = 54612,
    SPELL_SOUL_PRISON_CHAIN         = 54613,
    SPELL_DK_INITIATE_VISUAL        = 51519,

    SPELL_ICY_TOUCH                 = 52372,
    SPELL_PLAGUE_STRIKE             = 52373,
    SPELL_BLOOD_STRIKE              = 52374,
    SPELL_DEATH_COIL                = 52375
};

#define EVENT_ICY_TOUCH                 1
#define EVENT_PLAGUE_STRIKE             2
#define EVENT_BLOOD_STRIKE              3
#define EVENT_DEATH_COIL                4

//used by 29519,29520,29565,29566,29567 but signed for 29519
int32 say_event_start[8] =
{
    -1609000,-1609001,-1609002,-1609003,
    -1609004,-1609005,-1609006,-1609007
};

int32 say_event_attack[9] =
{
    -1609008,-1609009,-1609010,-1609011,-1609012,
    -1609013,-1609014,-1609015,-1609016
};

uint32 acherus_soul_prison[12] =
{
    191577,
    191580,
    191581,
    191582,
    191583,
    191584,
    191585,
    191586,
    191587,
    191588,
    191589,
    191590
};

uint32 acherus_unworthy_initiate[5] =
{
    29519,
    29520,
    29565,
    29566,
    29567
};

enum UnworthyInitiatePhase
{
    PHASE_CHAINED,
    PHASE_TO_EQUIP,
    PHASE_EQUIPING,
    PHASE_TO_ATTACK,
    PHASE_ATTACKING,
};

struct CW_DLL_DECL npc_unworthy_initiateAI : public ScriptedAI
{
    npc_unworthy_initiateAI(Creature *c) : ScriptedAI(c)
    {
        me->SetReactState(REACT_PASSIVE);
        if (!me->GetEquipmentId())
            if (const CreatureInfo *info = GetCreatureInfo(28406))
                if (info->equipmentId)
                    const_cast<CreatureInfo*>(me->GetCreatureInfo())->equipmentId = info->equipmentId;
    }

    uint64 playerGUID;
    UnworthyInitiatePhase phase;
    uint32 wait_timer;
    float anchorX, anchorY;
    uint64 anchorGUID;

    EventMap events;

    void Reset()
    {
        anchorGUID = 0;
        phase = PHASE_CHAINED;
        events.Reset();
        me->setFaction(7);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        me->SetUInt32Value(UNIT_FIELD_BYTES_1, 8);
        me->LoadEquipment(0, true);
    }

    void EnterCombat(Unit *who)
    {
        events.ScheduleEvent(EVENT_ICY_TOUCH, 1000, GCD_CAST);
        events.ScheduleEvent(EVENT_PLAGUE_STRIKE, 3000, GCD_CAST);
        events.ScheduleEvent(EVENT_BLOOD_STRIKE, 2000, GCD_CAST);
        events.ScheduleEvent(EVENT_DEATH_COIL, 5000, GCD_CAST);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 1)
        {
            wait_timer = 5000;
            me->CastSpell(me, SPELL_DK_INITIATE_VISUAL, true);

            if(Player* starter = Unit::GetPlayer(playerGUID))
                DoScriptText(say_event_attack[rand()%9], me, starter);

            phase = PHASE_TO_ATTACK;
        }
    }

    void EventStart(Creature* anchor, Player* target)
    {
        wait_timer = 5000;
        phase = PHASE_TO_EQUIP;

        me->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        me->RemoveAurasDueToSpell(SPELL_SOUL_PRISON_CHAIN_SELF);
        me->RemoveAurasDueToSpell(SPELL_SOUL_PRISON_CHAIN);

        float z;
        anchor->GetContactPoint(me, anchorX, anchorY, z, 1.0f);

        playerGUID = target->GetGUID();
        DoScriptText(say_event_start[rand()%8], me, target);
    }

    void UpdateAI(const uint32 diff);
};

CreatureAI* GetAI_npc_unworthy_initiate(Creature* pCreature)
{
    return new npc_unworthy_initiateAI(pCreature);
}

struct CW_DLL_DECL npc_unworthy_initiate_anchorAI : public PassiveAI
{
    npc_unworthy_initiate_anchorAI(Creature *c) : PassiveAI(c), prisonerGUID(0) {}

    uint64 prisonerGUID;

    void SetGUID(const uint64 &guid, int32 id)
    {
        if (!prisonerGUID)
            prisonerGUID = guid;
    }

    uint64 GetGUID(int32 id) { return prisonerGUID; }
};

void npc_unworthy_initiateAI::UpdateAI(const uint32 diff)
{
    switch(phase)
    {
    case PHASE_CHAINED:
        if (!anchorGUID)
        {
            if(Creature *anchor = me->FindNearestCreature(29521, 30))
            {
                anchor->AI()->SetGUID(me->GetGUID());
                anchor->CastSpell(me, SPELL_SOUL_PRISON_CHAIN, true);
                anchorGUID = anchor->GetGUID();
            }
            else
                error_log("npc_unworthy_initiateAI: unable to find anchor!");

            float dist = 99.0f;
            GameObject *prison = NULL;

            for (uint8 i = 0; i < 12; ++i)
            {
                if (GameObject* temp_prison = me->FindNearestGameObject(acherus_soul_prison[i],30))
                {
                    if (me->IsWithinDist(temp_prison, dist, false))
                    {
                        dist = me->GetDistance2d(temp_prison);
                        prison = temp_prison;
                    }
                }
            }

            if (prison)
                prison->ResetDoorOrButton();
            else
                error_log("npc_unworthy_initiateAI: unable to find prison!");
        }
        return;
    case PHASE_TO_EQUIP:
        if (wait_timer)
        {
            if (wait_timer > diff)
                wait_timer -= diff;
            else
            {
                me->GetMotionMaster()->MovePoint(1, anchorX, anchorY, me->GetPositionZ());
                //debug_log("npc_unworthy_initiateAI: move to %f %f %f", anchorX, anchorY, me->GetPositionZ());
                phase = PHASE_EQUIPING;
                wait_timer = 0;
            }
        }
        return;
    case PHASE_TO_ATTACK:
        if (wait_timer)
        {
            if (wait_timer > diff)
                wait_timer -= diff;
            else
            {
                me->setFaction(14);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                phase = PHASE_ATTACKING;

                if (Player* target = Unit::GetPlayer(playerGUID))
                    me->AI()->AttackStart(target);
                wait_timer = 0;
            }
        }
        return;
    case PHASE_ATTACKING:
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while(uint32 eventId = events.ExecuteEvent())
        {
            switch(eventId)
            {
            case EVENT_ICY_TOUCH:
                DoCast(me->getVictim(), SPELL_ICY_TOUCH);
                events.DelayEvents(1000, GCD_CAST);
                events.ScheduleEvent(EVENT_ICY_TOUCH, 5000, GCD_CAST);
                break;
            case EVENT_PLAGUE_STRIKE:
                DoCast(me->getVictim(), SPELL_PLAGUE_STRIKE);
                events.DelayEvents(1000, GCD_CAST);
                events.ScheduleEvent(SPELL_PLAGUE_STRIKE, 5000, GCD_CAST);
                break;
            case EVENT_BLOOD_STRIKE:
                DoCast(me->getVictim(), SPELL_BLOOD_STRIKE);
                events.DelayEvents(1000, GCD_CAST);
                events.ScheduleEvent(EVENT_BLOOD_STRIKE, 5000, GCD_CAST);
                break;
            case EVENT_DEATH_COIL:
                DoCast(me->getVictim(), SPELL_DEATH_COIL);
                events.DelayEvents(1000, GCD_CAST);
                events.ScheduleEvent(EVENT_DEATH_COIL, 5000, GCD_CAST);
                break;
            }
        }

        DoMeleeAttackIfReady();
    }
}

CreatureAI* GetAI_npc_unworthy_initiate_anchor(Creature* pCreature)
{
    return new npc_unworthy_initiate_anchorAI(pCreature);
}

bool GOHello_go_acherus_soul_prison(Player* pPlayer, GameObject* pGo)
{
    if (Creature *anchor = pGo->FindNearestCreature(29521, 15))
        if (uint64 prisonerGUID = anchor->AI()->GetGUID())
            if (Creature* prisoner = Creature::GetCreature(*pPlayer, prisonerGUID))
                CAST_AI(npc_unworthy_initiateAI, prisoner->AI())->EventStart(anchor, pPlayer);

    return false;
}

/*######
## npc_death_knight_initiate
######*/

#define GOSSIP_ACCEPT_DUEL      "I challenge you, death knight!"

enum eDuelEnums
{
    SAY_DUEL_A                  = -1609080,
    SAY_DUEL_B                  = -1609081,
    SAY_DUEL_C                  = -1609082,
    SAY_DUEL_D                  = -1609083,
    SAY_DUEL_E                  = -1609084,
    SAY_DUEL_F                  = -1609085,
    SAY_DUEL_G                  = -1609086,
    SAY_DUEL_H                  = -1609087,
    SAY_DUEL_I                  = -1609088,

    SPELL_DUEL                  = 52996,
    //SPELL_DUEL_TRIGGERED        = 52990,
    SPELL_DUEL_VICTORY          = 52994,
    SPELL_DUEL_FLAG             = 52991,

    QUEST_DEATH_CHALLENGE       = 12733,
    FACTION_HOSTILE             = 2068
};

int32 m_auiRandomSay[] =
{
    SAY_DUEL_A, SAY_DUEL_B, SAY_DUEL_C, SAY_DUEL_D, SAY_DUEL_E, SAY_DUEL_F, SAY_DUEL_G, SAY_DUEL_H, SAY_DUEL_I
};

struct CW_DLL_DECL npc_death_knight_initiateAI : public CombatAI
{
    npc_death_knight_initiateAI(Creature* pCreature) : CombatAI(pCreature)
    {
        m_bIsDuelInProgress = false;
    }

    bool lose;
    uint64 m_uiDuelerGUID;
    uint32 m_uiDuelTimer;
    bool m_bIsDuelInProgress;

    void Reset()
    {
        lose = false;
        me->RestoreFaction();
        CombatAI::Reset();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

        m_uiDuelerGUID = 0;
        m_uiDuelTimer = 5000;
        m_bIsDuelInProgress = false;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (!m_bIsDuelInProgress && pSpell->Id == SPELL_DUEL)
        {
            m_uiDuelerGUID = pCaster->GetGUID();
            m_bIsDuelInProgress = true;
        }
    }

   void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_bIsDuelInProgress && pDoneBy->IsControlledByPlayer())
        {
            if (pDoneBy->GetGUID() != m_uiDuelerGUID && pDoneBy->GetOwnerGUID() != m_uiDuelerGUID) // other players cannot help
                uiDamage = 0;
            else if (uiDamage >= m_creature->GetHealth())
            {
                uiDamage = 0;

                if (!lose)
                {
                    pDoneBy->RemoveGameObject(SPELL_DUEL_FLAG, true);
                    pDoneBy->AttackStop();
                    me->CastSpell(pDoneBy, SPELL_DUEL_VICTORY, true);
                    lose = true;
                    me->CastSpell(me, SPELL_ID_DUEL_BEG, true);
                    me->RestoreFaction();
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
        {
            if (m_bIsDuelInProgress)
            {
                if (m_uiDuelTimer < uiDiff)
                {
                    m_creature->setFaction(FACTION_HOSTILE);

                    if (Unit* pUnit = Unit::GetUnit(*m_creature, m_uiDuelerGUID))
                        AttackStart(pUnit);
                }
                else
                    m_uiDuelTimer -= uiDiff;
            }
            return;
        }

        if (m_bIsDuelInProgress)
        {
            if (lose)
            {
                if (!me->HasAura(SPELL_ID_DUEL_BEG))
                    EnterEvadeMode();
                return;
            }
            else if (me->getVictim()->GetTypeId() == TYPEID_PLAYER
                && me->getVictim()->GetHealth() * 10 < me->getVictim()->GetMaxHealth())
            {
                me->getVictim()->CastSpell(me->getVictim(), SPELL_ID_DUEL_BEG, true); // beg
                me->getVictim()->RemoveGameObject(SPELL_DUEL_FLAG, true);
                EnterEvadeMode();
                return;
            }
        }

        // TODO: spells

        CombatAI::UpdateAI(uiDiff);
    }
};

CreatureAI* GetAI_npc_death_knight_initiate(Creature* pCreature)
{
    return new npc_death_knight_initiateAI(pCreature);
}

bool GossipHello_npc_death_knight_initiate(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_DEATH_CHALLENGE) == QUEST_STATUS_INCOMPLETE && pCreature->GetHealth() == pCreature->GetMaxHealth())
    {
        if (pPlayer->GetHealth() * 10 < pPlayer->GetMaxHealth())
            return true;

        if (pPlayer->isInCombat() || pCreature->isInCombat())
            return true;

        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ACCEPT_DUEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(),pCreature->GetGUID());
    }
    return true;
}

bool GossipSelect_npc_death_knight_initiate(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (pPlayer->isInCombat() || pCreature->isInCombat())
            return true;

        if (npc_death_knight_initiateAI* pInitiateAI = CAST_AI(npc_death_knight_initiateAI, pCreature->AI()))
        {
            if (pInitiateAI->m_bIsDuelInProgress)
                return true;
        }

        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

        int32 uiSayId = rand()% (sizeof(m_auiRandomSay)/sizeof(int32));
        DoScriptText(m_auiRandomSay[uiSayId], pCreature, pPlayer);

        pPlayer->CastSpell(pCreature, SPELL_DUEL, false);
        pPlayer->CastSpell(pPlayer, SPELL_DUEL_FLAG, true);
    }
    return true;
}

/*######
## npc_dark_rider_of_acherus
######*/

#define DESPAWN_HORSE 52267
#define SAY_DARK_RIDER      "The realm of shadows awaits..."

struct CW_DLL_DECL npc_dark_rider_of_acherusAI : public ScriptedAI
{
    npc_dark_rider_of_acherusAI(Creature *c) : ScriptedAI(c) {}

    uint32 PhaseTimer;
    uint32 Phase;
    bool Intro;
    Unit *Target;

    void Reset()
    {
        PhaseTimer = 4000;
        Phase = 0;
        Intro = false;
        Target = NULL;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!Intro)
            return;

        if (PhaseTimer < diff)
        {
            switch(Phase)
            {
               case 0:
                    m_creature->MonsterSay(SAY_DARK_RIDER, LANG_UNIVERSAL, 0);
                    PhaseTimer = 5000;
                    Phase = 1;
                    break;
                case 1:
                    DoCast(Target, DESPAWN_HORSE, true);
                    PhaseTimer = 3000;
                    Phase = 2;
                    break;
                case 2:
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    PhaseTimer = 2000;
                    Phase = 3;
                    break;
                case 3:
                    m_creature->ForcedDespawn();
                    break;
                default:
                    break;
            }
        }else PhaseTimer -= diff;

    }

    void InitDespawnHorse(Unit *who)
    {
        if (!who)
            return;

        Target = who;
        m_creature->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
        m_creature->SetSpeed(MOVE_RUN, 0.4f);
        m_creature->GetMotionMaster()->MoveChase(Target);
        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Target->GetGUID());
        Intro = true;
    }

};

CreatureAI* GetAI_npc_dark_rider_of_acherus(Creature* pCreature)
{
    return new npc_dark_rider_of_acherusAI(pCreature);
}

/*######
## npc_salanar_the_horseman
######*/

enum eSalanar
{
    REALM_OF_SHADOWS            = 52693,
    EFFECT_STOLEN_HORSE         = 52263,
    DELIVER_STOLEN_HORSE        = 52264,
    CALL_DARK_RIDER             = 52266,
    SPELL_EFFECT_OVERTAKE       = 52349
};

struct CW_DLL_DECL npc_salanar_the_horsemanAI : public ScriptedAI
{
    npc_salanar_the_horsemanAI(Creature *c) : ScriptedAI(c) {}

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (spell->Id == DELIVER_STOLEN_HORSE)
        {
            if (caster->GetTypeId() == TYPEID_UNIT && caster->IsVehicle())
            {
                if (Unit *charmer = caster->GetCharmer())
                {
                    charmer->RemoveAurasDueToSpell(EFFECT_STOLEN_HORSE);
                    caster->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    caster->setFaction(35);
                    DoCast(caster, CALL_DARK_RIDER, true);
                    if (Creature* Dark_Rider = m_creature->FindNearestCreature(28654, 15))
                        CAST_AI(npc_dark_rider_of_acherusAI, Dark_Rider->AI())->InitDespawnHorse(caster);
                }
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        ScriptedAI::MoveInLineOfSight(who);

        if (who->GetTypeId() == TYPEID_UNIT && who->IsVehicle() && me->IsWithinDistInMap(who, 5.0f))
        {
            if (Unit *charmer = who->GetCharmer())
            {
                if (charmer->GetTypeId() == TYPEID_PLAYER)
                {
                    // for quest Into the Realm of Shadows(12687)
                    if(me->GetEntry() == 28788 && CAST_PLR(charmer)->GetQuestStatus(12687) == QUEST_STATUS_INCOMPLETE)
                    {
                        CAST_PLR(charmer)->GroupEventHappens(12687, me);
                        charmer->RemoveAurasDueToSpell(SPELL_EFFECT_OVERTAKE);
                        CAST_CRE(who)->ForcedDespawn();
                        //CAST_CRE(who)->Respawn(true);
                    }

                    if (CAST_PLR(charmer)->HasAura(REALM_OF_SHADOWS))
                        charmer->RemoveAurasDueToSpell(REALM_OF_SHADOWS);
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_salanar_the_horseman(Creature* pCreature)
{
    return new npc_salanar_the_horsemanAI(pCreature);
}

/*######
## npc_ros_dark_rider
######*/

struct CW_DLL_DECL npc_ros_dark_riderAI : public ScriptedAI
{
    npc_ros_dark_riderAI(Creature *c) : ScriptedAI(c) {}

    void EnterCombat(Unit *who)
    {
        me->ExitVehicle();
    }

    void Reset()
    {
        Creature* deathcharger = me->FindNearestCreature(28782, 30);
        if (!deathcharger) return;
        deathcharger->RestoreFaction();
        deathcharger->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        deathcharger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if (!me->GetVehicle() && deathcharger->IsVehicle() && deathcharger->GetVehicleKit()->HasEmptySeat(0))
            me->EnterVehicle(deathcharger);
    }

    void JustDied(Unit *killer)
    {
        Creature* deathcharger = me->FindNearestCreature(28782, 30);
        if (!deathcharger) return;
        if (killer->GetTypeId() == TYPEID_PLAYER && deathcharger->GetTypeId() == TYPEID_UNIT && deathcharger->IsVehicle())
        {
            deathcharger->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            deathcharger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            deathcharger->setFaction(2096);
        }
    }
};

CreatureAI* GetAI_npc_ros_dark_rider(Creature* pCreature)
{
    return new npc_ros_dark_riderAI(pCreature);
}

// correct way: 52312 52314 52555 ...
enum SG
{
    GHOULS = 28845,
    GHOSTS = 28846,
};
struct CW_DLL_DECL npc_dkc1_gothikAI : public ScriptedAI
{
    npc_dkc1_gothikAI(Creature *c) : ScriptedAI(c) {}

    void MoveInLineOfSight(Unit *who)
    {
        ScriptedAI::MoveInLineOfSight(who);

        if (who->GetEntry() == GHOULS && me->IsWithinDistInMap(who, 10.0f))
        {
            if (Unit *owner = who->GetOwner())
            {
                if (owner->GetTypeId() == TYPEID_PLAYER)
                {
                    if (CAST_PLR(owner)->GetQuestStatus(12698) == QUEST_STATUS_INCOMPLETE)
                    {
                        //CAST_CRE(who)->CastSpell(owner, 52517, true);
                        CAST_PLR(owner)->KilledMonsterCredit(GHOULS, me->GetGUID());
                    }
                    //Todo: Creatures must not be removed, but, must instead
                    //      stand next to Gothik and be commanded into the pit
                    //      and dig into the ground.
                    CAST_CRE(who)->ForcedDespawn();

                    if (CAST_PLR(owner)->GetQuestStatus(12698) == QUEST_STATUS_COMPLETE)
                        owner->RemoveAllMinionsByEntry(GHOULS);
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_dkc1_gothik(Creature* pCreature)
{
    return new npc_dkc1_gothikAI(pCreature);
}

struct CW_DLL_DECL npc_scarlet_ghoulAI : public ScriptedAI
{
    npc_scarlet_ghoulAI(Creature *c) : ScriptedAI(c)
    {
        // Ghouls should display their Birth Animation
        // Crawling out of the ground
        //m_creature->CastSpell(m_creature,35177,true);
        //m_creature->MonsterSay("Mommy?",LANG_UNIVERSAL,0);
        m_creature->SetReactState(REACT_DEFENSIVE);
    }

    void FindMinions(Unit *owner)
    {
        std::list<Creature*> MinionList;
        owner->GetAllMinionsByEntry(MinionList,GHOULS);

        if (!MinionList.empty())
        {
            for (std::list<Creature*>::iterator itr = MinionList.begin(); itr != MinionList.end(); ++itr)
            {
                if (CAST_CRE(*itr)->GetOwner()->GetGUID() == m_creature->GetOwner()->GetGUID())
                {
                    if (CAST_CRE(*itr)->isInCombat() && CAST_CRE(*itr)->getAttackerForHelper())
                    {
                        AttackStart(CAST_CRE(*itr)->getAttackerForHelper());
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->isInCombat())
        {
            if (Unit *owner = m_creature->GetOwner())
            {
                if (owner->GetTypeId() == TYPEID_PLAYER && CAST_PLR(owner)->isInCombat())
                {
                    if (CAST_PLR(owner)->getAttackerForHelper() && CAST_PLR(owner)->getAttackerForHelper()->GetEntry() == GHOSTS)
                    {
                        AttackStart(CAST_PLR(owner)->getAttackerForHelper());
                    }
                    else
                    {
                        FindMinions(owner);
                    }
                }
            }
        }

        if (!UpdateVictim())
            return;

        //ScriptedAI::UpdateAI(diff);
        //Check if we have a current target
        if (m_creature->getVictim()->GetEntry() == GHOSTS)
        {
            if (m_creature->isAttackReady())
            {
                //If we are within range melee the target
                if (m_creature->IsWithinMeleeRange(m_creature->getVictim()))
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
};

CreatureAI* GetAI_npc_scarlet_ghoul(Creature* pCreature)
{
    return new npc_scarlet_ghoulAI(pCreature);
}

/*####
## npc_scarlet_miner_cart
####*/

#define SPELL_CART_CHECK     54173
#define SPELL_CART_DRAG      52465

struct CW_DLL_DECL npc_scarlet_miner_cartAI : public PassiveAI
{
    npc_scarlet_miner_cartAI(Creature *c) : PassiveAI(c), minerGUID(0)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        me->SetDisplayId(me->GetCreatureInfo()->Modelid1); // H0 is horse
    }

    uint64 minerGUID;

    void SetGUID(const uint64 &guid, int32 id)
    {
        minerGUID = guid;
    }

    void DoAction(const int32 param)
    {
        if(Creature *miner = Unit::GetCreature(*me, minerGUID))
        {
            // very bad visual effect
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
            //me->SetSpeed(MOVE_WALK, miner->GetSpeed(MOVE_WALK));
            me->SetSpeed(MOVE_RUN, miner->GetSpeed(MOVE_RUN));
            me->GetMotionMaster()->MoveFollow(miner, 1.0f, 0);
        }
    }

    void PassengerBoarded(Unit *who, int8 seatId, bool apply)
    {
        if(!apply)
            if(Creature *miner = Unit::GetCreature(*me, minerGUID))
                miner->DisappearAndDie();
    }
};

CreatureAI* GetAI_npc_scarlet_miner_cart(Creature *_Creature)
{
    return new npc_scarlet_miner_cartAI(_Creature);
}

/*####
## npc_scarlet_miner
####*/

#define SAY_SCARLET_MINER1  "Where'd this come from? I better get this down to the ships before the foreman sees it!"
#define SAY_SCARLET_MINER2  "Now I can have a rest!"

struct CW_DLL_DECL npc_scarlet_minerAI : public npc_escortAI
{
    npc_scarlet_minerAI(Creature *c) : npc_escortAI(c)
    {
        me->SetReactState(REACT_PASSIVE);
    }

    uint32 IntroTimer;
    uint32 IntroPhase;
    uint64 carGUID;

    void Reset()
    {
        carGUID = 0;
        IntroTimer = 0;
        IntroPhase = 0;
    }

    void InitWaypoint()
    {
        AddWaypoint(1, 2389.03,     -5902.74,     109.014, 5000);
        AddWaypoint(2, 2341.812012, -5900.484863, 102.619743);
        AddWaypoint(3, 2306.561279, -5901.738281, 91.792419 );
        AddWaypoint(4, 2300.098389, -5912.618652, 86.014885 );
        AddWaypoint(5, 2294.142090, -5927.274414, 75.316849 );
        AddWaypoint(6, 2286.984375, -5944.955566, 63.714966 );
        AddWaypoint(7, 2280.001709, -5961.186035, 54.228283 );
        AddWaypoint(8, 2259.389648, -5974.197754, 42.359348 );
        AddWaypoint(9, 2242.882812, -5984.642578, 32.827850 );
        AddWaypoint(10, 2217.265625, -6028.959473, 7.675705 );
        AddWaypoint(11, 2202.595947, -6061.325684, 5.882018 );
        AddWaypoint(12, 2188.974609, -6080.866699, 3.370027 );

        if(rand()%2)
        {
            AddWaypoint(13, 2176.483887, -6110.407227, 1.855181 );
            AddWaypoint(14, 2172.516602, -6146.752441, 1.074235 );
            AddWaypoint(15, 2138.918457, -6158.920898, 1.342926 );
            AddWaypoint(16, 2129.866699, -6174.107910, 4.380779 );
            AddWaypoint(17, 2117.709473, -6193.830078, 13.3542, 10000);
        }
        else
        {
            AddWaypoint(13, 2184.190186, -6166.447266, 0.968877 );
            AddWaypoint(14, 2234.265625, -6163.741211, 0.916021 );
            AddWaypoint(15, 2268.071777, -6158.750977, 1.822252 );
            AddWaypoint(16, 2270.028320, -6176.505859, 6.340538 );
            AddWaypoint(17, 2271.739014, -6195.401855, 13.3542, 10000);
        }
    }

    void InitCartQuest(Player *who)
    {
        carGUID = who->GetVehicleBase()->GetGUID();
        InitWaypoint();
        Start(false, false, who->GetGUID());
        SetDespawnAtFar(false);
    }

    void WaypointReached(uint32 i)
    {
        switch (i)
        {
            case 1:
                if(Unit *car = Unit::GetCreature(*me, carGUID))
                {
                    me->SetInFront(car);
                    me->SendMovementFlagUpdate();
                }
                me->MonsterSay(SAY_SCARLET_MINER1,LANG_UNIVERSAL,NULL);
                SetRun(true);
                IntroTimer = 4000;
                IntroPhase = 1;
                break;
            case 17:
                if(Unit *car = Unit::GetCreature(*me, carGUID))
                {
                    me->SetInFront(car);
                    me->SendMovementFlagUpdate();
                    car->Relocate(car->GetPositionX(), car->GetPositionY(), me->GetPositionZ());
                    car->SendMonsterStop();
                    //this make player fall under ground, dunno why
                    //car->GetMotionMaster()->MovePoint(0, car->GetPositionX(), car->GetPositionY(), me->GetPositionZ());
                    car->RemoveAura(SPELL_CART_DRAG);
                }
                me->MonsterSay(SAY_SCARLET_MINER2,LANG_UNIVERSAL,NULL);
                break;
            default:
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (IntroPhase)
        {
            if (IntroTimer < diff)
            {
                if (IntroPhase == 1)
                {
                    if(Creature *car = Unit::GetCreature(*me, carGUID))
                        DoCast(car, SPELL_CART_DRAG);
                    IntroTimer = 800;
                    IntroPhase = 2;
                }
                else
                {
                    if(Creature *car = Unit::GetCreature(*me, carGUID))
                        car->AI()->DoAction();
                    IntroPhase = 0;
                }
            }else IntroTimer-=diff;
        }
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_scarlet_miner(Creature *_Creature)
{
    return new npc_scarlet_minerAI(_Creature);
}

/*######
## go_inconspicuous_mine_car
######*/

#define SPELL_CART_SUMM   52463

bool GOHello_go_inconspicuous_mine_car(Player* pPlayer, GameObject* pGO)
{
    if (pPlayer->GetQuestStatus(12701) == QUEST_STATUS_INCOMPLETE)
    {
        // Hack Why CW Dont Support Custom Summon Location
        if(Creature *miner = pPlayer->SummonCreature(28841, 2383.869629, -5900.312500, 107.996086, pPlayer->GetOrientation(),TEMPSUMMON_DEAD_DESPAWN, 1))
        {
            pPlayer->CastSpell(pPlayer, SPELL_CART_SUMM, true);
            if(Creature *car = pPlayer->GetVehicleCreatureBase())
            {
                if(car->GetEntry() == 28817)
                {
                    car->AI()->SetGUID(miner->GetGUID());
                    CAST_AI(npc_scarlet_minerAI, miner->AI())->InitCartQuest(pPlayer);
                }else error_log("TSCR: GOHello_go_inconspicuous_mine_car vehicle entry is not correct.");
            }else error_log("TSCR: GOHello_go_inconspicuous_mine_car player is not on the vehicle.");
        }else error_log("TSCR: GOHello_go_inconspicuous_mine_car Scarlet Miner cant be found by script.");
    }
    return true;
}

// npc 28912 quest 17217 boss 29001 mob 29007 go 191092

void AddSC_the_scarlet_enclave_c1()
{
    Script *newscript;

    // 12848 The Endless Hunger
    newscript = new Script;
    newscript->Name = "npc_unworthy_initiate";
    newscript->GetAI = &GetAI_npc_unworthy_initiate;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_unworthy_initiate_anchor";
    newscript->GetAI = &GetAI_npc_unworthy_initiate_anchor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_acherus_soul_prison";
    newscript->pGOHello = &GOHello_go_acherus_soul_prison;
    newscript->RegisterSelf();

    // Death's Challenge
    newscript = new Script;
    newscript->Name = "npc_death_knight_initiate";
    newscript->GetAI = &GetAI_npc_death_knight_initiate;
    newscript->pGossipHello = &GossipHello_npc_death_knight_initiate;
    newscript->pGossipSelect = &GossipSelect_npc_death_knight_initiate;
    newscript->RegisterSelf();

    // 12680 Grand Theft Palomino
    newscript = new Script;
    newscript->Name = "npc_salanar_the_horseman";
    newscript->GetAI = &GetAI_npc_salanar_the_horseman;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_dark_rider_of_acherus";
    newscript->GetAI = &GetAI_npc_dark_rider_of_acherus;
    newscript->RegisterSelf();

    // 12687 Into the Realm of Shadows
    newscript = new Script;
    newscript->Name = "npc_ros_dark_rider";
    newscript->GetAI = &GetAI_npc_ros_dark_rider;
    newscript->RegisterSelf();

    // 12698 The Gift That Keeps On Giving
    newscript = new Script;
    newscript->Name = "npc_dkc1_gothik";
    newscript->GetAI = &GetAI_npc_dkc1_gothik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_scarlet_ghoul";
    newscript->GetAI = &GetAI_npc_scarlet_ghoul;
    newscript->RegisterSelf();

    // Massacre At Light's Point
    newscript = new Script;
    newscript->Name = "npc_scarlet_miner";
    newscript->GetAI = &GetAI_npc_scarlet_miner;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_scarlet_miner_cart";
    newscript->GetAI = &GetAI_npc_scarlet_miner_cart;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_inconspicuous_mine_car";
    newscript->pGOHello = &GOHello_go_inconspicuous_mine_car;
    newscript->RegisterSelf();
}

/*
DELETE FROM `script_texts` WHERE `entry` IN(-1609301, -1609302);
INSERT INTO `script_texts` (`entry`,`content_default`,`type`,`language`,`emote`,`comment`) VALUES
(-1609301, 'Come, weakling! Strike me down!', 0, 0, 0, 'SAY_DEATH_RIDER_FINAL'),
(-1609302, '%s rears up, beckoning you to ride it.', 2, 0, 0, 'SAY_DEATH_CHARGER');
*/
