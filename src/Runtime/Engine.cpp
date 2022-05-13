#include "Engine.h"

#include "Function/Event/EventSystem.h"

#include "Function/Scene/Scene.h"

#include "Function/Render/Renderer.h"

#include "Core/Base/macro.h"
#include <thread>
namespace Soarscape
{
    void Engine::startEngine(int argc, char* argv[])
    {
        LOG_INFO("Start editor engine");
        m_isRunning = true;
    }
    void Engine::logicalTick()
    {
        PublicSingleton<EventSystem>::getInstance().processEvents();
        m_UISurface->tick();
    }
    void Engine::renderTick(uint32_t defaultFramebufferid)
    { 
        PublicSingleton<Scene>::getInstance().renderTick();
        PublicSingleton<Renderer>::getInstance().tick(defaultFramebufferid);
    }

    void Engine::renderInitialize(int x, int y, int width, int height)
    {
        PublicSingleton<Renderer>::getInstance().initialize(x, y, width, height);
        PublicSingleton<Scene>::getInstance().initialize();
    }
    void Engine::logicalInitialize()
    {
    }

    void Engine::shutdownEngine()
    {
        LOG_INFO("Shutdown editor engine");

        // Shut down event system
        PublicSingleton<EventSystem>::getInstance().shutdown();

        // waiting for other thread to release
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        m_isRunning = false;
    }
    
} // namespace SoaScape
