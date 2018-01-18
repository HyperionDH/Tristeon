﻿#include "Scene.h"

namespace Tristeon
{
	namespace Scenes
	{
		REGISTER_TYPE_CPP(Scene)

		Scene::Scene()
		{
		}

		void Scene::init()
		{
			for (int i = 0; i < gameObjects.size(); i++)
				gameObjects[i]->init();
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json output;
			output["typeID"] = typeid(Scene).name();
			output["name"] = name;
			nlohmann::json jsonArray = nlohmann::json::array();
			for (int i = 0; i < gameObjects.size(); i++)
			{
				jsonArray.push_back(gameObjects[i]->serialize());
			}
			output["gameObjects"] = jsonArray;
			return output;
		}

		void Scene::deserialize(nlohmann::json json)
		{
			nlohmann::json gameObjectData = json["gameObjects"];
			if (gameObjectData.is_array())
			{
				for (auto iterator = gameObjectData.begin(); iterator != gameObjectData.end(); ++iterator)
				{
					std::unique_ptr<Core::GameObject> gameObject = std::make_unique<Core::GameObject>();
					gameObject->deserialize(iterator->get<nlohmann::json>());
					gameObjects.push_back(std::move(gameObject));
				}
			} else
			{
				std::cout << "Deserialization of the scene is going goofy, ur probably deserializing the scene with a wrong json format";
			}
			const std::string nameValue = json["name"];
			name = nameValue;
		}

		void Scene::addGameObject(Core::GameObject* gameObj)
		{
			gameObjects.push_back(std::move(std::unique_ptr<Core::GameObject>(gameObj)));
			nlohmann::json json = gameObj->serialize();
			gameObj->deserialize(json);
		}

		void Scene::removeGameObject(Core::GameObject* gameObj)
		{
			for (int i = 0; i < gameObjects.size(); ++i)
			{
				if (gameObjects[i].get() == gameObj) { gameObjects.erase(gameObjects.begin() + i); }
			}
		}

		Core::GameObject* Scene::getGameObject(std::string instanceID)
		{
			for (int i = 0; i < gameObjects.size(); ++i)
			{
				if (gameObjects[i]->getInstanceID() == instanceID) return gameObjects[i].get();
			}
			std::cout << "Couldn't find gameObject\n";
			return nullptr;
		}
	}
}
