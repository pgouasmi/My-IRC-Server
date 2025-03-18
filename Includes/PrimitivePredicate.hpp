#ifndef PRIMITIVEPREDICATE_HPP
	#define PRIMITIVEPREDICATE_HPP
	#include <string>

	/**
	 * @class StringPredicate
	 * @brief A predicate class for comparing strings.
	 *
	 * This class provides a way to compare a string with a target string.
	 * It can be used with STL algorithms like find_if or remove_if.
	 *
	 * @author jbadaire
	 */
	class StringPredicate
	{
		private:
			std::string target;

		public:
			explicit StringPredicate(const std::string &v);

			/**
			 * @brief Compares the given string with the target string.
			 * @param a The string to compare.
			 * @return true if the strings are equal, false otherwise.
			 * @author jbadaire
			 */
			bool operator ()(const std::string &a);

			/**
			 * @brief Checks if the given string is equal to the target string.
			 * @param a The string to check.
			 * @return true if the strings are equal, false otherwise.
			 * @author jbadaire
			 */
			bool operator ==(const std::string &a);
	};

	/**
	 * @class SizeTPredicate
	 * @brief A predicate class for comparing size_t values.
	 *
	 * This class provides a way to compare a size_t value with a target size_t value.
	 * It can be used with STL algorithms like find_if or remove_if.
	 *
	 * @author jbadaire
	 */
	class SizeTPredicate
	{
		private:
			size_t target;

		public:
			explicit SizeTPredicate(size_t v);

			/**
			 * @brief Compares the given size_t value with the target size_t value.
			 * @param a The size_t value to compare.
			 * @return true if the values are equal, false otherwise.
			 * @author jbadaire
			 */
			bool operator ()(size_t a);

			/**
			 * @brief Checks if the given size_t value is equal to the target size_t value.
			 * @param a The size_t value to check.
			 * @return true if the values are equal, false otherwise.
			 * @author jbadaire
			 */
			bool operator ==(size_t a);
	};
#endif
