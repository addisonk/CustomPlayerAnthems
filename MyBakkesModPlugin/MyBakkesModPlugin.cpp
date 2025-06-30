#include "pch.h"
#include "MyBakkesModPlugin.h"

BAKKESMOD_PLUGIN(MyBakkesModPlugin, "Hello World Plugin", plugin_version, PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING | PLUGINTYPE_SPECTATOR | PLUGINTYPE_REPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void MyBakkesModPlugin::onLoad()
{
    _globalCvarManager = cvarManager;
    
    // Log plugin load
    LOG("Hello World Plugin loaded successfully!");
    
    // Register CVars for configuration
    cvarManager->registerCvar("helloworld_enabled", "1", "Enable/disable Hello World Plugin", true, true, 0, true, 1);
    cvarManager->registerCvar("helloworld_show_window", "0", "Show Hello World window", true, true, 0, true, 1);
    
    // Register F-key binding CVar (Deja-Vu pattern)
    keybindCVar = cvarManager->registerCvar("helloworld_keybind", "None", "F-key to toggle Hello World window", true, true);
    currentKeybind = keybindCVar.getStringValue();
    
    // Add callback for keybind changes (following Deja-Vu implementation)
    keybindCVar.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        std::string newBind = cvar.getStringValue();
        
        // Unbind old key if it exists and isn't "None"
        if (!oldValue.empty() && oldValue != "None") {
            cvarManager->executeCommand("unbind " + oldValue, false);
            LOG("Unbound old Hello World keybind: " + oldValue);
        }
        
        // Set new keybind if it's not "None"
        if (newBind != "None") {
            cvarManager->setBind(newBind, "togglemenu " + GetMenuName());
            LOG("Set Hello World keybind: " + newBind + " -> togglemenu " + GetMenuName());
            statusMessage = "Hello World keybind set to " + newBind;
        } else {
            statusMessage = "Hello World keybind cleared";
        }
        
        currentKeybind = newBind;
    });
    
    // Register console commands with PERMISSION_ALL to work everywhere
    cvarManager->registerNotifier("helloworld_toggle", [this](std::vector<std::string> args) {
        cvarManager->executeCommand("togglemenu " + GetMenuName());
        LOG("Hello World window toggle command executed");
    }, "Toggle Hello World window", PERMISSION_ALL);
    
    cvarManager->registerNotifier("helloworld_show", [this](std::vector<std::string> args) {
        if (!this->isWindowOpen) {
            cvarManager->executeCommand("togglemenu " + GetMenuName());
        }
        LOG("Hello World window show command executed");
    }, "Show Hello World window", PERMISSION_ALL);
    
    cvarManager->registerNotifier("helloworld_hide", [this](std::vector<std::string> args) {
        if (this->isWindowOpen) {
            cvarManager->executeCommand("togglemenu " + GetMenuName());
        }
        LOG("Hello World window hide command executed");
    }, "Hide Hello World window", PERMISSION_ALL);
    
    // Hook game events (optional - for demonstration)
    gameWrapper->HookEvent("Function TAGame.Ball_TA.OnHitGoal", [this](std::string eventName) {
        OnGoalScored(eventName);
    });
    
    gameWrapper->HookEvent("Function TAGame.Car_TA.OnHitBall", [this](std::string eventName) {
        OnBallHit(eventName);
    });
    
    LOG("Hello World Plugin: Event hooks and commands registered");
    statusMessage = "Hello World Plugin ready! Use 'helloworld_toggle' to open the window.";
}

void MyBakkesModPlugin::onUnload()
{
    LOG("Hello World Plugin unloaded");
}

void MyBakkesModPlugin::OnBallHit(std::string eventName)
{
    if (!pluginEnabled) return;
    
    // Example: Log when ball is hit
    LOG("Ball hit detected!");
    statusMessage = "Ball hit at " + std::to_string(std::time(nullptr));
}

void MyBakkesModPlugin::OnGoalScored(std::string eventName)
{
    if (!pluginEnabled) return;
    
    // Example: Log when goal is scored
    LOG("Goal scored!");
    statusMessage = "Goal scored at " + std::to_string(std::time(nullptr));
    helloWorldCounter++; // Increment counter when goal is scored
}

