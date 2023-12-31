#pragma once

#include <vector>
#include <functional>

#include "ServiceCollection.h"
#include "IHostedService.h"
#include "IHostBuilder.h"
#include "Logger.h"


namespace cobold
{
    namespace hosting
    {

        class HostingExtensions
        {
        private:
            /* data */
        public:
            /**
             * @brief Get a list of services that inherit from the specified interface.
             *
             * @tparam TInterface The interface type to check for.
             * @return A vector of services that inherit from the specified interface.
             */
            static std::vector<cobold::hosting::IHostedService *> GetHostedServices(ServiceCollection *serviceCollection)
            {
                Serial.println("HostingExtensions::GetHostedServices");
                std::vector<cobold::hosting::IHostedService *> servicesList;
                SerialLogger *logger = serviceCollection->getService<cobold::SerialLogger>();

                // Check the existing services map
                for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                {
                    cobold::sys::BaseObject *typeWrapper = it->first;

                    if (cobold::hosting::HostingExtensions::isHostedService(typeWrapper))
                    {
                        logger->verbose("Service found");
                        void *servicePtr = it->second();
                        cobold::hosting::IHostedService *service = reinterpret_cast<cobold::hosting::IHostedService *>(servicePtr);
                        if (service)
                        {
                            servicesList.push_back(service);
                        }
                    }
                }

                // Check the constructorMap for new services
                for (auto constructorIt = serviceCollection->constructorMap.begin(); constructorIt != serviceCollection->constructorMap.end(); ++constructorIt)
                {
                    cobold::sys::BaseObject *typeWrapper = constructorIt->first;

                    // Compare wrapped type with TInterface using is_base_of and typeid
                    if (cobold::hosting::HostingExtensions::isHostedService(typeWrapper))
                    {
                        // if not already in the services map
                        bool isAlreadyInServicesMap = false;
                        for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                        {
                            cobold::sys::BaseObject *srvTypeWrapper = it->first;

                            if (srvTypeWrapper->getTypeName() == typeWrapper->getTypeName())
                            {
                                isAlreadyInServicesMap = true;
                                break;
                            }
                        }

                        if (isAlreadyInServicesMap)
                        {
                            continue;
                        }
                        else
                        {
                            logger->verbose("Service found, creating new instance");
                            void *newService = constructorIt->second(serviceCollection);
                            serviceCollection->services[typeWrapper] = [newService]() -> void *
                            { return newService; };

                            cobold::hosting::IHostedService *service = reinterpret_cast<cobold::hosting::IHostedService *>(newService);
                            if (service)
                            {
                                servicesList.push_back(service);
                            }
                        }
                    }
                }

                return servicesList;
            }

            template <typename T>
            static void addHostedService(ServiceCollection *serviceCollection, std::function<void *(ServiceCollection *)> constructor)
            {
                cobold::sys::BaseObject *typeWrapper = new cobold::sys::Object<T>();

                typeWrapper->setProperty("cobold::hosting::IHostedService", "true");
                serviceCollection->addExternalService(typeWrapper, constructor);
            }

            static bool isHostedService(cobold::sys::BaseObject *typeWrapper)
            {
                return typeWrapper->getProperty("cobold::hosting::IHostedService") == "true";
            }
        };

    } // namespace hosting
} // namespace cobold