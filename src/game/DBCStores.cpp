/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * Copyright (C) 2009 CWCore <http://www.wow-extrem.de/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "DBCStores.h"
#include "Policies/SingletonImp.h"
#include "Log.h"
#include "ProgressBar.h"
#include "SharedDefines.h"
#include "SpellMgr.h"

#include "DBCfmt.h"

#include <map>

typedef std::map<uint16,uint32> AreaFlagByAreaID;
typedef std::map<uint32,uint32> AreaFlagByMapID;

DBCStorage <AreaTableEntry> sAreaStore(AreaTableEntryfmt);
DBCStorage <AreaGroupEntry> sAreaGroupStore(AreaGroupEntryfmt);
DBCStorage <AreaPOIEntry> sAreaPOIStore(AreaPOIEntryfmt);
static AreaFlagByAreaID sAreaFlagByAreaID;
static AreaFlagByMapID  sAreaFlagByMapID;                   // for instances without generated *.map files

DBCStorage <AchievementEntry> sAchievementStore(Achievementfmt);
DBCStorage <AchievementCriteriaEntry> sAchievementCriteriaStore(AchievementCriteriafmt);
DBCStorage <AreaTriggerEntry> sAreaTriggerStore(AreaTriggerEntryfmt);
DBCStorage <AuctionHouseEntry> sAuctionHouseStore(AuctionHouseEntryfmt);
DBCStorage <BankBagSlotPricesEntry> sBankBagSlotPricesStore(BankBagSlotPricesEntryfmt);
DBCStorage <BattlemasterListEntry> sBattlemasterListStore(BattlemasterListEntryfmt);
DBCStorage <BarberShopStyleEntry> sBarberShopStyleStore(BarberShopStyleEntryfmt);
DBCStorage <CharStartOutfitEntry> sCharStartOutfitStore(CharStartOutfitEntryfmt);
DBCStorage <CharTitlesEntry> sCharTitlesStore(CharTitlesEntryfmt);
DBCStorage <ChatChannelsEntry> sChatChannelsStore(ChatChannelsEntryfmt);
DBCStorage <ChrClassesEntry> sChrClassesStore(ChrClassesEntryfmt);
DBCStorage <ChrRacesEntry> sChrRacesStore(ChrRacesEntryfmt);
DBCStorage <CinematicSequencesEntry> sCinematicSequencesStore(CinematicSequencesEntryfmt);
DBCStorage <CreatureDisplayInfoEntry> sCreatureDisplayInfoStore(CreatureDisplayInfofmt);
DBCStorage <CreatureFamilyEntry> sCreatureFamilyStore(CreatureFamilyfmt);
DBCStorage <CreatureSpellDataEntry> sCreatureSpellDataStore(CreatureSpellDatafmt);
DBCStorage <CreatureTypeEntry> sCreatureTypeStore(CreatureTypefmt);
DBCStorage <CurrencyTypesEntry> sCurrencyTypesStore(CurrencyTypesfmt);

DBCStorage <DurabilityQualityEntry> sDurabilityQualityStore(DurabilityQualityfmt);
DBCStorage <DurabilityCostsEntry> sDurabilityCostsStore(DurabilityCostsfmt);

DBCStorage <EmotesEntry> sEmotesStore(EmotesEntryfmt);
DBCStorage <EmotesTextEntry> sEmotesTextStore(EmotesTextEntryfmt);

typedef std::map<uint32,SimpleFactionsList> FactionTeamMap;
static FactionTeamMap sFactionTeamMap;
DBCStorage <FactionEntry> sFactionStore(FactionEntryfmt);
DBCStorage <FactionTemplateEntry> sFactionTemplateStore(FactionTemplateEntryfmt);

DBCStorage <GameObjectDisplayInfoEntry> sGameObjectDisplayInfoStore(GameObjectDisplayInfofmt);
DBCStorage <GemPropertiesEntry> sGemPropertiesStore(GemPropertiesEntryfmt);
DBCStorage <GlyphPropertiesEntry> sGlyphPropertiesStore(GlyphPropertiesfmt);
DBCStorage <GlyphSlotEntry> sGlyphSlotStore(GlyphSlotfmt);

