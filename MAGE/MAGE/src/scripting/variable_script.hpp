#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "resource\resource.hpp"
#include "scripting\variable.hpp"
#include "utils\collection\collection.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {
	
	/**
	 A class of variable scripts.
	 */
	class VariableScript : public Resource< VariableScript > {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a variable script.

		 @param[in]		fname
						The filename (the globally unique identifier).
		 @param[in]		import
						Flag indicating whether the variables of the variable 
						script need to be imported.
		 @throws		FormattedException
						Failed to import the variable script from file (only 
						possible if @a import is equal to @c true).
		 */
		explicit VariableScript(wstring fname, bool import = true);

		/**
		 Constructs a variable script from the given variable script.

		 @param[in]		variable_script
						A reference to the variable script to copy.
		 */
		VariableScript(const VariableScript &variable_script) = delete;

		/**
		 Constructs a variable script by moving the given variable script.

		 @param[in]		variable_script
						A reference to the variable script to move.
		 */
		VariableScript(VariableScript &&variable_script);

		/**
		 Destruct this variable script.
		 */
		virtual ~VariableScript();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given variable script to this variable script.

		 @param[in]		variable_script
						A reference to the variable script to copy.
		 @return		A reference to the copy of the given variable script
						(i.e. this variable script).
		 */
		VariableScript &operator=(const VariableScript &variable_script) = delete;

		/**
		 Moves the given variable script to this variable script.

		 @param[in]		variable_script
						A reference to the variable script to move.
		 @return		A reference to the moved variable script (i.e. this 
						variable script).
		 */
		VariableScript &operator=(VariableScript &&variable_script) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Imports this variable script from the file of this variable script.

		 @throws		FormattedException
						Failed to import the variable script from file.
		 */
		void ImportScript() {
			ImportScript(GetFilename());
		}

		/**
		 Imports this variable script from the file with the given filename.

		 @param[in]		fname
						A reference to the filename.
		 @throws		FormattedException
						Failed to import the variable script from file.
		 */
		void ImportScript(const wstring &fname);

		/**
		 Exports this variable script to the file of this variable script.

		 @throws		FormattedException
						Failed to export the variable script to file.
		 */
		void ExportScript() const {
			ExportScript(GetFilename());
		}

		/**
		 Exports this variable script to the file with the given filename.

		 @param[in]		fname
						A reference to the filename.
		 @throws		FormattedException
						Failed to export the variable script to file.
		 */
		void ExportScript(const wstring &fname) const;

		/**
		 Checks whether this variable script is empty.

		 @return		@c true if this variable script is empty. @c false 
						otherwise.
		 */
		bool IsEmpty() const noexcept {
			return m_variables.empty();
		}

		/**
		 Returns the number of variables in this variable script.

		 @return		The number of variables in this variable script.
		 */
		size_t GetNumberOfVariables() const noexcept {
			return m_variables.size();
		}

		/**
		 Checks whether this variable script has a variable with the given 
		 name.

		 @param[in]		name
						A reference to the name of the variable.
		 @return		@c true if this variable script has a variable with the
						given name. @c false otherwise.
		 */
		bool HasVariable(const string &name) const noexcept;

		/**
		 Checks whether this variable script has a variable with the given 
		 name and type.

		 @tparam		T
						The type of the variable.
		 @param[in]		name
						A reference to the name of the variable.
		 @return		@c true if this variable script has a variable with the
						given name. @c false otherwise.
		 */
		template< typename T >
		bool HasVariableOfType(const string &name) const noexcept;

		/**
		 Adds the given variable to this variable script.

		 @tparam		T
						The type of the variable.
		 @param[in]		name
						The name of the variable.
		 @param[in]		value
						The value of the variable.
		 */
		template< typename T >
		void AddVariable(string name, T value);

		/**
		 Removes the given variable from this variable script.

		 @param[in]		name
						The name of the variable.
		 */
		void RemoveVariable(const string &name);

		/**
		 Removes all variables from this variable script.
		 */
		void RemoveAllVariables() noexcept;

		/**
		 Returns the value of the given variable in this variable script.

		 @tparam		T
						The type of the variable.
		 @param[in]		name
						A reference to the name of the variable.
		 @return		@c nullptr if no variable with the name @a name exists 
						in this variable script.
		 @return		A pointer to the value of the variable.
		 @throws		@c std::bad_variant_access
						The variable corresponding to the given name has a 
						different type.
		 */
		template< typename T >
		const T *GetValueOfVariable(const string &name) const;

		/**
		 Sets the value of the given variable in this variable script.

		 @tparam		T
						The type of the variable.
		 @param[in]		name
						A reference to the name of the variable.
		 @param[in]		value
						The value of the variable.
		 @note			Nothing happens if no variable with the name @a name 
						exists in this variable script.
		 */
		template< typename T >
		void SetValueOfVariable(const string &name, T value);

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A map containing the variables in this variable script.
		 */
		map< string, Value > m_variables;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scripting\variable_script.tpp"

#pragma endregion