#include "common/pch.h"
#include "skin_lr2_converters.h"

#include "common/entry/entry_song.h"
#include "game/chart/chart.h"
#include "game/ruleset/ruleset.h"
#include "game/ruleset/ruleset_bms.h"
#include "game/data/data_types.h"
#include "game/arena/arena_data.h"
#include "game/runtime/i18n.h"

namespace lunaticvibes
{

using namespace data;

class TextConverter
{
private:
    static std::shared_ptr<EntryBase> getCurrentSelectedEntry()
    {
        if (SelectData.entries.empty())
            return nullptr;
        if (SelectData.entries[SelectData.selectedEntryIndex].first->type() != eEntryType::CHART)
            return nullptr;
        return SelectData.entries[SelectData.selectedEntryIndex].first;
    }

    static std::shared_ptr<ChartFormatBase> getCurrentSelectedChart()
    {
        auto p = getCurrentSelectedEntry();
        if (p == nullptr)
            return nullptr;
        switch (p->type())
        {
        case eEntryType::SONG:
        case eEntryType::RIVAL_SONG:
            return std::dynamic_pointer_cast<EntryFolderSong>(p)->getCurrentChart();
        case eEntryType::CHART:
        case eEntryType::RIVAL_CHART:
            return std::dynamic_pointer_cast<EntryChart>(p)->getChart();
        }
        return nullptr;
    }

    static std::string_view playModRandom(PlayModifierRandomType t)
    {
        switch (t)
        {
        case PlayModifierRandomType::NONE: return "NORMAL";
        case PlayModifierRandomType::MIRROR: return "MIRROR";
        case PlayModifierRandomType::RANDOM: return "RANDOM";
        case PlayModifierRandomType::SRAN: return "S-RANDOM";
        case PlayModifierRandomType::HRAN: return "H-RANDOM";
        case PlayModifierRandomType::ALLSCR: return "ALL-SCR";
        case PlayModifierRandomType::RRAN: return "R-RANDOM";
        case PlayModifierRandomType::DB_SYNCHRONIZE: return "SYNCHRONIZE";
        case PlayModifierRandomType::DB_SYMMETRY: return "SYMMETRY";
        }
        return "";
    }

    static std::string_view playModGauge(PlayModifierGaugeType t)
    {
        switch (t)
        {
        case PlayModifierGaugeType::NORMAL: return "NORMAL";
        case PlayModifierGaugeType::HARD: return "HARD";
        case PlayModifierGaugeType::DEATH: return "DEATH";
        case PlayModifierGaugeType::EASY: return "EASY";
        case PlayModifierGaugeType::EXHARD: return "EX-HARD";
        case PlayModifierGaugeType::ASSISTEASY: return "ASSIST-EASY";
        }
        return "";
    }

    static std::string_view playModAssist(uint8_t m)
    {
        switch (m)
        {
        case 0: return "NONE";
        case PLAY_MOD_ASSIST_AUTOSCR: return "AUTO-SCR";
        }
        return "";
    }

    static std::string_view playModLaneEffect(PlayModifierLaneEffectType t)
    {
        switch (t)
        {
            case PlayModifierLaneEffectType::OFF: return "OFF";
            case PlayModifierLaneEffectType::HIDDEN: return "HIDDEN+";
            case PlayModifierLaneEffectType::SUDDEN: return "SUDDEN+";
            case PlayModifierLaneEffectType::SUDHID: return "SUD+&HID+";
            case PlayModifierLaneEffectType::LIFT: return "LIFT";
            case PlayModifierLaneEffectType::LIFTSUD: return "LIFT&SUD+";
        }
        return "";
    }

    static std::shared_ptr<Struct_LR2Customize::Option> getCustomizeOption(int i)
    {
        const auto& d = LR2CustomizeData;
        if (i >= d.optionsKeyList.size())
            return nullptr;

        auto& key = d.optionsKeyList[d.topOptionIndex];
        return d.optionsMap.at(key);
    }
    static std::string_view getCustomizeOptionName(int i)
    {
        auto p = getCustomizeOption(LR2CustomizeData.topOptionIndex + i);
        return p ? p->displayName : "";
    }
    static std::string_view getCustomizeOptionEntry(int i)
    {
        auto p = getCustomizeOption(LR2CustomizeData.topOptionIndex + i);
        return p ? p->entries[p->selectedEntry] : "";
    }

    static std::string_view getCourseStageTitle(int i)
    {
        if (!PlayData.isCourse) return "";
        if (PlayData.courseStageData.size() <= i) return "";
        return PlayData.courseStageData[i].title;
    }
    static std::string_view getCourseStageSubTitle(int i)
    {
        if (!PlayData.isCourse) return "";
        if (PlayData.courseStageData.size() <= i) return "";
        return PlayData.courseStageData[i].subTitle;
    }

private:

    static std::string targetName;
    static std::string fullTitle;
    static std::string levelEstimated;

public:
    static std::string_view text_0() { return ""; }