DBCStorage <GtBarberShopCostBaseEntry>    sGtBarberShopCostBaseStore(GtBarberShopCostBasefmt);
DBCStorage <GtCombatRatingsEntry>         sGtCombatRatingsStore(GtCombatRatingsfmt);
DBCStorage <GtChanceToMeleeCritBaseEntry> sGtChanceToMeleeCritBaseStore(GtChanceToMeleeCritBasefmt);
DBCStorage <GtChanceToMeleeCritEntry>     sGtChanceToMeleeCritStore(GtChanceToMeleeCritfmt);
DBCStorage <GtChanceToSpellCritBaseEntry> sGtChanceToSpellCritBaseStore(GtChanceToSpellCritBasefmt);
DBCStorage <GtChanceToSpellCritEntry>     sGtChanceToSpellCritStore(GtChanceToSpellCritfmt);
DBCStorage <GtOCTRegenHPEntry>            sGtOCTRegenHPStore(GtOCTRegenHPfmt);
//DBCStorage <GtOCTRegenMPEntry>            sGtOCTRegenMPStore(GtOCTRegenMPfmt);  -- not used currently
DBCStorage <GtRegenHPPerSptEntry>         sGtRegenHPPerSptStore(GtRegenHPPerSptfmt);
DBCStorage <GtRegenMPPerSptEntry>         sGtRegenMPPerSptStore(GtRegenMPPerSptfmt);

DBCStorage <HolidaysEntry>                sHolidaysStore(Holidaysfmt);

DBCStorage <ItemEntry>                    sItemStore(Itemfmt);
DBCStorage <ItemBagFamilyEntry>           sItemBagFamilyStore(ItemBagFamilyfmt);
//DBCStorage <ItemCondExtCostsEntry> sItemCondExtCostsStore(ItemCondExtCostsEntryfmt);
//DBCStorage <ItemDisplayInfoEntry> sItemDisplayInfoStore(ItemDisplayTemplateEntryfmt); -- not used currently
DBCStorage <ItemExtendedCostEntry> sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DBCStorage <ItemLimitCategoryEntry> sItemLimitCategoryStore(ItemLimitCategoryEntryfmt);
DBCStorage <ItemRandomPropertiesEntry> sItemRandomPropertiesStore(ItemRandomPropertiesfmt);
DBCStorage <ItemRandomSuffixEntry> sItemRandomSuffixStore(ItemRandomSuffixfmt);
DBCStorage <ItemSetEntry> sItemSetStore(ItemSetEntryfmt);

DBCStorage <LockEntry> sLockStore(LockEntryfmt);

DBCStorage <MailTemplateEntry> sMailTemplateStore(MailTemplateEntryfmt);
DBCStorage <MapEntry> sMapStore(MapEntryfmt);
DBCStorage <MovieEntry> sMovieStore(MovieEntryfmt);

DBCStorage <QuestSortEntry> sQuestSortStore(QuestSortEntryfmt);

DBCStorage <RandomPropertiesPointsEntry> sRandomPropertiesPointsStore(RandomPropertiesPointsfmt);
DBCStorage <ScalingStatDistributionEntry> sScalingStatDistributionStore(ScalingStatDistributionfmt);
DBCStorage <ScalingStatValuesEntry> sScalingStatValuesStore(ScalingStatValuesfmt);

DBCStorage <SkillLineEntry> sSkillLineStore(SkillLinefmt);
DBCStorage <SkillLineAbilityEntry> sSkillLineAbilityStore(SkillLineAbilityfmt);

DBCStorage <SoundEntriesEntry> sSoundEntriesStore(SoundEntriesfmt);

DBCStorage <SpellItemEnchantmentEntry> sSpellItemEnchantmentStore(SpellItemEnchantmentfmt);
DBCStorage <SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore(SpellItemEnchantmentConditionfmt);
DBCStorage <SpellEntry> sSpellStore(SpellEntryfmt);
SpellCategoryStore sSpellCategoryStore;
PetFamilySpellsStore sPetFamilySpellsStore;

