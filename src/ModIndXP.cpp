/*
 * Copyright (C) 2023+ Firelands Core Project , released under AGPL v3 license: https://github.com/FirelandsProject/firelands-cata/blob/master/LICENSE
 */

#include "ScriptMgr.h"
#include "WorldSession.h"
#include "AccountMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Object.h"
#include "DatabaseEnv.h"

using namespace Firelands::ChatCommands;

enum IndividualXPString
{
    IXP_HELLO = 100040,
    IXP_MODULE_DISABLED,
    IXP_RATES_DISABLED,
    IXP_COMMAND_VIEW,
    IXP_MAX_RATE,
    IXP_MIN_RATE,
    IXP_COMMAND_SET,
    IXP_COMMAND_DISABLED,
    IXP_COMMAND_ENABLED,
    IXP_COMMAND_DEFAULT
};

bool moduleEnabled;
uint32 currentRate;
uint32 maxRate;
uint32 defaultRate;

// Add IndividualXP Announcer
class IndividualXP_Announce : public PlayerScript
{
  public:
    IndividualXP_Announce() : PlayerScript("IndividualXP_Announcer") {}

    void OnLogin(Player* player, bool fistLogin) override
    {
        if (sConfigMgr->GetBoolDefault("IndividualXP.Announce", true))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(IXP_HELLO);
        }
    }
};

class IndividualXP : public PlayerScript
{
  public:
    IndividualXP() : PlayerScript("IndividualXP") {}

    void OnLogin(Player* player, bool fistLogin) override
    {
        QueryResult result = CharacterDatabase.Query(Firelands::StringFormat("SELECT `Rate` FROM `individual_xp` WHERE `CharacterGUID` = %u", player->GetGUID().GetCounter()).c_str());
        if (!result)
        {
            defaultRate = sConfigMgr->GetIntDefault("IndividualXP.DefaultRate", 1);
            maxRate = sConfigMgr->GetIntDefault("IndividualXP.MaxRate", 10);
        }
        else
        {
            Field* fields = result->Fetch();
            defaultRate = fields[0].GetInt32();
        }
    }

    void OnLogout(Player* player) override
    {
        if (currentRate != 0)
        {
            CharacterDatabase.Query(Firelands::StringFormat("REPLACE INTO `individual_xp` (`CharacterGUID`, `Rate`) VALUES ('%u', '%u');", player->GetGUID().GetCounter(), currentRate).c_str());
        }
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
    {
        if (moduleEnabled)
            amount *= currentRate;
    }
};

class IndividualXPCommandScript : public CommandScript
{
  public:
    IndividualXPCommandScript() : CommandScript("IndividualXPCommand") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        // clang-format off
        static std::vector<ChatCommand> IndividualXPCommandTable =
        {
            { "enable",      rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleEnableCommand,     "" },
            { "disable",     rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleDisableCommand,    "" },
            { "view",        rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleViewCommand,      "" },
            { "set",         rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleSetCommand,      "" },
            { "default",     rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleDefaultCommand,      "" },
            { "",            rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleXPCommand,      "" },
        };
        static std::vector<ChatCommand> IndividualXPBaseTable =
        {
            { "xp",     rbac::RBAC_PERM_SEC_PLAYER,    false,    nullptr,                   "", IndividualXPCommandTable },
        };
        // clang-format on
        return IndividualXPBaseTable;
    }

    static bool HandleXPCommand(ChatHandler* handler, char const* args)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!*args)
            return false;

        return true;
    }

    static bool HandleViewCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        if (me->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN))
        {
            handler->PSendSysMessage(IXP_RATES_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
        {
            me->GetSession()->SendAreaTriggerMessage(handler->GetFirelandsString(IXP_COMMAND_VIEW), currentRate);
        }
        return true;
    }

    // Set Command
    static bool HandleSetCommand(ChatHandler* handler, char const* args)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!*args)
            return false;

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        uint32 rate = (uint32)atol(args);
        if (rate > maxRate)
        {
            handler->PSendSysMessage(IXP_MAX_RATE, maxRate);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else if (rate == 0)
        {
            handler->PSendSysMessage(IXP_MIN_RATE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        currentRate = rate;
        me->GetSession()->SendAreaTriggerMessage(handler->GetFirelandsString(IXP_COMMAND_SET), rate);
        return true;
    }

    // Disable Command
    static bool HandleDisableCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        // Turn Disabled On But Don't Change Value...
        me->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
        me->GetSession()->SendAreaTriggerMessage("%s", handler->GetFirelandsString(IXP_COMMAND_DISABLED));
        return true;
    }

    // Enable Command
    static bool HandleEnableCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        me->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
        me->GetSession()->SendAreaTriggerMessage("%s", handler->GetFirelandsString(IXP_COMMAND_ENABLED));
        return true;
    }

    // Default Command
    static bool HandleDefaultCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!moduleEnabled)
        {
            handler->PSendSysMessage(IXP_MODULE_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* me = handler->GetSession()->GetPlayer();
        if (!me)
            return false;

        currentRate = defaultRate;
        me->GetSession()->SendAreaTriggerMessage(handler->GetFirelandsString(IXP_COMMAND_DEFAULT), defaultRate);
        return true;
    }
};

// Add all scripts in one
void AddModIndXPScripts()
{
    moduleEnabled = sConfigMgr->GetBoolDefault("IndividualXP.Enabled", true);
    maxRate = sConfigMgr->GetBoolDefault("IndividualXP.MaxRate", 10);
    defaultRate = sConfigMgr->GetBoolDefault("IndividualXP.DefaultRate", 1);

    new IndividualXP_Announce();
    new IndividualXP();
    new IndividualXPCommandScript();
}
