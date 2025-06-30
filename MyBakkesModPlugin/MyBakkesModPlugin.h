#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "version.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class CustomPlayerAnthems: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{
    virtual void onLoad();
    virtual void onUnload();

    // Inherited via PluginSettingsWindow
    void RenderSettings() override;
    std::string GetPluginName() override;
    void SetImGuiContext(uintptr_t ctx) override;

    // Inherited via PluginWindow  
    bool isWindowOpen = false;
    bool shouldBlockInput = false;
    std::string menuTitle = "Custom Player Anthems";

    void Render() override;
    std::string GetMenuName() override;
    std::string GetMenuTitle() override;
    bool ShouldBlockInput() override;
    bool IsActiveOverlay() override;
    void OnOpen() override;
    void OnClose() override;

    // Custom Player Anthems functionality (PRD implementation)
    void OnGoalScored(std::string eventName);
    void OnBallHit(std::string eventName);  // Keep for demo
    
    // Audio functionality
    void PlayCustomAnthem();
    void LoadWAVFile(const std::string& filePath);
    bool IsLocalPlayerGoal();
    void OpenFileDialog();
    
private:
    // Custom Player Anthems settings (PRD requirements)
    bool customAnthemsEnabled = true;
    std::string wavFilePath = "";
    bool fadeOutEnabled = true;
    std::string statusMessage = "Plugin loaded successfully!";
    
    // Demo functionality (keep Hello World counter for demo)
    int goalCounter = 0;
    
    // F-key binding functionality (Deja-Vu pattern)  
    std::string currentKeybind = "None";
    std::shared_ptr<CVarWrapper> keybindCVar;
    
    // Audio system state
    bool audioInitialized = false;
    std::string selectedFileName = "No file selected";
};