DBCStorage <SpellCastTimesEntry> sSpellCastTimesStore(SpellCastTimefmt);
DBCStorage <SpellDurationEntry> sSpellDurationStore(SpellDurationfmt);
DBCStorage <SpellFocusObjectEntry> sSpellFocusObjectStore(SpellFocusObjectfmt);
DBCStorage <SpellRadiusEntry> sSpellRadiusStore(SpellRadiusfmt);
DBCStorage <SpellRangeEntry> sSpellRangeStore(SpellRangefmt);
DBCStorage <SpellRuneCostEntry> sSpellRuneCostStore(SpellRuneCostfmt);
DBCStorage <SpellShapeshiftEntry> sSpellShapeshiftStore(SpellShapeshiftfmt);
DBCStorage <StableSlotPricesEntry> sStableSlotPricesStore(StableSlotPricesfmt);
DBCStorage <SummonPropertiesEntry> sSummonPropertiesStore(SummonPropertiesfmt);
DBCStorage <TalentEntry> sTalentStore(TalentEntryfmt);
TalentSpellPosMap sTalentSpellPosMap;
DBCStorage <TalentTabEntry> sTalentTabStore(TalentTabEntryfmt);

// store absolute bit position for first rank for talent inspect
static uint32 sTalentTabPages[12/*MAX_CLASSES*/][3];

DBCStorage <TaxiNodesEntry> sTaxiNodesStore(TaxiNodesEntryfmt);
TaxiMask sTaxiNodesMask;
TaxiMask sOldContinentsNodesMask;

// DBC used only for initialization sTaxiPathSetBySource at startup.
TaxiPathSetBySource sTaxiPathSetBySource;
DBCStorage <TaxiPathEntry> sTaxiPathStore(TaxiPathEntryfmt);

// DBC used only for initialization sTaxiPathSetBySource at startup.
TaxiPathNodesByPath sTaxiPathNodesByPath;

static DBCStorage <TaxiPathNodeEntry> sTaxiPathNodeStore(TaxiPathNodeEntryfmt);
DBCStorage <TotemCategoryEntry> sTotemCategoryStore(TotemCategoryEntryfmt);
DBCStorage <VehicleEntry> sVehicleStore(VehicleEntryfmt);
DBCStorage <VehicleSeatEntry> sVehicleSeatStore(VehicleSeatEntryfmt);
DBCStorage <WorldMapAreaEntry>  sWorldMapAreaStore(WorldMapAreaEntryfmt);
DBCStorage <WorldMapOverlayEntry> sWorldMapOverlayStore(WorldMapOverlayEntryfmt);
DBCStorage <WorldSafeLocsEntry> sWorldSafeLocsStore(WorldSafeLocsEntryfmt);

typedef std::list<std::string> StoreProblemList;

static bool LoadDBC_assert_print(uint32 fsize,uint32 rsize, const std::string& filename)
{
    sLog.outError("ERROR: Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).",filename.c_str(),fsize,rsize);

    // assert must fail after function call
    return false;
}