    static std::string_view text_1()
    {
        if (PlayData.isBattle)
        {
            return "Player";
        }
        else
        {
            switch (PlayData.targetType)
            {
            case TargetType::Zero: return "RATE 0%";
            case TargetType::RankAAA: return "RANK AAA";
            case TargetType::RankAA: return "RANK AA";
            case TargetType::RankA: return "RANK A";
            case TargetType::UseTargetRate:
            {
                switch (PlayData.targetRate)
                {
                case 22:  return "RANK E";
                case 33:  return "RANK D";
                case 44:  return "RANK C";
                case 55:  return "RANK B";
                case 66:  return "RANK A";
                case 77:  return "RANK AA";
                case 88:  return "RANK AAA";
                case 100: return "DJ AUTO";
                }
                targetName = "RATE "s + std::to_string(PlayData.targetRate) + "%";
                return targetName;
            }
            }
        }
        return "";
    }
    static std::string_view text_2() { return SystemData.playerName; };

    std::string_view text_10()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            return chart ? chart->title : p->_name;
        }
        return "";
    }
    std::string_view text_11()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            return chart ? chart->title2 : p->_name2;
        }
        return "";
    }
    std::string_view text_12()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            auto title = chart ? chart->title : p->_name;
            auto title2 = chart ? chart->title : p->_name;
            fullTitle = title + (!title.empty() ? " " : "") + title2;
            return fullTitle;
        }
        return "";
    }
    std::string_view text_13()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            return chart ? chart->genre : "";
        }
        return "";
    }
    std::string_view text_14()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            return chart ? chart->artist : "";
        }
        return "";
    }
    std::string_view text_15()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            return chart ? chart->artist2 : "";
        }
        return "";
    }
    // 16 TAG
    std::string_view text_17()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            levelEstimated = std::to_string(chart ? (int)chart->levelEstimated : 0);
            return levelEstimated;
        }
        return "";
    }
    std::string_view text_18()
    {
        auto p = getCurrentSelectedEntry();
        if (p)
        {
            auto chart = getCurrentSelectedChart();
            return chart ? chart->version : "";
        }
        return "";
    }

    /*
    EDIT_TITLE = 20,
    EDIT_SUBTITLE,
    EDIT_FULLTITLE,
    EDIT_GENRE,
    EDIT_ARTIST,
    EDIT_SUBARTIST,
    EDIT_TAG,
    EDIT_PLAYLEVEL,
    EDIT_DIFFICULTY,
    EDIT_INSANE_LEVEL,
    EDIT_JUKEBOX_NAME,
    */

    static std::string_view text_40() { return KeyConfigData.bindName[KeyConfigData.selecting.first]; };
    static std::string_view text_41() { return "-"; };
    static std::string_view text_42() { return "-"; };
    static std::string_view text_43() { return "-"; };
    static std::string_view text_44() { return "-"; };
    static std::string_view text_45() { return "-"; };
    static std::string_view text_46() { return "-"; };
    static std::string_view text_47() { return "-"; };
    static std::string_view text_48() { return "-"; };
    static std::string_view text_49() { return "-"; };

    static std::string_view text_50() { return LR2CustomizeData.currentSkinName; }
    static std::string_view text_51() { return LR2CustomizeData.currentSkinMakerName; }

    static std::string_view text_60()
    {
        switch (SelectData.filterKeys)
        {
        case FilterKeysType::All: return "ALL";
        case FilterKeysType::Single: return "SINGLE";
        case FilterKeysType::_7: return "7K";
        case FilterKeysType::_5: return "5K";
        case FilterKeysType::Double: return "DOUBLE";
        case FilterKeysType::_14: return "14K";
        case FilterKeysType::_10: return "10K";
        case FilterKeysType::_9: return "9K";
        }
        return "";
    }

    static std::string_view text_61()
    {
        switch (SelectData.sortType)
        {
        case SongListSortType::DEFAULT: return "DIRECTORY";
        case SongListSortType::TITLE: return "TITLE";
        case SongListSortType::LEVEL: return "LEVEL";
        case SongListSortType::CLEAR: return "CLEAR";
        case SongListSortType::RATE: return "RANK";
        }
        return "";
    }

    static std::string_view text_62()
    {
        switch (SelectData.filterDifficulty)
        {
        case FilterDifficultyType::All: return "ALL";
        case FilterDifficultyType::B: return "BEGINNER";
        case FilterDifficultyType::N: return "NORMAL";
        case FilterDifficultyType::H: return "HYPER";
        case FilterDifficultyType::A: return "ANOTHER";
        case FilterDifficultyType::I: return "INSANE";
        }
        return "";
    }

    static std::string_view text_63()
    {
        return playModRandom(PlayData.player[PLAYER_SLOT_PLAYER].mods.randomLeft);
    }
    static std::string_view text_64()
    {
        PlayModifierRandomType ran = PlayModifierRandomType::NONE;
        if (PlayData.isBattle)
        {
            ran = PlayData.player[PLAYER_SLOT_PLAYER].mods.randomRight;
        }
        else
        {
            ran = PlayData.player[PLAYER_SLOT_TARGET].mods.randomLeft;
        }
        return playModRandom(ran);
    }

    static std::string_view text_65()
    {
        return playModGauge(PlayData.player[PLAYER_SLOT_PLAYER].mods.gauge);
    }
    static std::string_view text_66()
    {
        return playModGauge(PlayData.player[PLAYER_SLOT_TARGET].mods.gauge);
    }

    static std::string_view text_67()
    {
        return playModAssist(PlayData.player[PLAYER_SLOT_PLAYER].mods.assist_mask);
    }
    static std::string_view text_68()
    {
        return playModAssist(PlayData.player[PLAYER_SLOT_TARGET].mods.assist_mask);
    }

    static std::string_view text_69()
    {
        if (!PlayData.isBattle)
            return "OFF";

        switch (PlayData.battleType)
        {
        case PlayModifierBattleType::Off: return "OFF";
        case PlayModifierBattleType::LocalBattle: return "BATTLE";
        case PlayModifierBattleType::DoubleBattle: return "DOUBLE";
        case PlayModifierBattleType::GhostBattle: return "GHOST";
        }
        return "";
    }

    static std::string_view text_70()
    {
        if (PlayData.mode != SkinType::PLAY10 && PlayData.mode != SkinType::PLAY14)
            return "OFF";

        return PlayData.player[PLAYER_SLOT_PLAYER].mods.DPFlip ? "ON" : "OFF";
    }

    static std::string_view text_71()
    {
        switch (PlayData.panelStyle & PANEL_STYLE_GRAPH_MASK)
        {
        case PANEL_STYLE_GRAPH_OFF: return "OFF";
        default: return "ON";
        }
    }

    static std::string_view text_72()
    {
        switch (PlayData.ghostType)
        {
        case GhostScorePosition::Off: return "OFF";
        case GhostScorePosition::AboveJudge: return "TYPE A";
        case GhostScorePosition::NearJudge: return "TYPE B";
        case GhostScorePosition::NearJudgeLower: return "TYPE C";
        }
        return "";
    }


    static std::string_view text_73()
    {
        auto t = PlayData.player[PLAYER_SLOT_PLAYER].mods.laneEffect;
        return (t == PlayModifierLaneEffectType::OFF || t == PlayModifierLaneEffectType::LIFT) ? "OFF" : "ON";
    }

    static std::string_view text_74()
    {
        switch (PlayData.player[PLAYER_SLOT_PLAYER].mods.hispeedFix)
        {
        case PlayModifierHispeedFixType::NONE: return "OFF";
        case PlayModifierHispeedFixType::MINBPM: return "MIN FIX";
        case PlayModifierHispeedFixType::MAXBPM: return "MAX FIX";
        case PlayModifierHispeedFixType::AVERAGE: return "AVERAGE";
        case PlayModifierHispeedFixType::CONSTANT: return "CONSTANT";
        case PlayModifierHispeedFixType::INITIAL: return "START FIX";
        case PlayModifierHispeedFixType::MAIN: return "MAIN FIX";
        }
        return "";
    }

    static std::string_view text_75()
    {
        switch (PlayData.panelStyle & PANEL_STYLE_BGA_MASK)
        {
        case PANEL_STYLE_BGA_OFF:
        case PANEL_STYLE_BGA_SIDE: return "NORMAL";
        case PANEL_STYLE_BGA_SIDE_EXPAND: 
        case PANEL_STYLE_BGA_FULLSCREEN: return "EXTEND";
        }
        return "";
    }

    static std::string_view text_76()
    {
        return (PlayData.panelStyle & PANEL_STYLE_BGA_MASK) == PANEL_STYLE_BGA_OFF ? "OFF" : "ON";
    }

    static std::string_view text_77()
    {
        return "32 BITS";
    }

    static std::string_view text_78()
    {
        switch (SystemData.vsyncMode)
        {
        case GameVsyncMode::OFF: return "OFF";
        case GameVsyncMode::ON: return "ON";
        case GameVsyncMode::ADAPTIVE: return "ADAPTIVE";
        }
        return "";
    }

    static std::string_view text_79()
    {
        switch (SystemData.windowMode)
        {
        case GameWindowMode::FULLSCREEN: return "FULL";
        case GameWindowMode::BORDERLESS: return "BORDERLESS";
        case GameWindowMode::WINDOWED: return "WINDOW";
        }
    }

    static std::string_view text_80()
    {
        // judge auto adjust
        return "NOT SUPPORTED";
    }

    static std::string_view text_81()
    {
        return "EXSCORE";
    }

    static std::string_view text_82()
    {
        return PROJECT_NAME " " PROJECT_VERSION;
    }

    static std::string_view text_83()
    {
        return "Thank you for playing!";
    }

    static std::string_view text_84()
    {
        return playModLaneEffect(PlayData.player[PLAYER_SLOT_PLAYER].mods.laneEffect);
    }
    static std::string_view text_85()
    {
        return playModLaneEffect(PlayData.player[PLAYER_SLOT_TARGET].mods.laneEffect);
    }

    static std::string_view text_100() { return getCustomizeOptionName(0); }
    static std::string_view text_101() { return getCustomizeOptionName(1); }
    static std::string_view text_102() { return getCustomizeOptionName(2); }
    static std::string_view text_103() { return getCustomizeOptionName(3); }
    static std::string_view text_104() { return getCustomizeOptionName(4); }
    static std::string_view text_105() { return getCustomizeOptionName(5); }
    static std::string_view text_106() { return getCustomizeOptionName(6); }
    static std::string_view text_107() { return getCustomizeOptionName(7); }
    static std::string_view text_108() { return getCustomizeOptionName(8); }
    static std::string_view text_109() { return getCustomizeOptionName(9); }

    static std::string_view text_110() { return getCustomizeOptionEntry(0); }
    static std::string_view text_111() { return getCustomizeOptionEntry(1); }
    static std::string_view text_112() { return getCustomizeOptionEntry(2); }
    static std::string_view text_113() { return getCustomizeOptionEntry(3); }
    static std::string_view text_114() { return getCustomizeOptionEntry(4); }
    static std::string_view text_115() { return getCustomizeOptionEntry(5); }
    static std::string_view text_116() { return getCustomizeOptionEntry(6); }
    static std::string_view text_117() { return getCustomizeOptionEntry(7); }
    static std::string_view text_118() { return getCustomizeOptionEntry(8); }
    static std::string_view text_119() { return getCustomizeOptionEntry(9); }

    static std::string_view text_150() { return getCourseStageTitle(0); }
    static std::string_view text_151() { return getCourseStageTitle(1); }
    static std::string_view text_152() { return getCourseStageTitle(2); }
    static std::string_view text_153() { return getCourseStageTitle(3); }
    static std::string_view text_154() { return getCourseStageTitle(4); }
    static std::string_view text_155() { return getCourseStageTitle(5); }
    static std::string_view text_156() { return getCourseStageTitle(6); }
    static std::string_view text_157() { return getCourseStageTitle(7); }
    static std::string_view text_158() { return getCourseStageTitle(8); }
    static std::string_view text_159() { return getCourseStageTitle(9); }

    static std::string_view text_160() { return getCourseStageSubTitle(0); }
    static std::string_view text_161() { return getCourseStageSubTitle(1); }
    static std::string_view text_162() { return getCourseStageSubTitle(2); }
    static std::string_view text_163() { return getCourseStageSubTitle(3); }
    static std::string_view text_164() { return getCourseStageSubTitle(4); }
    static std::string_view text_165() { return getCourseStageSubTitle(5); }
    static std::string_view text_166() { return getCourseStageSubTitle(6); }
    static std::string_view text_167() { return getCourseStageSubTitle(7); }
    static std::string_view text_168() { return getCourseStageSubTitle(8); }
    static std::string_view text_169() { return getCourseStageSubTitle(9); }

    static std::string_view text_260() { return gArenaData.isOnline() ? "ARENA LOBBY" : ""; };
    static std::string_view text_261() { return gArenaData.getPlayerName(0); };
    static std::string_view text_262() { return gArenaData.getPlayerName(1); };
    static std::string_view text_263() { return gArenaData.getPlayerName(2); };
    static std::string_view text_264() { return gArenaData.getPlayerName(3); };
    static std::string_view text_265() { return gArenaData.getPlayerName(4); };
    static std::string_view text_266() { return gArenaData.getPlayerName(5); };
    static std::string_view text_267() { return gArenaData.getPlayerName(6); };
    static std::string_view text_268() { return gArenaData.getPlayerName(7); };
    static std::string_view text_270() { auto r = std::dynamic_pointer_cast<RulesetBMS>(PlayData.player[PLAYER_SLOT_PLAYER].ruleset); return r ? r->getModifierText() : ""; };
    static std::string_view text_271() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(0)); return r ? r->getModifierText() : ""; };
    static std::string_view text_272() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(1)); return r ? r->getModifierText() : ""; };
    static std::string_view text_273() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(2)); return r ? r->getModifierText() : ""; };
    static std::string_view text_274() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(3)); return r ? r->getModifierText() : ""; };
    static std::string_view text_275() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(4)); return r ? r->getModifierText() : ""; };
    static std::string_view text_276() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(5)); return r ? r->getModifierText() : ""; };
    static std::string_view text_277() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(6)); return r ? r->getModifierText() : ""; };
    static std::string_view text_278() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(7)); return r ? r->getModifierText() : ""; };
    static std::string_view text_280() { auto r = std::dynamic_pointer_cast<RulesetBMS>(PlayData.player[PLAYER_SLOT_PLAYER].ruleset); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_281() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(0)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_282() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(1)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_283() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(2)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_284() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(3)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_285() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(4)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_286() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(5)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_287() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(6)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_288() { auto r = std::dynamic_pointer_cast<RulesetBMS>(gArenaData.getPlayerRuleset(7)); return r ? r->getModifierTextShort() : ""; };
    static std::string_view text_300() { return i18n::c(i18nText::KEYCONFIG_HINT_KEY); };
    static std::string_view text_301() { return i18n::c(i18nText::KEYCONFIG_HINT_BIND); };
    static std::string_view text_302() { return i18n::c(i18nText::KEYCONFIG_HINT_DEADZONE); };
    static std::string_view text_303() { return i18n::c(i18nText::KEYCONFIG_HINT_F1); };
    static std::string_view text_304() { return i18n::c(i18nText::KEYCONFIG_HINT_F2); };
    static std::string_view text_305() { return i18n::c(i18nText::KEYCONFIG_HINT_DEL); };
    static std::string_view text_306() { return i18n::c(i18nText::KEYCONFIG_HINT_SCRATCH_ABS); };

    static std::string_view text_311() { return "1P 1"; };
    static std::string_view text_312() { return "1P 2"; };
    static std::string_view text_313() { return "1P 3"; };
    static std::string_view text_314() { return "1P 4"; };
    static std::string_view text_315() { return "1P 5"; };
    static std::string_view text_316() { return "1P 6"; };
    static std::string_view text_317() { return "1P 7"; };
    static std::string_view text_318() { return "1P 8"; };
    static std::string_view text_319() { return "1P 9"; };
    static std::string_view text_320() { return "1P SC-L"; };
    static std::string_view text_321() { return "1P SC-R"; };
    static std::string_view text_322() { return "1P START"; };
    static std::string_view text_323() { return "1P SELECT"; };
    static std::string_view text_324() { return "1P SC ABS"; };
    static std::string_view text_331() { return "2P 1"; };
    static std::string_view text_332() { return "2P 2"; };
    static std::string_view text_333() { return "2P 3"; };
    static std::string_view text_334() { return "2P 4"; };
    static std::string_view text_335() { return "2P 5"; };
    static std::string_view text_336() { return "2P 6"; };
    static std::string_view text_337() { return "2P 7"; };
    static std::string_view text_338() { return "2P 8"; };
    static std::string_view text_339() { return "2P 9"; };
    static std::string_view text_340() { return "2P SC-L"; };
    static std::string_view text_341() { return "2P SC-R"; };
    static std::string_view text_342() { return "2P START"; };
    static std::string_view text_343() { return "2P SELECT"; };
    static std::string_view text_344() { return "2P SC ABS"; };

    static std::string_view text_351()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K11];
        }
        return "";
    }
    static std::string_view text_352()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K12];
        }
        return "";
    }
    static std::string_view text_353()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K13];
        }
        return "";
    }
    static std::string_view text_354()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K14];
        }
        return "";
    }
    static std::string_view text_355()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K15];
        }
        return "";
    }
    static std::string_view text_356()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K16];
        }
        return "";
    }
    static std::string_view text_357()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K17];
        }
        return "";
    }
    static std::string_view text_358()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K18];
        }
        return "";
    }
    static std::string_view text_359()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K19];
        }
        return "";
    }
    static std::string_view text_362()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K1START];
        }
        return "";
    }
    static std::string_view text_363()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K1SELECT];
        }
        return "";
    }
    static std::string_view text_360()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::S1L];
        }
        return "";
    }
    static std::string_view text_361()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::S1R];
        }
        return "";
    }
    static std::string_view text_364()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.scratchAxisValueText[0];
        }
        return "";
    }
    static std::string_view text_371()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K21];
        }
        return "";
    }
    static std::string_view text_372()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K22];
        }
        return "";
    }
    static std::string_view text_373()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K23];
        }
        return "";
    }
    static std::string_view text_374()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K24];
        }
        return "";
    }
    static std::string_view text_375()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K25];
        }
        return "";
    }
    static std::string_view text_376()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K26];
        }
        return "";
    }
    static std::string_view text_377()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K27];
        }
        return "";
    }
    static std::string_view text_378()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K28];
        }
        return "";
    }
    static std::string_view text_379()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K29];
        }
        return "";
    }
    static std::string_view text_382()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K2START];
        }
        return "";
    }
    static std::string_view text_383()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::K2SELECT];
        }
        return "";
    }
    static std::string_view text_380()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::S2L];
        }
        return "";
    }
    static std::string_view text_381()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.bindName[k::S2R];
        }
        return "";
    }
    static std::string_view text_384()
    {
        if (SystemData.gNextScene == SceneType::KEYCONFIG)
        {
            using k = Input::Pad;
            return KeyConfigData.scratchAxisValueText[1];
        }
        return "";
    }
};


