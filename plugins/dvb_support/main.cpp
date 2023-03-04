#include "core/plugin.h"
#include "logger.h"
#include "core/module.h"

#include "dvbs/module_dvbs_demod.h"
#include "dvbs2/module_dvbs2_demod.h"
#include "dvbs2/module_s2_ts2tcp.h"

class DVBSupport : public satdump::Plugin
{
public:
    std::string getID()
    {
        return "dvb_support";
    }

    void init()
    {
        satdump::eventBus->register_handler<RegisterModulesEvent>(registerPluginsHandler);
    }

    static void registerPluginsHandler(const RegisterModulesEvent &evt)
    {
        REGISTER_MODULE_EXTERNAL(evt.modules_registry, dvb::DVBSDemodModule);
        REGISTER_MODULE_EXTERNAL(evt.modules_registry, dvb::DVBS2DemodModule);
        REGISTER_MODULE_EXTERNAL(evt.modules_registry, dvbs2::S2TStoTCPModule);
    }
};

PLUGIN_LOADER(DVBSupport)