template<class T>
inline void LoadDBC(uint32& availableDbcLocales,barGoLink& bar, StoreProblemList& errlist, DBCStorage<T>& storage, const std::string& dbc_path, const std::string& filename)
{
    // compatibility format and C++ structure sizes
    assert(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()),sizeof(T),filename));

    std::string dbc_filename = dbc_path + filename;
    if(storage.Load(dbc_filename.c_str()))
    {
        bar.step();
        for(uint8 i = 0; i < MAX_LOCALE; ++i)
        {
            if(!(availableDbcLocales & (1 << i)))
                continue;

            std::string dbc_filename_loc = dbc_path + localeNames[i] + "/" + filename;
            if(!storage.LoadStringsFrom(dbc_filename_loc.c_str()))
                availableDbcLocales &= ~(1<<i);             // mark as not available for speedup next checks
        }
    }
    else
    {
        // sort problematic dbc to (1) non compatible and (2) non-existed
        FILE * f=fopen(dbc_filename.c_str(),"rb");
        if(f)
        {
            char buf[100];
            snprintf(buf,100," (exist, but have %d fields instead " SIZEFMTD ") Wrong client version DBC file?",storage.GetFieldCount(),strlen(storage.GetFormat()));
            errlist.push_back(dbc_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(dbc_filename);
    }
}

void LoadDBCStores(const std::string& dataPath)
{
    std::string dbcPath = dataPath+"dbc/";

    const uint32 DBCFilesCount = 79;

    barGoLink bar( DBCFilesCount );

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAreaStore,                dbcPath,"AreaTable.dbc");

    // must be after sAreaStore loading
    for(uint32 i = 0; i < sAreaStore.GetNumRows(); ++i)           // areaflag numbered from 0
    {
        if(AreaTableEntry const* area = sAreaStore.LookupEntry(i))
        {
            // fill AreaId->DBC records
            sAreaFlagByAreaID.insert(AreaFlagByAreaID::value_type(uint16(area->ID),area->exploreFlag));

            // fill MapId->DBC records ( skip sub zones and continents )
            if(area->zone==0 && area->mapid != 0 && area->mapid != 1 && area->mapid != 530 && area->mapid != 571 )
                sAreaFlagByMapID.insert(AreaFlagByMapID::value_type(area->mapid,area->exploreFlag));
        }
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAchievementStore,         dbcPath,"Achievement.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAchievementCriteriaStore, dbcPath,"Achievement_Criteria.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAreaTriggerStore,         dbcPath,"AreaTrigger.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAreaGroupStore,           dbcPath,"AreaGroup.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAreaPOIStore,             dbcPath,"AreaPOI.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sAuctionHouseStore,        dbcPath,"AuctionHouse.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sBankBagSlotPricesStore,   dbcPath,"BankBagSlotPrices.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sBattlemasterListStore,    dbcPath,"BattlemasterList.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sBarberShopStyleStore,     dbcPath,"BarberShopStyle.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCharStartOutfitStore,     dbcPath,"CharStartOutfit.dbc");

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCharTitlesStore,          dbcPath,"CharTitles.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sChatChannelsStore,        dbcPath,"ChatChannels.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sChrClassesStore,          dbcPath,"ChrClasses.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sChrRacesStore,            dbcPath,"ChrRaces.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCinematicSequencesStore,  dbcPath,"CinematicSequences.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCreatureDisplayInfoStore, dbcPath,"CreatureDisplayInfo.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCreatureFamilyStore,      dbcPath,"CreatureFamily.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCreatureSpellDataStore,   dbcPath,"CreatureSpellData.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCreatureTypeStore,        dbcPath,"CreatureType.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sCurrencyTypesStore,       dbcPath,"CurrencyTypes.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sDurabilityCostsStore,     dbcPath,"DurabilityCosts.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sDurabilityQualityStore,   dbcPath,"DurabilityQuality.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sEmotesStore,              dbcPath,"Emotes.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sEmotesTextStore,          dbcPath,"EmotesText.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sFactionStore,             dbcPath,"Faction.dbc");
    for (uint32 i=0;i<sFactionStore.GetNumRows(); ++i)
    {
        FactionEntry const * faction = sFactionStore.LookupEntry(i);
        if (faction && faction->team)
        {
            SimpleFactionsList &flist = sFactionTeamMap[faction->team];
            flist.push_back(i);
        }
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sFactionTemplateStore,     dbcPath,"FactionTemplate.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGameObjectDisplayInfoStore, dbcPath,"GameObjectDisplayInfo.dbc");
    for(uint32 i = 0; i < sGameObjectDisplayInfoStore.GetNumRows(); ++i)
    {
        if(GameObjectDisplayInfoEntry const * info = sGameObjectDisplayInfoStore.LookupEntry(i))
        {
            if(info->maxX < info->minX)
                std::swap(*(float*)(&info->maxX), *(float*)(&info->minX));
            if(info->maxY < info->minY)
                std::swap(*(float*)(&info->maxY), *(float*)(&info->minY));
            if(info->maxZ < info->minZ)
                std::swap(*(float*)(&info->maxZ), *(float*)(&info->minZ));
        }
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGemPropertiesStore,       dbcPath,"GemProperties.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGlyphPropertiesStore,     dbcPath,"GlyphProperties.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGlyphSlotStore,           dbcPath,"GlyphSlot.dbc");

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtBarberShopCostBaseStore,dbcPath,"gtBarberShopCostBase.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtCombatRatingsStore,     dbcPath,"gtCombatRatings.dbc");

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtChanceToMeleeCritBaseStore, dbcPath,"gtChanceToMeleeCritBase.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtChanceToMeleeCritStore, dbcPath,"gtChanceToMeleeCrit.dbc");

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtChanceToSpellCritBaseStore, dbcPath,"gtChanceToSpellCritBase.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtChanceToSpellCritStore, dbcPath,"gtChanceToSpellCrit.dbc");

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtOCTRegenHPStore,        dbcPath,"gtOCTRegenHP.dbc");
    //LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtOCTRegenMPStore,        dbcPath,"gtOCTRegenMP.dbc");       -- not used currently
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtRegenHPPerSptStore,     dbcPath,"gtRegenHPPerSpt.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sGtRegenMPPerSptStore,     dbcPath,"gtRegenMPPerSpt.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sHolidaysStore,            dbcPath,"Holidays.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemStore,                dbcPath,"Item.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemBagFamilyStore,       dbcPath,"ItemBagFamily.dbc");
    //LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemDisplayInfoStore,     dbcPath,"ItemDisplayInfo.dbc");     -- not used currently
    //LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemCondExtCostsStore,    dbcPath,"ItemCondExtCosts.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemExtendedCostStore,    dbcPath,"ItemExtendedCost.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemLimitCategoryStore,   dbcPath,"ItemLimitCategory.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemRandomPropertiesStore,dbcPath,"ItemRandomProperties.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemRandomSuffixStore,    dbcPath,"ItemRandomSuffix.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sItemSetStore,             dbcPath,"ItemSet.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sLockStore,                dbcPath,"Lock.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sMailTemplateStore,        dbcPath,"MailTemplate.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sMapStore,                 dbcPath,"Map.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sMovieStore,               dbcPath,"Movie.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sQuestSortStore,           dbcPath,"QuestSort.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sRandomPropertiesPointsStore, dbcPath,"RandPropPoints.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sScalingStatDistributionStore, dbcPath,"ScalingStatDistribution.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sScalingStatValuesStore,   dbcPath,"ScalingStatValues.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSkillLineStore,           dbcPath,"SkillLine.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSkillLineAbilityStore,    dbcPath,"SkillLineAbility.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSoundEntriesStore,        dbcPath,"SoundEntries.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellStore,               dbcPath,"Spell.dbc");
    for(uint32 i = 1; i < sSpellStore.GetNumRows(); ++i)
    {
        SpellEntry const * spell = sSpellStore.LookupEntry(i);
        if(spell && spell->Category)
            sSpellCategoryStore[spell->Category].insert(i);
    }

    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);

        if(!skillLine)
            continue;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);

        if(spellInfo && IsPassiveSpell(spellInfo->Id))
        {
            for (uint32 i = 1; i < sCreatureFamilyStore.GetNumRows(); ++i)
            {
                CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(i);
                if(!cFamily)
                    continue;

                if(skillLine->skillId != cFamily->skillLine[0] && skillLine->skillId != cFamily->skillLine[1])
                    continue;
                if(spellInfo->spellLevel)
                    continue;

                if(skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                sPetFamilySpellsStore[i].insert(spellInfo->Id);
            }
        }
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellCastTimesStore,      dbcPath,"SpellCastTimes.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellDurationStore,       dbcPath,"SpellDuration.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellFocusObjectStore,    dbcPath,"SpellFocusObject.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellItemEnchantmentStore,dbcPath,"SpellItemEnchantment.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellItemEnchantmentConditionStore,dbcPath,"SpellItemEnchantmentCondition.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellRadiusStore,         dbcPath,"SpellRadius.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellRangeStore,          dbcPath,"SpellRange.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellRuneCostStore,       dbcPath,"SpellRuneCost.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSpellShapeshiftStore,     dbcPath,"SpellShapeshiftForm.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sStableSlotPricesStore,    dbcPath,"StableSlotPrices.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sSummonPropertiesStore,    dbcPath,"SummonProperties.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sTalentStore,              dbcPath,"Talent.dbc");

    // create talent spells set
    for (unsigned int i = 0; i < sTalentStore.GetNumRows(); ++i)
    {
        TalentEntry const *talentInfo = sTalentStore.LookupEntry(i);
        if (!talentInfo) continue;
        for (int j = 0; j < MAX_TALENT_RANK; j++)
            if(talentInfo->RankID[j])
                sTalentSpellPosMap[talentInfo->RankID[j]] = TalentSpellPos(i,j);
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sTalentTabStore,           dbcPath,"TalentTab.dbc");

    // prepare fast data access to bit pos of talent ranks for use at inspecting
    {
        // now have all max ranks (and then bit amount used for store talent ranks in inspect)
        for(uint32 talentTabId = 1; talentTabId < sTalentTabStore.GetNumRows(); ++talentTabId)
        {
            TalentTabEntry const *talentTabInfo = sTalentTabStore.LookupEntry( talentTabId );
            if(!talentTabInfo)
                continue;

            // prevent memory corruption; otherwise cls will become 12 below
            if ((talentTabInfo->ClassMask & CLASSMASK_ALL_PLAYABLE)==0)
                continue;

            // store class talent tab pages
            uint32 cls = 1;
            for(uint32 m=1;!(m & talentTabInfo->ClassMask) && cls < MAX_CLASSES;m <<=1, ++cls) {}

            sTalentTabPages[cls][talentTabInfo->tabpage]=talentTabId;
        }
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sTaxiNodesStore,           dbcPath,"TaxiNodes.dbc");

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sTaxiPathStore,            dbcPath,"TaxiPath.dbc");
    for(uint32 i = 1; i < sTaxiPathStore.GetNumRows(); ++i)
        if(TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i))
            sTaxiPathSetBySource[entry->from][entry->to] = TaxiPathBySourceAndDestination(entry->ID,entry->price);
    uint32 pathCount = sTaxiPathStore.GetNumRows();

    //## TaxiPathNode.dbc ## Loaded only for initialization different structures
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sTaxiPathNodeStore,        dbcPath,"TaxiPathNode.dbc");
    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           // 0 and some other indexes not used
    for(uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if(TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
        {
            if (pathLength[entry->path] < entry->index + 1)
                pathLength[entry->path] = entry->index + 1;
        }
    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for(uint32 i = 1; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);
    // fill data
    for(uint32 i = 1; i < sTaxiPathNodeStore.GetNumRows(); ++i)
        if(TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(i))
            sTaxiPathNodesByPath[entry->path][entry->index] = TaxiPathNode(entry->mapid,entry->x,entry->y,entry->z,entry->actionFlag,entry->delay);
    sTaxiPathNodeStore.Clear();

    // Initialize global taxinodes mask
    // include existed nodes that have at least single not spell base (scripted) path
    {
        std::set<uint32> spellPaths;
        for(uint32 i = 1; i < sSpellStore.GetNumRows (); ++i)
            if(SpellEntry const* sInfo = sSpellStore.LookupEntry (i))
                for(int j=0; j < 3; ++j)
                    if(sInfo->Effect[j]==123 /*SPELL_EFFECT_SEND_TAXI*/)
                        spellPaths.insert(sInfo->EffectMiscValue[j]);

        memset(sTaxiNodesMask,0,sizeof(sTaxiNodesMask));
        memset(sOldContinentsNodesMask,0,sizeof(sTaxiNodesMask));
        for(uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);
            if(!node)
                continue;

            TaxiPathSetBySource::const_iterator src_i = sTaxiPathSetBySource.find(i);
            if(src_i!=sTaxiPathSetBySource.end() && !src_i->second.empty())
            {
                bool ok = false;
                for(TaxiPathSetForSource::const_iterator dest_i = src_i->second.begin();dest_i != src_i->second.end(); ++dest_i)
                {
                    // not spell path
                    if(spellPaths.find(dest_i->second.ID)==spellPaths.end())
                    {
                        ok = true;
                        break;
                    }
                }

                if(!ok)
                    continue;
            }

            // valid taxi network node
            uint8  field   = (uint8)((i - 1) / 32);
            uint32 submask = 1<<((i-1)%32);
            sTaxiNodesMask[field] |= submask;

            // old continent node (+ nodes virtually at old continents, check explicitly to avoid loading map files for zone info)
            if (node->map_id < 2 || i == 82 || i == 83 || i == 93 || i == 94)
                sOldContinentsNodesMask[field] |= submask;
        }
    }

    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sTotemCategoryStore,       dbcPath,"TotemCategory.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sVehicleStore,             dbcPath,"Vehicle.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sVehicleSeatStore,         dbcPath,"VehicleSeat.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sWorldMapAreaStore,        dbcPath,"WorldMapArea.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sWorldMapOverlayStore,     dbcPath,"WorldMapOverlay.dbc");
    LoadDBC(availableDbcLocales,bar,bad_dbc_files,sWorldSafeLocsStore,       dbcPath,"WorldSafeLocs.dbc");

    // error checks
    if(bad_dbc_files.size() >= DBCFilesCount )
    {
        sLog.outError("\nIncorrect DataDir value in CWd.conf or ALL required *.dbc files (%d) not found by path: %sdbc",DBCFilesCount,dataPath.c_str());
        exit(1);
    }
    else if(!bad_dbc_files.empty() )
    {
        std::string str;
        for(std::list<std::string>::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        sLog.outError("\nSome required *.dbc files (%u from %d) not found or not compatible:\n%s",(uint32)bad_dbc_files.size(),DBCFilesCount,str.c_str());
        exit(1);
    }

    // Check loaded DBC files proper version
    if( !sSpellStore.LookupEntry(66530)            ||       // last added spell in 3.1.3
        !sMapStore.LookupEntry(624)                ||       // last map added in 3.1.3
        !sGemPropertiesStore.LookupEntry(1609)     ||       // last gem property added in 3.1.3
        !sItemExtendedCostStore.LookupEntry(2671)  ||       // last item extended cost added in 3.1.3
        !sCharTitlesStore.LookupEntry(166)         ||       // last char title added in 3.1.3
        !sAreaStore.LookupEntry(2905)              ||       // last area (areaflag) added in 3.1.3
        !sItemStore.LookupEntry(46894)             )        // last client known item added in 3.1.3
    {
        sLog.outError("\nYou have _outdated_ DBC files. Please extract correct versions from current using client.");
        exit(1);
    }

    sLog.outString();
    sLog.outString( ">> Initialized %d data stores", DBCFilesCount );
}