// PluginSettingsWindow Implementation
void MyBakkesModPlugin::RenderSettings()
{
    // Main Hello World Display
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Hello, World!");
    ImGui::Separator();
    
    ImGui::Text("Welcome to the Hello World Plugin!");
    ImGui::Text("This is your Hello World window, accessible through BakkesMod settings.");
    
    ImGui::Spacing();
    
    // Interactive Hello World elements
    ImGui::Text("Goal Counter: %d", helloWorldCounter);
    
    if (ImGui::Button("Reset Counter"))
    {
        helloWorldCounter = 0;
        LOG("Hello World counter reset");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Increment Counter"))
    {
        helloWorldCounter++;
        LOG("Hello World counter incremented to " + std::to_string(helloWorldCounter));
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Plugin Settings
    ImGui::Text("Plugin Settings");
    ImGui::Separator();
    
    // Enable/disable plugin
    if (ImGui::Checkbox("Enable Plugin", &pluginEnabled)) {
        cvarManager->getCvar("helloworld_enabled").setValue(pluginEnabled);
    }
    
    // F-key binding settings (Deja-Vu pattern)
    ImGui::Text("F-Key Binding");
    ImGui::Separator();
    
    // Available F-keys (from Deja-Vu implementation)
    const char* keybindOptions[] = { "None", "F1", "F3", "F4", "F5", "F7", "F8", "F9", "F10", "F11", "F12" };
    int currentKeybindIndex = 0;
    
    // Find current selection index
    for (int i = 0; i < IM_ARRAYSIZE(keybindOptions); i++) {
        if (currentKeybind == keybindOptions[i]) {
            currentKeybindIndex = i;
            break;
        }
    }
    
    // F-key selection dropdown
    if (ImGui::Combo("Hello World Keybind", &currentKeybindIndex, keybindOptions, IM_ARRAYSIZE(keybindOptions))) {
        std::string newKeybind = keybindOptions[currentKeybindIndex];
        keybindCVar.setValue(newKeybind);
    }
    
    ImGui::Text("Current Keybind: %s", currentKeybind.c_str());
    if (currentKeybind != "None") {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Press %s to toggle Hello World window!", currentKeybind.c_str());
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Standalone window controls
    ImGui::Text("Window Controls");
    ImGui::Text("Standalone Window Status: %s", this->isWindowOpen ? "OPEN" : "CLOSED");
    
    // Control buttons for standalone window
    ImGui::Spacing();
    if (ImGui::Button("Open Standalone Window")) {
        cvarManager->executeCommand("togglemenu " + GetMenuName());
    }
    ImGui::SameLine();
    if (ImGui::Button("Use Console Command")) {
        cvarManager->executeCommand("helloworld_toggle");
    }
    
    // Status display
    ImGui::Separator();
    ImGui::Text("Status: %s", statusMessage.c_str());
    ImGui::Text("Plugin Version: %s", plugin_version);
    ImGui::Text("Plugin Status: %s", pluginEnabled ? "Enabled" : "Disabled");
    
    // Instructions
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "How to Use:");
    ImGui::BulletText("This settings window shows your Hello World functionality");
    ImGui::BulletText("Set an F-key above for quick access to standalone window!");
    ImGui::BulletText("Or use console: 'togglemenu helloworld', 'helloworld_toggle'");
    ImGui::BulletText("The goal counter auto-increments when goals are scored!");
    ImGui::BulletText("Works in all game modes: Freeplay, Training, Spectator, Replay");
}

std::string MyBakkesModPlugin::GetPluginName()
{
    return "Hello World Plugin";
}

void MyBakkesModPlugin::SetImGuiContext(uintptr_t ctx)
{
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// PluginWindow Implementation
void MyBakkesModPlugin::Render()
{
    // Follow Deja-Vu pattern for window management
    if (!this->isWindowOpen) {
        cvarManager->executeCommand("togglemenu " + GetMenuName());
        return;
    }
    
    // Set window flags for a nice Hello World window
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
    
    // Create the Hello World window using GetMenuTitle() and isWindowOpen reference
    if (!ImGui::Begin(GetMenuTitle().c_str(), &this->isWindowOpen, windowFlags))
    {
        // Early out if the window is collapsed, as an optimization
        this->shouldBlockInput = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        ImGui::End();
        return;
    }
    
    // Main Hello World content
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Hello, World!");
    ImGui::Separator();
    
    // Display some information
    ImGui::Text("Welcome to the Hello World Plugin!");
    ImGui::Text("This standalone window is accessible everywhere in Rocket League.");
    
    ImGui::Spacing();
    
    // Interactive elements
    ImGui::Text("Goal Counter: %d", helloWorldCounter);
    
    if (ImGui::Button("Reset Counter"))
    {
        helloWorldCounter = 0;
        LOG("Hello World counter reset");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Increment Counter"))
    {
        helloWorldCounter++;
        LOG("Hello World counter incremented to " + std::to_string(helloWorldCounter));
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Status information
    ImGui::Text("Plugin Status: %s", pluginEnabled ? "Enabled" : "Disabled");
    ImGui::Text("Current Status: %s", statusMessage.c_str());
    ImGui::Text("Plugin Version: %s", plugin_version);
    
    // F-key binding info
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Keybind Info:");
    if (currentKeybind != "None") {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Bound to: %s", currentKeybind.c_str());
        ImGui::Text("Press %s to toggle this window!", currentKeybind.c_str());
    } else {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "No F-key bound");
        ImGui::Text("Set one in BakkesMod Settings > Plugins > Hello World Plugin");
    }
    
    // Instructions
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "How to Use:");
    ImGui::BulletText("Set F-key binding in BakkesMod Settings > Plugins");
    ImGui::BulletText("Or use console: 'togglemenu helloworld'");
    ImGui::BulletText("Goal counter auto-increments when goals are scored!");
    
    // Update input blocking
    this->shouldBlockInput = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
    
    ImGui::End();
}

std::string MyBakkesModPlugin::GetMenuName()
{
    return "helloworld";
}

std::string MyBakkesModPlugin::GetMenuTitle()
{
    return this->menuTitle;
}

bool MyBakkesModPlugin::ShouldBlockInput()
{
    return this->shouldBlockInput;
}

bool MyBakkesModPlugin::IsActiveOverlay()
{
    return this->isWindowOpen;
}

void MyBakkesModPlugin::OnOpen()
{
    this->isWindowOpen = true;
    LOG("Hello World window opened");
}

void MyBakkesModPlugin::OnClose()
{
    this->isWindowOpen = false;
    LOG("Hello World window closed");
}

// Template verification build
// Hello World Plugin - Created for demonstration
// GitHub Workflow Integration Test - Sun Jun 29 10:43:56 PDT 2025