#define define_has_member(index)                                                            \
class has_text_##index                                                                      \
{                                                                                           \
private:                                                                                    \
    typedef long yes_type;                                                                  \
    typedef char no_type;                                                                   \
    template <typename U> static yes_type test(decltype(&U::text_##index));                 \
    template <typename U> static no_type  test(...);                                        \
public:                                                                                     \
    static constexpr bool has_func = sizeof(test<TextConverter>()) == sizeof(yes_type);     \
private:                                                                                    \
    template <typename U, typename = std::enable_if_t<!has_func>>                           \
    static constexpr std::string_view(*func())() { return &U::text_0; }                     \
    template <typename U, typename = std::enable_if_t<has_func>>                            \
    static constexpr std::string_view(*func())() { return &U::text_##index; }               \
public:                                                                                     \
    static constexpr std::string_view(*value)() = func<TextConverter>();                    \
}

#define has_text(index)  has_text_##index::has_func
#define text(index) has_text_##index::value

#pragma region define_has_member

define_has_member(0);
define_has_member(1);
define_has_member(2);
define_has_member(3);
define_has_member(4);
define_has_member(5);
define_has_member(6);
define_has_member(7);
define_has_member(8);
define_has_member(9);
define_has_member(10);
define_has_member(11);
define_has_member(12);
define_has_member(13);
define_has_member(14);
define_has_member(15);
define_has_member(16);
define_has_member(17);
define_has_member(18);
define_has_member(19);
define_has_member(20);
define_has_member(21);
define_has_member(22);
define_has_member(23);
define_has_member(24);
define_has_member(25);
define_has_member(26);
define_has_member(27);
define_has_member(28);
define_has_member(29);
define_has_member(30);
define_has_member(31);
define_has_member(32);
define_has_member(33);
define_has_member(34);
define_has_member(35);
define_has_member(36);
define_has_member(37);
define_has_member(38);
define_has_member(39);
define_has_member(40);
define_has_member(41);
define_has_member(42);
define_has_member(43);
define_has_member(44);
define_has_member(45);
define_has_member(46);
define_has_member(47);
define_has_member(48);
define_has_member(49);
define_has_member(50);
define_has_member(51);
define_has_member(52);
define_has_member(53);
define_has_member(54);
define_has_member(55);
define_has_member(56);
define_has_member(57);
define_has_member(58);
define_has_member(59);
define_has_member(60);
define_has_member(61);
define_has_member(62);
define_has_member(63);
define_has_member(64);
define_has_member(65);
define_has_member(66);
define_has_member(67);
define_has_member(68);
define_has_member(69);
define_has_member(70);
define_has_member(71);
define_has_member(72);
define_has_member(73);
define_has_member(74);
define_has_member(75);
define_has_member(76);
define_has_member(77);
define_has_member(78);
define_has_member(79);
define_has_member(80);
define_has_member(81);
define_has_member(82);
define_has_member(83);
define_has_member(84);
define_has_member(85);
define_has_member(86);
define_has_member(87);
define_has_member(88);
define_has_member(89);
define_has_member(90);
define_has_member(91);
define_has_member(92);
define_has_member(93);
define_has_member(94);
define_has_member(95);
define_has_member(96);
define_has_member(97);
define_has_member(98);
define_has_member(99);
define_has_member(100);
define_has_member(101);
define_has_member(102);
define_has_member(103);
define_has_member(104);
define_has_member(105);
define_has_member(106);
define_has_member(107);
define_has_member(108);
define_has_member(109);
define_has_member(110);
define_has_member(111);
define_has_member(112);
define_has_member(113);
define_has_member(114);
define_has_member(115);
define_has_member(116);
define_has_member(117);
define_has_member(118);
define_has_member(119);
define_has_member(120);
define_has_member(121);
define_has_member(122);
define_has_member(123);
define_has_member(124);
define_has_member(125);
define_has_member(126);
define_has_member(127);
define_has_member(128);
define_has_member(129);
define_has_member(130);
define_has_member(131);
define_has_member(132);
define_has_member(133);
define_has_member(134);
define_has_member(135);
define_has_member(136);
define_has_member(137);
define_has_member(138);
define_has_member(139);
define_has_member(140);
define_has_member(141);
define_has_member(142);
define_has_member(143);
define_has_member(144);
define_has_member(145);
define_has_member(146);
define_has_member(147);
define_has_member(148);
define_has_member(149);
define_has_member(150);
define_has_member(151);
define_has_member(152);
define_has_member(153);
define_has_member(154);
define_has_member(155);
define_has_member(156);
define_has_member(157);
define_has_member(158);
define_has_member(159);
define_has_member(160);
define_has_member(161);
define_has_member(162);
define_has_member(163);
define_has_member(164);
define_has_member(165);
define_has_member(166);
define_has_member(167);
define_has_member(168);
define_has_member(169);
define_has_member(170);
define_has_member(171);
define_has_member(172);
define_has_member(173);
define_has_member(174);
define_has_member(175);
define_has_member(176);
define_has_member(177);
define_has_member(178);
define_has_member(179);
define_has_member(180);
define_has_member(181);
define_has_member(182);
define_has_member(183);
define_has_member(184);
define_has_member(185);
define_has_member(186);
define_has_member(187);
define_has_member(188);
define_has_member(189);
define_has_member(190);
define_has_member(191);
define_has_member(192);
define_has_member(193);
define_has_member(194);
define_has_member(195);
define_has_member(196);
define_has_member(197);
define_has_member(198);
define_has_member(199);
define_has_member(200);
define_has_member(201);
define_has_member(202);
define_has_member(203);
define_has_member(204);
define_has_member(205);
define_has_member(206);
define_has_member(207);
define_has_member(208);
define_has_member(209);
define_has_member(210);
define_has_member(211);
define_has_member(212);
define_has_member(213);
define_has_member(214);
define_has_member(215);
define_has_member(216);
define_has_member(217);
define_has_member(218);
define_has_member(219);
define_has_member(220);
define_has_member(221);
define_has_member(222);
define_has_member(223);
define_has_member(224);
define_has_member(225);
define_has_member(226);
define_has_member(227);
define_has_member(228);
define_has_member(229);
define_has_member(230);
define_has_member(231);
define_has_member(232);
define_has_member(233);
define_has_member(234);
define_has_member(235);
define_has_member(236);
define_has_member(237);
define_has_member(238);
define_has_member(239);
define_has_member(240);
define_has_member(241);
define_has_member(242);
define_has_member(243);
define_has_member(244);
define_has_member(245);
define_has_member(246);
define_has_member(247);
define_has_member(248);
define_has_member(249);
define_has_member(250);
define_has_member(251);
define_has_member(252);
define_has_member(253);
define_has_member(254);
define_has_member(255);
define_has_member(256);
define_has_member(257);
define_has_member(258);
define_has_member(259);
define_has_member(260);
define_has_member(261);
define_has_member(262);
define_has_member(263);
define_has_member(264);
define_has_member(265);
define_has_member(266);
define_has_member(267);
define_has_member(268);
define_has_member(269);
define_has_member(270);
define_has_member(271);
define_has_member(272);
define_has_member(273);
define_has_member(274);
define_has_member(275);
define_has_member(276);
define_has_member(277);
define_has_member(278);
define_has_member(279);
define_has_member(280);
define_has_member(281);
define_has_member(282);
define_has_member(283);
define_has_member(284);
define_has_member(285);
define_has_member(286);
define_has_member(287);
define_has_member(288);
define_has_member(289);
define_has_member(290);
define_has_member(291);
define_has_member(292);
define_has_member(293);
define_has_member(294);
define_has_member(295);
define_has_member(296);
define_has_member(297);
define_has_member(298);
define_has_member(299);
define_has_member(300);
define_has_member(301);
define_has_member(302);
define_has_member(303);
define_has_member(304);
define_has_member(305);
define_has_member(306);
define_has_member(307);
define_has_member(308);
define_has_member(309);
define_has_member(310);
define_has_member(311);
define_has_member(312);
define_has_member(313);
define_has_member(314);
define_has_member(315);
define_has_member(316);
define_has_member(317);
define_has_member(318);
define_has_member(319);
define_has_member(320);
define_has_member(321);
define_has_member(322);
define_has_member(323);
define_has_member(324);
define_has_member(325);
define_has_member(326);
define_has_member(327);
define_has_member(328);
define_has_member(329);
define_has_member(330);
define_has_member(331);
define_has_member(332);
define_has_member(333);
define_has_member(334);
define_has_member(335);
define_has_member(336);
define_has_member(337);
define_has_member(338);
define_has_member(339);
define_has_member(340);
define_has_member(341);
define_has_member(342);
define_has_member(343);
define_has_member(344);
define_has_member(345);
define_has_member(346);
define_has_member(347);
define_has_member(348);
define_has_member(349);
define_has_member(350);
define_has_member(351);
define_has_member(352);
define_has_member(353);
define_has_member(354);
define_has_member(355);
define_has_member(356);
define_has_member(357);
define_has_member(358);
define_has_member(359);
define_has_member(360);
define_has_member(361);
define_has_member(362);
define_has_member(363);
define_has_member(364);
define_has_member(365);
define_has_member(366);
define_has_member(367);
define_has_member(368);
define_has_member(369);
define_has_member(370);
define_has_member(371);
define_has_member(372);
define_has_member(373);
define_has_member(374);
define_has_member(375);
define_has_member(376);
define_has_member(377);
define_has_member(378);
define_has_member(379);
define_has_member(380);
define_has_member(381);
define_has_member(382);
define_has_member(383);
define_has_member(384);
define_has_member(385);
define_has_member(386);
define_has_member(387);
define_has_member(388);
define_has_member(389);
define_has_member(390);
define_has_member(391);
define_has_member(392);
define_has_member(393);
define_has_member(394);
define_has_member(395);
define_has_member(396);
define_has_member(397);
define_has_member(398);
define_has_member(399);

#pragma endregion

namespace lr2skin
{

std::function<std::string_view()> convertTextIndex(int n)
{
    if (n >= 0 && n <= 399)
    {
        static constexpr std::string_view(*kv[400])() =
        {
            text(0),
            text(1),
            text(2),
            text(3),
            text(4),
            text(5),
            text(6),
            text(7),
            text(8),
            text(9),
            text(10),
            text(11),
            text(12),
            text(13),
            text(14),
            text(15),
            text(16),
            text(17),
            text(18),
            text(19),
            text(20),
            text(21),
            text(22),
            text(23),
            text(24),
            text(25),
            text(26),
            text(27),
            text(28),
            text(29),
            text(30),
            text(31),
            text(32),
            text(33),
            text(34),
            text(35),
            text(36),
            text(37),
            text(38),
            text(39),
            text(40),
            text(41),
            text(42),
            text(43),
            text(44),
            text(45),
            text(46),
            text(47),
            text(48),
            text(49),
            text(50),
            text(51),
            text(52),
            text(53),
            text(54),
            text(55),
            text(56),
            text(57),
            text(58),
            text(59),
            text(60),
            text(61),
            text(62),
            text(63),
            text(64),
            text(65),
            text(66),
            text(67),
            text(68),
            text(69),
            text(70),
            text(71),
            text(72),
            text(73),
            text(74),
            text(75),
            text(76),
            text(77),
            text(78),
            text(79),
            text(80),
            text(81),
            text(82),
            text(83),
            text(84),
            text(85),
            text(86),
            text(87),
            text(88),
            text(89),
            text(90),
            text(91),
            text(92),
            text(93),
            text(94),
            text(95),
            text(96),
            text(97),
            text(98),
            text(99),
            text(100),
            text(101),
            text(102),
            text(103),
            text(104),
            text(105),
            text(106),
            text(107),
            text(108),
            text(109),
            text(110),
            text(111),
            text(112),
            text(113),
            text(114),
            text(115),
            text(116),
            text(117),
            text(118),
            text(119),
            text(120),
            text(121),
            text(122),
            text(123),
            text(124),
            text(125),
            text(126),
            text(127),
            text(128),
            text(129),
            text(130),
            text(131),
            text(132),
            text(133),
            text(134),
            text(135),
            text(136),
            text(137),
            text(138),
            text(139),
            text(140),
            text(141),
            text(142),
            text(143),
            text(144),
            text(145),
            text(146),
            text(147),
            text(148),
            text(149),
            text(150),
            text(151),
            text(152),
            text(153),
            text(154),
            text(155),
            text(156),
            text(157),
            text(158),
            text(159),
            text(160),
            text(161),
            text(162),
            text(163),
            text(164),
            text(165),
            text(166),
            text(167),
            text(168),
            text(169),
            text(170),
            text(171),
            text(172),
            text(173),
            text(174),
            text(175),
            text(176),
            text(177),
            text(178),
            text(179),
            text(180),
            text(181),
            text(182),
            text(183),
            text(184),
            text(185),
            text(186),
            text(187),
            text(188),
            text(189),
            text(190),
            text(191),
            text(192),
            text(193),
            text(194),
            text(195),
            text(196),
            text(197),
            text(198),
            text(199),
            text(200),
            text(201),
            text(202),
            text(203),
            text(204),
            text(205),
            text(206),
            text(207),
            text(208),
            text(209),
            text(210),
            text(211),
            text(212),
            text(213),
            text(214),
            text(215),
            text(216),
            text(217),
            text(218),
            text(219),
            text(220),
            text(221),
            text(222),
            text(223),
            text(224),
            text(225),
            text(226),
            text(227),
            text(228),
            text(229),
            text(230),
            text(231),
            text(232),
            text(233),
            text(234),
            text(235),
            text(236),
            text(237),
            text(238),
            text(239),
            text(240),
            text(241),
            text(242),
            text(243),
            text(244),
            text(245),
            text(246),
            text(247),
            text(248),
            text(249),
            text(250),
            text(251),
            text(252),
            text(253),
            text(254),
            text(255),
            text(256),
            text(257),
            text(258),
            text(259),
            text(260),
            text(261),
            text(262),
            text(263),
            text(264),
            text(265),
            text(266),
            text(267),
            text(268),
            text(269),
            text(270),
            text(271),
            text(272),
            text(273),
            text(274),
            text(275),
            text(276),
            text(277),
            text(278),
            text(279),
            text(280),
            text(281),
            text(282),
            text(283),
            text(284),
            text(285),
            text(286),
            text(287),
            text(288),
            text(289),
            text(290),
            text(291),
            text(292),
            text(293),
            text(294),
            text(295),
            text(296),
            text(297),
            text(298),
            text(299),
            text(300),
            text(301),
            text(302),
            text(303),
            text(304),
            text(305),
            text(306),
            text(307),
            text(308),
            text(309),
            text(310),
            text(311),
            text(312),
            text(313),
            text(314),
            text(315),
            text(316),
            text(317),
            text(318),
            text(319),
            text(320),
            text(321),
            text(322),
            text(323),
            text(324),
            text(325),
            text(326),
            text(327),
            text(328),
            text(329),
            text(330),
            text(331),
            text(332),
            text(333),
            text(334),
            text(335),
            text(336),
            text(337),
            text(338),
            text(339),
            text(340),
            text(341),
            text(342),
            text(343),
            text(344),
            text(345),
            text(346),
            text(347),
            text(348),
            text(349),
            text(350),
            text(351),
            text(352),
            text(353),
            text(354),
            text(355),
            text(356),
            text(357),
            text(358),
            text(359),
            text(360),
            text(361),
            text(362),
            text(363),
            text(364),
            text(365),
            text(366),
            text(367),
            text(368),
            text(369),
            text(370),
            text(371),
            text(372),
            text(373),
            text(374),
            text(375),
            text(376),
            text(377),
            text(378),
            text(379),
            text(380),
            text(381),
            text(382),
            text(383),
            text(384),
            text(385),
            text(386),
            text(387),
            text(388),
            text(389),
            text(390),
            text(391),
            text(392),
            text(393),
            text(394),
            text(395),
            text(396),
            text(397),
            text(398),
            text(399),
        };

        return kv[n];
    }

    return []() -> std::string_view { return ""; };
}

}
}