SimpleFactionsList const* GetFactionTeamList(uint32 faction, bool &isTeamMember)
{
    for(FactionTeamMap::const_iterator itr = sFactionTeamMap.begin(); itr != sFactionTeamMap.end(); ++itr)
    {
        if(itr->first == faction)
        {
            isTeamMember = false;
            return &itr->second;
        }
        for(SimpleFactionsList::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            if((*itr2) == faction)
            {
                isTeamMember = true;
                return &itr->second;
            }
        }
    }
    return NULL;
}

char* GetPetName(uint32 petfamily, uint32 dbclang)
{
    if(!petfamily)
        return NULL;
    CreatureFamilyEntry const *pet_family = sCreatureFamilyStore.LookupEntry(petfamily);
    if(!pet_family)
        return NULL;
    return pet_family->Name[dbclang]?pet_family->Name[dbclang]:NULL;
}

TalentSpellPos const* GetTalentSpellPos(uint32 spellId)
{
    TalentSpellPosMap::const_iterator itr = sTalentSpellPosMap.find(spellId);
    if(itr==sTalentSpellPosMap.end())
        return NULL;

    return &itr->second;
}

uint32 GetTalentSpellCost(uint32 spellId)
{
    if(TalentSpellPos const* pos = GetTalentSpellPos(spellId))
        return pos->rank+1;

    return 0;
}

