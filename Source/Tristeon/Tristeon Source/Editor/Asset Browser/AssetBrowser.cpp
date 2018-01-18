#include "AssetBrowser.h"
#include "FileItemManager.h"
#include "FolderItem.h"
#include <GLFW/glfw3.h>
#include "SceneFileItem.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "MaterialFileItem.h"
#include "Core/Rendering/Material.h"
#include "Editor/EditorNode.h"
#include "Editor/EditorDragging.h"
#include "PrefabFileItem.h"
#include "ShaderFileItem.h"
#include "Editor/TristeonEditor.h"
#include "SkyboxFileItem.h"

using namespace Tristeon::Editor;
using namespace Tristeon;

const std::string AssetBrowser::assetPath = "Assets/";

AssetBrowser::AssetBrowser()
{
	//Setup rootfolder
	rootFolder = std::make_shared<FolderItem>();
	rootFolder->filepath = "Assets";
	rootFolder->isFolder = true;
	rootFolder->setup(true);

	//Set asset browser view to the asset folder (rootfolder)
	itemManager = std::make_unique<FileItemManager>(editor->getBindingData());
	itemManager->setView(rootFolder.get());
}

AssetBrowser::~AssetBrowser()
{
}

void AssetBrowser::onGui()
{
	//On CTL+S
	if (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) && ImGui::IsKeyPressed(GLFW_KEY_S,false))
	{
		//Save current scene
		if (Scenes::SceneManager::getActiveScene() != nullptr) {
			Scenes::Scene* currentScene = Scenes::SceneManager::getActiveScene();
			std::cout << "Saving scene: " << currentScene->name << std::endl;
			if (itemManager->currentlyLoadedSceneFile != nullptr)
				itemManager->currentlyLoadedSceneFile->createFile(currentScene->serialize());
			else
			{
				//Create a scene file
				SceneFileItem* sceneFile = new SceneFileItem();
				sceneFile->init("Scene", itemManager->currentFolder, "scene"); //Create meta data
				sceneFile->createFile(currentScene->serialize()); //Create create the scene file
			}
		}
	}

	ImGui::Begin("Tristeon file window",0,windowFlags); // begin window

	//Draw folder hierarchy
	ImGui::BeginChild("Left panel", ImVec2(150, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	folderHierarchy.drawHierarchy(rootFolder,itemManager.get());
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Right panel", ImVec2(0, 0), ImGuiWindowFlags_HorizontalScrollbar);

	//Dropped on assetbrowser
	//handleDroppedItems();


	// Right click popup (opens asset creation window)
	if (ImGui::BeginPopupContextWindow("Asset creation menu")) {

		//TODO: Select desired asset type to create and then create name after fileitem is created
		//		aka remove hard-code
		// Asset creation UI
		ImGui::InputText("Asset name", createdItemName, 255);

		if (std::string(createdItemName).size() < 24)
		{
			if (ImGui::Button("Create folder") || ImGui::IsKeyPressed(GLFW_KEY_ENTER,false))
			{
				std::unique_ptr<FolderItem> folder = std::make_unique<FolderItem>();
				folder->init(createdItemName, itemManager->currentFolder); //Create both folder and meta data
				//Set parenting and child relations
				folder->parent = itemManager->currentFolder;
				itemManager->currentFolder->children.push_back(std::move(folder));
				itemManager->currentFolder->fileItems.push_back(folder.get());
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Create scene"))
			{
				//Create a scene file
				SceneFileItem* sceneFile = new SceneFileItem();
				sceneFile->init(createdItemName,itemManager->currentFolder,"scene"); //Create meta data
				Scenes::Scene createdScene = Scenes::Scene(); //Create empty scene
				createdScene.name = createdItemName;
				sceneFile->createFile(createdScene.serialize()); //Create create the scene file
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Create material"))
			{
				MaterialFileItem* materialFile = new MaterialFileItem();
				materialFile->init(createdItemName, itemManager->currentFolder, "mat");
				Core::Rendering::Material material;
				materialFile->createFile(material.serialize());
			}
			if (ImGui::Button("Create shader"))
			{
				ShaderFileItem* shaderItem = new ShaderFileItem();
				shaderItem->init(createdItemName, itemManager->currentFolder, "shader");
				Core::Rendering::ShaderFile file;
				shaderItem->createFile(file.serialize());
			}
			if (ImGui::Button("Create skybox"))
			{
				SkyboxFileItem* skyboxItem = new SkyboxFileItem();
				skyboxItem->init(createdItemName, itemManager->currentFolder, "skybox");
				skyboxItem->createFile(Core::Rendering::Skybox().serialize());
			}
		}
		else std::cout << "Can't create items with more than 24 characters\n";
		ImGui::EndPopup();
	}
	
	itemManager->drawFileItems();
	ImGui::EndChild();

	ImGui::End(); // end window
}

void AssetBrowser::handleDroppedItems()
{
	//Drop condition
	if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
	{
		//Is draggable an editor node?
		Draggable* draggable = EditorDragging::getDragableItem();
		EditorNode* draggingNode = dynamic_cast<EditorNode*>(draggable);
		if (draggingNode != nullptr)
		{
			PrefabFileItem* prefab = new PrefabFileItem();
			prefab->init((*draggingNode->getData())["name"], itemManager->currentFolder, "prefab");
			prefab->createFile(*draggingNode->getData());
			draggingNode->setPrefab(prefab);
		}
	}
}
