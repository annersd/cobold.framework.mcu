#pragma once

#include "IApplication.h"
#include "IConfiguration.h"
#include "Configuration.h"

#include "MqttAppServices.h"
#include "NetworkAppServices.h"
#include "SchedulerAppServices.h"
#include "EventAppServices.h"
#include "TaskAppServices.h"

namespace cobold::application
{
    void deaultConfigure(IApplication *app, char *deviceConfig, char *secretsConfig)
    {
        // configure the application
        app->getAppConfiguration()->update(
            cobold::configuration::Configuration::fromJson(deviceConfig));
        app->getAppConfiguration()->update(
            cobold::configuration::Configuration::fromJson(secretsConfig));
    }

    void defaultPreSetup(IApplication *app)
    {
        app->preSetup();

        cobold::services::AddTaskService(app);
        cobold::services::AddEventService(app);
        cobold::services::AddSchedulerService(app);

        cobold::services::AddNetworkService(app);
        cobold::services::AddMqttClientService(app);
    }

    void defaultSetup(IApplication *app)
    {
        app->setup();

        cobold::services::UseTaskService(app);
        cobold::services::UseEventService(app);
        cobold::services::UseSchedulerService(app);

        cobold::services::UseNetworkService(app);
        cobold::services::UseMqttClientService(app);
    }

}