int32 GetAreaFlagByAreaID(uint32 area_id)
{
    AreaFlagByAreaID::iterator i = sAreaFlagByAreaID.find(area_id);
    if(i == sAreaFlagByAreaID.end())
        return -1;

    return i->second;
}

AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id)
{
    int32 areaflag = GetAreaFlagByAreaID(area_id);
    if(areaflag < 0)
        return NULL;

    return sAreaStore.LookupEntry(areaflag );
}

AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag,uint32 map_id)
{
    if(area_flag)
        return sAreaStore.LookupEntry(area_flag);

    if(MapEntry const* mapEntry = sMapStore.LookupEntry(map_id))
        return GetAreaEntryByAreaID(mapEntry->linked_zone);

    return NULL;
}

uint32 GetAreaFlagByMapId(uint32 mapid)
{
    AreaFlagByMapID::iterator i = sAreaFlagByMapID.find(mapid);
    if(i == sAreaFlagByMapID.end())
        return 0;
    else
        return i->second;
}

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId)
{
    if(mapid != 530 && mapid != 571)                        // speed for most cases
        return mapid;

    if(WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->virtual_map_id >= 0 ? wma->virtual_map_id : wma->map_id;

    return mapid;
}

ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId)
{
    mapid = GetVirtualMapForMapAndZone(mapid,zoneId);
    if(mapid < 2)
        return CONTENT_1_60;

    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    if(!mapEntry)
        return CONTENT_1_60;

    switch(mapEntry->Expansion())
    {
        default: return CONTENT_1_60;
        case 1:  return CONTENT_61_70;
        case 2:  return CONTENT_71_80;
    }
}

