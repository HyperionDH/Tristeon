﻿#include "SceneManager.h"
#include "Core/Message.h"
#include "Core/ManagerProtocol.h"
#include "Scene.h"
#include "Core/Rendering/Components/MeshRenderer.h"
#include "Editor/JsonSerializer.h"

namespace Tristeon
{
	namespace Scenes
	{
		std::unique_ptr<Scene> SceneManager::activeScene = nullptr;
		std::map<std::string, std::string> SceneManager::sceneFilePaths;

		SceneManager::~SceneManager()
		{
			activeScene.reset();
		}

		void SceneManager::init()
		{
			activeScene = std::make_unique<Scene>();
			activeScene->name = "UnNamed";

			//Load scenes into the manager
			std::ifstream stream("Scenes.ProjectSettings", std::fstream::in | std::fstream::out | std::fstream::app);
			nlohmann::json json;
			stream >> json;

			//Invalid json error handling
			if (json.is_null())
			{
				std::cout << "file is either a non-json file or corrupted" << std::endl;
				throw std::invalid_argument("file is either a non-json file or corrupted");
			}
			for (auto iterator = json.begin(); iterator != json.end(); iterator++)
			{
				const std::string stringValue = iterator.value();
				sceneFilePaths[iterator.key()] = stringValue;
			}
		}

		void SceneManager::loadScene(int id)
		{
		}

		void SceneManager::loadScene(std::string name)
		{
			Core::ManagerProtocol::sendMessage(Core::MT_MANAGER_RESET);
			auto scene = JsonSerializer::deserialize<Scene>(sceneFilePaths[name]);
			activeScene = std::unique_ptr<Scene>(scene);
		}

		void SceneManager::loadScene(Scene* scene)
		{
			activeScene = std::unique_ptr<Scene>(scene);
			createParentalBonds(activeScene.get());
		}

		Core::GameObject* SceneManager::findGameObjectWithInstanceID(int instanceID)
		{
			for (int i = 0; i < activeScene->gameObjects.size(); ++i)
			{
				if (activeScene->gameObjects[i]->getInstanceID() == instanceID)
					return activeScene->gameObjects[i].get();
			}
			return nullptr;
		}

		void SceneManager::createParentalBonds(Scene* scene)
		{
			std::vector<std::unique_ptr<Core::GameObject>>& gameObjects = scene->gameObjects;
			for (int i = 0; i < gameObjects.size(); ++i)
			{
				if (gameObjects[i]->getInstanceID() != -1)
				{
					gameObjects[i]->transform->setParent(findGameObjectWithInstanceID(gameObjects[i]->getInstanceID())->transform);
				}
			}
		}

		void SceneManager::addScenePath(std::string name, std::string path)
		{
			sceneFilePaths[name] = path;
			nlohmann::json json(sceneFilePaths);
			JsonSerializer::serialize("Scenes.ProjectSettings", json);
		}

		void SceneManager::reset()
		{
			//TODO: SceneManager::reset()
		}
	}
}
