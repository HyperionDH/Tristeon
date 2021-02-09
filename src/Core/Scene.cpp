﻿#include "Scene.h"
#include <iostream>

namespace Tristeon::Core
{
	REGISTER_TYPE_CPP(Scene)

	nlohmann::json Scene::serialize()
	{
		nlohmann::json output;
		output["typeID"] = Type<Scene>::fullName();
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

	void Scene::add(std::unique_ptr<Core::GameObject> gameObj)
	{
		gameObj->deserialize(gameObj->serialize());
		gameObjects.push_back(std::move(gameObj));
	}

	void Scene::remove(Core::GameObject* gameObj)
	{
		for (int i = 0; i < gameObjects.size(); ++i)
		{
			if (gameObjects[i].get() == gameObj) { gameObjects.erase(gameObjects.begin() + i); }
		}
	}

	Core::GameObject* Scene::get(std::string instanceID)
	{
		for (int i = 0; i < gameObjects.size(); ++i)
		{
			if (gameObjects[i]->getInstanceID() == instanceID) return gameObjects[i].get();
		}
		std::cout << "Couldn't find gameObject\n";
		return nullptr;
	}
}