ChatChannelsEntry const* GetChannelEntryFor(uint32 channel_id)
{
    // not sorted, numbering index from 0
    for(uint32 i = 0; i < sChatChannelsStore.GetNumRows(); ++i)
    {
        ChatChannelsEntry const* ch = sChatChannelsStore.LookupEntry(i);
        if(ch && ch->ChannelID == channel_id)
            return ch;
    }
    return NULL;
}

bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId)
{
    if(requiredTotemCategoryId==0)
        return true;
    if(itemTotemCategoryId==0)
        return false;

    TotemCategoryEntry const* itemEntry = sTotemCategoryStore.LookupEntry(itemTotemCategoryId);
    if(!itemEntry)
        return false;
    TotemCategoryEntry const* reqEntry = sTotemCategoryStore.LookupEntry(requiredTotemCategoryId);
    if(!reqEntry)
        return false;

    if(itemEntry->categoryType!=reqEntry->categoryType)
        return false;

    return (itemEntry->categoryMask & reqEntry->categoryMask)==reqEntry->categoryMask;
}

void Zone2MapCoordinates(float& x,float& y,uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if(!maEntry)
        return;

    std::swap(x,y);                                         // at client map coords swapped
    x = x*((maEntry->x2-maEntry->x1)/100)+maEntry->x1;
    y = y*((maEntry->y2-maEntry->y1)/100)+maEntry->y1;      // client y coord from top to down
}

