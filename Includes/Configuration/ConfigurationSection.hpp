#ifndef CONFIGURATIONSECTION_HPP
#define CONFIGURATIONSECTION_HPP

	#include <string>
	#include <map>
	#include <vector>
	class ConfigurationSection {
		private:
			std::string name;
			std::map<std::string, std::string> map;

		public:
			ConfigurationSection(const std::string& name);
			const std::string& getName() const;
			void addEntry(const std::string& key, const std::string& value);
			std::string getStringValue(const std::string& key, std::string defaultValue = "NONE");
			size_t getNumericValue(const std::string& key, size_t defaultValue = -1);
			bool getBooleanValue(const std::string& key, bool defaultValue = false);
			std::vector<std::string> getVectorValue(const std::string& key);

			std::map<std::string, std::string>& getValues();
	};

#endif
