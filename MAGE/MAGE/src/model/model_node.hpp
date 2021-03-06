#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\transform\transform_node.hpp"
#include "model\model.hpp"
#include "math\transform\texture_transform.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of model nodes.
	 */
	class ModelNode : public Node {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a model node.

		 @tparam		ConstructorArgsT
						The constructor argument types of the model.
		 @param[in]		name
						The name.
		 @param[in]		args
						A reference to the constructor arguments for the model.
		 */
		template< typename... ConstructorArgsT >
		explicit ModelNode(string name, ConstructorArgsT&&... args);

		/**
		 Constructs a model node.

		 @pre			@a model refers to a non @c nullptr.
		 @param[in]		name
						The name.
		 @param[in]		model
						A reference to the model to move.
		 */
		explicit ModelNode(string name, UniquePtr< Model > &&model);

		/**
		 Constructs a model node from the given model node.

		 @param[in]		model_node
						A reference to the model node to copy.
		 */
		ModelNode(const ModelNode &model_node);

		/**
		 Constructs a model node by moving the given model node.

		 @param[in]		model_node
						A reference to the model node to move.
		 */
		ModelNode(ModelNode &&model_node);

		/**
		 Destructs this model node.
		 */
		virtual ~ModelNode();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given model node to this model node.

		 @param[in]		model_node
						A reference to the model node to copy.
		 @return		A reference to the copy of the given model node (i.e. 
						this model node).
		 */
		ModelNode &operator=(const ModelNode &model_node) = delete;

		/**
		 Moves the given model node to this model node.

		 @param[in]		model_node
						A reference to the model node to move.
		 @return		A reference to the moved model node (i.e. this model 
						node).
		 */
		ModelNode &operator=(ModelNode &&model_node) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Clones this model node.

		 @return		A pointer to the clone of this model node.
		 */
		UniquePtr< ModelNode > Clone() const {
			return static_pointer_cast< ModelNode >(CloneImplementation());
		}

		/**
		 Returns the texture transform of this model node.

		 @return		A pointer to the texture transform of this model node.
		 */
		TextureTransform *GetTextureTransform() noexcept {
			return m_texture_transform.get();
		}

		/**
		 Returns the texture transform of this model node.

		 @return		A pointer to the texture transform of this model node.
		 */
		const TextureTransform *GetTextureTransform() const noexcept {
			return m_texture_transform.get();
		}

		/**
		 Returns the model of this model node.

		 @return		A pointer to the model of this model node.
		 */
		Model *GetModel() noexcept {
			return m_model.get();
		}

		/**
		 Returns the model of this model node.

		 @return		A pointer to the model of this model node.
		 */
		const Model *GetModel() const noexcept {
			return m_model.get();
		}

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Clones this model node.

		 @return		A pointer to the clone of this model node.
		 */
		virtual UniquePtr< Node > CloneImplementation() const override;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A pointer to the model of this model node.
		 */
		UniquePtr< Model > m_model;

		/**
		 A pointer to the model of this model node.
		 */
		UniquePtr< TextureTransform > m_texture_transform;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "model\model_node.tpp"

#pragma endregion