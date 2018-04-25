﻿#pragma once
#include "Core/Rendering/Internal/InternalRenderer.h"
#include "Core/Components/Component.h"
#include "Core/Rendering/Material.h"

namespace Tristeon
{
	namespace Scenes { class SceneManager; }
	namespace Core
	{
		//Forward decl
		class BindingData;
		class ManagerProtocol;

		namespace Rendering
		{
			//Forward decl
			class RenderManager;

			/**
			 * \brief Renderer is the base class for all renderers in the engine. 
			 * It provides base functionality and wraps around a so called "InternalRenderer", 
			 * which is designed to be created based on the current selected rendering API.
			 */
			class Renderer : public Components::Component
			{
				friend RenderManager;
				friend Scenes::SceneManager;
			public:
				/**
				 * \brief Render is the main rendering function of the renderer and calls internalRenderer.render(). It can be overriden if needed.
				 */
				virtual void render();
				/**
				 * \brief Deregisters the renderer from the rendering system
				 */
				~Renderer();
				/**
				 * \brief Registers the renderer to the rendering system 
				 */
				void init() override;
				/**
				 * \brief Creates and initializes the internal renderer
				 * \param data Rendering binding data, used to initialize the renderer
				 */
				virtual void initInternalRenderer(BindingData* data) = 0;

				/**
				* \brief The (shared) material of the renderer
				*/
				Property(Material*, material);
				GetProperty(material) { return _material; }
				SetProperty(material) { _material = value; }

				/**
				 * \brief Gets the internal renderer
				 * \return Returns the internal renderer
				 */
				InternalRenderer* getInternalRenderer() const { return renderer; }
			protected:
				/**
				 * \brief The Filepath of the material
				 */
				std::string materialPath;
				/**
				 * \brief The internal renderer, rendering API specific
				 */
				InternalRenderer* renderer = nullptr;
				/**
				 * \brief The material
				 */
				Material* _material = nullptr;

				bool registered = false;
			};
		}
	}
}