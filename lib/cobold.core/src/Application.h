#pragma once

#include "IApplication.h"
#include "ServiceCollection.h"
#include "IHost.h"
#include "IHostBuilder.h"
#include "IConfiguration.h"


namespace cobold
{

    class Application : public cobold::IApplication
    {
    public:
        Application(cobold::hosting::IHostBuilder *hostBuilder);
        ~Application();

        void preSetup() override;
        void setup() override;
        void loop() override;
        void run() override;

        ServiceCollection *getServices() override;
        cobold::hosting::IHost *getHost() override;
        cobold::hosting::IHostBuilder *getHostBuilder() override;
        cobold::configuration::IConfiguration *getAppConfiguration() override;

    private:
        ServiceCollection *services;
        cobold::hosting::IHost *host;
        cobold::hosting::IHostBuilder *hostBuilder;
        cobold::configuration::IConfiguration *configuration;
    };

    
} // namespace cobold
