#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\model_loader.hpp"
#include "mesh\static_mesh.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of model descriptors describing a complete model.
	 */
	class ModelDescriptor : public Resource< ModelDescriptor > {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a model descriptor.

		 @pre			The device associated with the current engine
						must be loaded.
		 @pre			The resource manager associated with the current engine
						must be loaded.
		 @tparam		VertexT
						The vertex type.
		 @param[in]		fname
						The filename (the globally unique identifier).
		 @param[in]		desc
						A reference to the mesh descriptor.
		 @param[in]		export_as_MDL
						@c true if the model descriptor needs to be
						exported as MDL file. @c false otherwise.
		 @throws		FormattedException
						Failed to initialize the model descriptor.
		 */
		template < typename VertexT >
		explicit ModelDescriptor(wstring fname, 
			const MeshDescriptor< VertexT > &desc = MeshDescriptor< VertexT >(),
			bool export_as_MDL = false);

		/**
		 Constructs a model descriptor.

		 @pre			@a device is not equal to @c nullptr.
		 @pre			The resource manager associated with the current engine
						must be loaded.
		 @tparam		VertexT
						The vertex type.
		 @param[in]		fname
						The filename (the globally unique identifier).
		 @param[in]		device
						A pointer to the device.
		 @param[in]		desc
						A reference to the mesh descriptor.
		 @param[in]		export_as_MDL
						@c true if the model descriptor needs to be exported as 
						MDL file. @c false otherwise.
		 @throws		FormattedException
						Failed to initialize the model descriptor.
		 */
		template < typename VertexT >
		explicit ModelDescriptor(wstring fname, ID3D11Device5 *device,
			const MeshDescriptor< VertexT > &desc = MeshDescriptor< VertexT >(),
			bool export_as_MDL = false);
		
		/**
		 Constructs a model descriptor from the given model descriptor.

		 @param[in]		desc
						A reference to the model descriptor to copy.
		 */
		ModelDescriptor(const ModelDescriptor &desc) = delete;

		/**
		 Constructs a model descriptor by moving the given model descriptor.

		 @param[in]		desc
						A reference to the model descriptor to move.
		 */
		ModelDescriptor(ModelDescriptor &&desc);

		/**
		 Destructs a model descriptor.
		 */
		virtual ~ModelDescriptor();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given model descriptor to this model descriptor.

		 @param[in]		desc
						A reference to the model descriptor to copy.
		 */
		ModelDescriptor &operator=(const ModelDescriptor &desc) = delete;

		/**
		 Moves the given model descriptor to this model descriptor.

		 @param[in]		desc
						A reference to the model descriptor to move.
		 */
		ModelDescriptor &operator=(ModelDescriptor &&desc) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the mesh of this model descriptor.

		 @return		A pointer to the mesh of this model descriptor.
		 */
		SharedPtr< const Mesh > GetMesh() const noexcept {
			return m_mesh;
		}

		/**
		 Returns the material corresponding to the given name.

		 @param[in]		name
						A reference to the name of the material.
		 @return		@c nullptr if this model descriptor contains no 
						material matching the given name @a name.
		 @return		A pointer to the material of this model 
						descriptor matching the given name @a name.
		 */
		const Material *GetMaterial(const string &name) const noexcept;

		/**
		 Traverses all materials of this model descriptor.

		 @tparam		ActionT
						An action to perform on all materials of this model 
						descriptor. The action must accept @c const 
						@c Material* values.
		*/
		template< typename ActionT >
		void ForEachMaterial(ActionT action) const;
		
		/**
		 Returns the model part corresponding to the given name.

		 @param[in]		name
						A reference to the name of the model part.
		 @return		@c nullptr if this model descriptor contains no model 
						part matching the given name @a name.
		 @return		A pointer to the model part of this model descriptor 
						matching the given name @a name.
		 */
		const ModelPart *GetModelPart(const string &name) const noexcept;
		
		/**
		 Traverses all model parts of this model descriptor.

		 @tparam		ActionT
						An action to perform on all model parts of this model
						descriptor. The action must accept @c const 
						@c ModelPart* values.
		 */
		template< typename ActionT >
		void ForEachModelPart(ActionT action) const;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A pointer to the mesh of the model of this model descriptor.
		 */
		SharedPtr< const Mesh > m_mesh;

		/**
		 A vector containing all the materials of the model of this model 
		 descriptor.
		 */
		vector< Material > m_materials;

		/**
		 A vector containing all the model parts of the model of this model 
		 descriptor.
		 */
		vector< ModelPart > m_model_parts;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "model\model_descriptor.tpp"

#pragma endregion