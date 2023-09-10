#pragma once

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <ArduinoJson.h>

namespace cobold
{
    namespace configuration
    {
        class Configuration : public cobold::configuration::IConfiguration
        {
        protected:
            std::map<std::string, std::string> configMap;

        public:
            Configuration()
            {
            }

            std::string getValue(const std::string &key)
            {
                auto it = configMap.find(key);
                if (it != configMap.end())
                {
                    return it->second;
                }
                else
                {
                    return "n/a";
                }
            }

            void setValue(const std::string &key, const std::string &value)
            {
                configMap[key] = value;
            }

            void deleteKey(const std::string &key)
            {
                configMap.erase(key);
            }

            bool keyExists(const std::string &key)
            {
                return configMap.find(key) != configMap.end();
            }

            std::string findKeyStartingWith(const std::string &prefix)
            {
                for (const auto &pair : configMap)
                {
                    if (pair.first.find(prefix) == 0)
                    {
                        return pair.first;
                    }
                }
                return "";
            }

            std::vector<std::string> findKeysByRegex(const std::string &regexPattern)
            {
                std::vector<std::string> matchingKeys;
                std::regex pattern(regexPattern);

                for (const auto &pair : configMap)
                {
                    if (std::regex_match(pair.first, pattern))
                    {
                        matchingKeys.push_back(pair.first);
                    }
                }

                return matchingKeys;
            }

            cobold::configuration::IConfiguration *getSection(const std::string &path) override
            {
                cobold::configuration::IConfiguration *sectionConfig = new Configuration();
                
                Serial.println("getSection");

                std::string regexPattern = path + "\\..*";
                std::regex pattern(regexPattern);

                std::string prefix = path + ".";
                size_t prefixLength = prefix.length();

                for (const auto &pair : configMap)
                {

                           Serial.println(pair.first.c_str());
                        Serial.println(pair.second.c_str());
                    if (std::regex_match(pair.first, pattern))
                    {
                        std::string shortenedKey = pair.first.substr(prefixLength);
                 
                        sectionConfig->setValue(shortenedKey, pair.second);
                    }
                }


                return sectionConfig;
            }

            void loadFromJson(const char *jsonString)
            {
                StaticJsonDocument<256> jsonDoc;
                deserializeJson(jsonDoc, jsonString);

                for (const JsonPair &kvp : jsonDoc.as<JsonObject>())
                {

                    configMap[std::string(kvp.key().c_str())] = kvp.value().as<std::string>();
                }
            }

            void update(IConfiguration *configuration)
            {
                for (const auto &pair : configMap)
                {
                    configuration->setValue(pair.first, pair.second);
                }
            }
        };

    }
}