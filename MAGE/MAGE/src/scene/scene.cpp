//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	Scene::Scene(const string &name)
		: m_name(name), 
		m_scripts(), m_scene_fog(), m_cameras(), m_models(), m_ambient_light(), 
		m_directional_lights(), m_omni_lights(), m_spot_lights(), m_sprites() {}

	Scene::~Scene() {
		// Clears this scene.
		Clear();
	}
	
	//-------------------------------------------------------------------------
	// Scene Member Methods: Lifecycle
	//-------------------------------------------------------------------------

	void Scene::Initialize() {
		m_scene_fog = MakeUnique< SceneFog >();

		// Loads this scene.
		Load();
	}
	
	void Scene::Uninitialize() {
		// Closes this scene.
		Close();

		// Clears this scene.
		Clear();
	}

	void Scene::Clear() noexcept {
		// Scripts
		RemoveAllScripts();

		// World
		m_scene_fog.reset();
		m_cameras.clear();
		m_models.clear();
		m_ambient_light.reset();
		m_directional_lights.clear();
		m_omni_lights.clear();
		m_spot_lights.clear();
		m_sprites.clear();
	}

	//-------------------------------------------------------------------------
	// Scene Member Methods: Scripts
	//-------------------------------------------------------------------------

	bool Scene::HasScript(SharedPtr< const BehaviorScript > script) const {
		return std::find(m_scripts.begin(), m_scripts.end(), script) != m_scripts.end();
	}
	
	void Scene::AddScript(SharedPtr< BehaviorScript > script) {
		if (!script) {
			return;
		}

		m_scripts.push_back(std::move(script));
	}
	
	void Scene::RemoveScript(SharedPtr< BehaviorScript > script) {
		const auto it = std::find(m_scripts.begin(), m_scripts.end(), script);
		if (it != m_scripts.end()) {
			m_scripts.erase(it);
		}
	}
	
	void Scene::RemoveAllScripts() noexcept {
		m_scripts.clear();
	}

	//-------------------------------------------------------------------------
	// Scene Member Methods: World
	//-------------------------------------------------------------------------

	SharedPtr< ModelNode > Scene::CreateModel(const ModelDescriptor &desc) {

		// Create a default material.
		const Material default_material("material");

		SharedPtr< ModelNode > root_model_node;
		size_t nb_root_childs = 0;

		using ModelPair = pair< SharedPtr< ModelNode >, string >;
		map< string, ModelPair > mapping;

		// Create model nodes.
		desc.ForEachModelPart([&](const ModelPart *model_part) {

			if (model_part->m_child == MAGE_MDL_PART_DEFAULT_CHILD && model_part->m_nb_indices == 0) {
				return;
			}

			// Create a submodel node.
			const SharedPtr< ModelNode > submodel_node = MakeShared< ModelNode >(
															model_part->m_child, desc.GetMesh(),
															model_part->m_start_index, model_part->m_nb_indices,
															model_part->m_aabb, model_part->m_bs);
			// Create a material.
			const Material material = (model_part->m_material == MAGE_MDL_PART_DEFAULT_MATERIAL) ?
										default_material : *desc.GetMaterial(model_part->m_material);
			submodel_node->GetModel()->SetMaterial(material);

			// Add this submodel node to this scene.
			AddSceneNode(submodel_node);

			if (model_part->m_parent == MAGE_MDL_PART_DEFAULT_PARENT) {
				root_model_node = submodel_node;
				++nb_root_childs;
			}

			// Add this submodel node to the mapping.
			mapping.insert(std::make_pair(
							model_part->m_child, 
							ModelPair(submodel_node, model_part->m_parent)));
		});

		Assert(nb_root_childs != 0);

		const bool create_root_model_node = (nb_root_childs > 1);

		// Create root model node.
		if (create_root_model_node) {
			root_model_node = MakeShared< ModelNode >(
				"model", desc.GetMesh(), 0, 0, AABB(), BS());
			
			// Add this root model node to this scene.
			AddSceneNode(root_model_node);
		}

		// Connect model nodes.
		for (const auto &model_pair : mapping) {
			const SharedPtr< ModelNode > &child = model_pair.second.first;
			const string &parent                = model_pair.second.second;
			if (parent == MAGE_MDL_PART_DEFAULT_PARENT) {
				if (create_root_model_node) {
					root_model_node->AddChildNode(child);
				}
			}
			else {
				mapping[parent].first->AddChildNode(child);
			}
		}

		return root_model_node;
	}

	void Scene::AddSceneNode(SharedPtr< CameraNode > camera) {
		if (!camera) {
			return;
		}

		m_cameras.push_back(std::move(camera));
	}
	
	void Scene::AddSceneNode(SharedPtr< ModelNode > model) {
		if (!model) {
			return;
		}

		m_models.push_back(std::move(model));
	}
	
	void Scene::AddSceneNode(SharedPtr< AmbientLightNode > light) {
		if (!light) {
			return;
		}

		m_ambient_light = std::move(light);
	}
	
	void Scene::AddSceneNode(SharedPtr< DirectionalLightNode > light) {
		if (!light) {
			return;
		}

		m_directional_lights.push_back(std::move(light));
	}
	
	void Scene::AddSceneNode(SharedPtr< OmniLightNode > light) {
		if (!light) {
			return;
		}

		m_omni_lights.push_back(std::move(light));
	}
	
	void Scene::AddSceneNode(SharedPtr< SpotLightNode > light) {
		if (!light) {
			return;
		}

		m_spot_lights.push_back(std::move(light));
	}
	
	void Scene::AddSceneNode(SharedPtr< SpriteNode > sprite) {
		if (!sprite) {
			return;
		}

		m_sprites.push_back(std::move(sprite));
	}
}