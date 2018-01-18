#pragma once
#include "Selectable.h"
#include "json.hpp"
#include "Core/GameObject.h"
#include "Draggable.h"
#include "Asset Browser/PrefabFileItem.h"

namespace Tristeon
{
	namespace Editor
	{
		class EditorNode : public Selectable, public Draggable
		{
			friend class EditorCameraController;
			friend class EditorNodeTree;
			friend class EditorSelection;
		public:
			explicit EditorNode(Core::GameObject* gameObj);
			explicit EditorNode(Core::GameObject* gameObj, PrefabFileItem* prefab);
			~EditorNode() override;

			/**
			 * \brief Applies the json data to the connecetedGameObject
			 */
			void applyChanges();

			/**
			 * \brief Sets the data as the new specified json
			 */
			void load(nlohmann::json json);

			/**
			 * \brief Returns gameobject data in json format
			 */
			nlohmann::json* getData();

			/**
			 * \brief Sets a new parent
			 */
			void move(EditorNode* parent);
			
			/**
			 * \brief Checks if the given node is a child of this one
			 */
			bool hasChild(EditorNode* node);

			void setPrefab(PrefabFileItem* prefab);

			void setPrefabData(nlohmann::json prefabData);

			bool isPrefab() { return _isPrefab; }

			virtual nlohmann::json getPrefabData();

			void applyPrefab();

			/**
			 * \brief Unique typeid of the connectedGameObject
			 */
			std::string typeID;
			/**
			 * \brief Parent node
			 */
			EditorNode* parent = nullptr;
			std::vector<EditorNode*> children;
		protected:
			/**
			 * \brief The gameobject the editornode is bound to
			 */
			Core::GameObject* connectedGameObject;
			/**
			 * \brief The serialized data of the connectedGameObject
			 */
			nlohmann::json data;

			PrefabFileItem* prefab = nullptr;

			bool _isPrefab;
		};
	}
}
