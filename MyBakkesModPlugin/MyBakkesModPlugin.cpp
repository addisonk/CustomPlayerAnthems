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
    
    // Register console commands with PERMISSION_ALL to work everywhere
    cvarManager->registerNotifier("helloworld_toggle", [this](std::vector<std::string> args) {
        showHelloWorldWindow = !showHelloWorldWindow;
        LOG("Hello World window toggled: " + std::string(showHelloWorldWindow ? "ON" : "OFF"));
    }, "Toggle Hello World window", PERMISSION_ALL);
    
    cvarManager->registerNotifier("helloworld_show", [this](std::vector<std::string> args) {
        showHelloWorldWindow = true;
        LOG("Hello World window shown");
    }, "Show Hello World window", PERMISSION_ALL);
    
    cvarManager->registerNotifier("helloworld_hide", [this](std::vector<std::string> args) {
        showHelloWorldWindow = false;
        LOG("Hello World window hidden");
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
    
    // Toggle standalone window (if it works)
    if (ImGui::Checkbox("Show Standalone Window", &showHelloWorldWindow)) {
        cvarManager->getCvar("helloworld_show_window").setValue(showHelloWorldWindow);
    }
    
    // Control buttons for standalone window
    ImGui::Spacing();
    if (ImGui::Button("Test Standalone Window")) {
        cvarManager->executeCommand("helloworld_show");
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide Standalone Window")) {
        cvarManager->executeCommand("helloworld_hide");
    }
    
    // Status display
    ImGui::Separator();
    ImGui::Text("Status: %s", statusMessage.c_str());
    ImGui::Text("Plugin Version: %s", plugin_version);
    ImGui::Text("Plugin Status: %s", pluginEnabled ? "Enabled" : "Disabled");
    
    // Instructions
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "How to Use:");
    ImGui::BulletText("This settings window IS your Hello World window!");
    ImGui::BulletText("Access it anytime via BakkesMod Settings > Plugins > Hello World Plugin");
    ImGui::BulletText("Try console commands: 'helloworld_toggle', 'helloworld_show', 'helloworld_hide'");
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
    if (!showHelloWorldWindow) return;
    
    // Set window flags for a nice Hello World window
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
    
    // Create the Hello World window
    if (ImGui::Begin("Hello World!", &showHelloWorldWindow, windowFlags))
    {
        // Main Hello World content
        ImGui::Text("Hello, World!");
        ImGui::Separator();
        
        // Display some information
        ImGui::Text("Welcome to the Hello World Plugin!");
        ImGui::Text("This window is accessible everywhere in Rocket League.");
        
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
        
        // Close button
        ImGui::Spacing();
        if (ImGui::Button("Close Window"))
        {
            showHelloWorldWindow = false;
        }
    }
    ImGui::End();
}

std::string MyBakkesModPlugin::GetMenuName()
{
    return "helloworld";
}

std::string MyBakkesModPlugin::GetMenuTitle()
{
    return menuTitle_;
}

bool MyBakkesModPlugin::ShouldBlockInput()
{
    return false; // Don't block input so the game remains playable
}

bool MyBakkesModPlugin::IsActiveOverlay()
{
    return showHelloWorldWindow;
}

void MyBakkesModPlugin::OnOpen()
{
    isWindowOpen_ = true;
    showHelloWorldWindow = true;
}

void MyBakkesModPlugin::OnClose()
{
    isWindowOpen_ = false;
    showHelloWorldWindow = false;
}

// Template verification build
// Hello World Plugin - Created for demonstration
// GitHub Workflow Integration Test - Sun Jun 29 10:43:56 PDT 2025
