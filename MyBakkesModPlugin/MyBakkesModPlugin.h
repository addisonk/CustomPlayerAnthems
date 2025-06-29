#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "version.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class MyBakkesModPlugin: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{
    virtual void onLoad();
    virtual void onUnload();

    // Inherited via PluginSettingsWindow
    void RenderSettings() override;
    std::string GetPluginName() override;
    void SetImGuiContext(uintptr_t ctx) override;

    // Inherited via PluginWindow
    bool isWindowOpen_ = false;
    bool isMinimized_ = false;
    std::string menuTitle_ = "Hello World Plugin";

    void Render() override;
    std::string GetMenuName() override;
    std::string GetMenuTitle() override;
    bool ShouldBlockInput() override;
    bool IsActiveOverlay() override;
    void OnOpen() override;
    void OnClose() override;

    // Plugin functionality
    void OnBallHit(std::string eventName);
    void OnGoalScored(std::string eventName);
    
private:
    bool pluginEnabled = true;
    float exampleFloat = 1.0f;
    std::string statusMessage = "Plugin loaded successfully!";
    
    // Hello World window state
    bool showHelloWorldWindow = false;
    int helloWorldCounter = 0;
};