void Map2ZoneCoordinates(float& x,float& y,uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // if not listed then map coordinates (instance)
    if(!maEntry)
        return;

    x = (x-maEntry->x1)/((maEntry->x2-maEntry->x1)/100);
    y = (y-maEntry->y1)/((maEntry->y2-maEntry->y1)/100);    // client y coord from top to down
    std::swap(x,y);                                         // client have map coords swapped
}

uint32 const* GetTalentTabPages(uint32 cls)
{
    return sTalentTabPages[cls];
}

// script support functions
CW_DLL_SPEC DBCStorage <SoundEntriesEntry>  const* GetSoundEntriesStore()   { return &sSoundEntriesStore;   }
CW_DLL_SPEC DBCStorage <SpellEntry>         const* GetSpellStore()          { return &sSpellStore;          }
CW_DLL_SPEC DBCStorage <SpellRangeEntry>    const* GetSpellRangeStore()     { return &sSpellRangeStore;     }
CW_DLL_SPEC DBCStorage <FactionEntry>       const* GetFactionStore()        { return &sFactionStore;        }
CW_DLL_SPEC DBCStorage <ItemEntry>          const* GetItemDisplayStore()    { return &sItemStore;           }
CW_DLL_SPEC DBCStorage <CreatureDisplayInfoEntry> const* GetCreatureDisplayStore() { return &sCreatureDisplayInfoStore; }
CW_DLL_SPEC DBCStorage <EmotesEntry>        const* GetEmotesStore()         { return &sEmotesStore;         }
CW_DLL_SPEC DBCStorage <EmotesTextEntry>    const* GetEmotesTextStore()     { return &sEmotesTextStore;     }
CW_DLL_SPEC DBCStorage <AchievementEntry>   const* GetAchievementStore()    { return &sAchievementStore;    }
