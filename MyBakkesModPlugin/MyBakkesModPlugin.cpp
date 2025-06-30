#include "pch.h"
#include "MyBakkesModPlugin.h"

BAKKESMOD_PLUGIN(CustomPlayerAnthems, "Custom Player Anthems", plugin_version, PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING | PLUGINTYPE_SPECTATOR | PLUGINTYPE_REPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void CustomPlayerAnthems::onLoad()
{
    _globalCvarManager = cvarManager;
    
    // Log plugin load
    LOG("Custom Player Anthems v{} loaded successfully!", plugin_version);
    
    // Register CVars for configuration (PRD requirements)
    cvarManager->registerCvar("helloworld_enabled", "1", "Enable/disable Custom Player Anthems", true, true, 0, true, 1);
    cvarManager->registerCvar("helloworld_show_window", "0", "Show Custom Player Anthems window", true, true, 0, true, 1);
    
    // Register F-key binding CVar (Deja-Vu pattern)
    auto cvar = cvarManager->registerCvar("helloworld_keybind", "None", "F-key to toggle Custom Player Anthems window", true, true);
    keybindCVar = std::make_shared<CVarWrapper>(cvar);
    currentKeybind = keybindCVar->getStringValue();
    
    // Add callback for keybind changes (following Deja-Vu implementation)
    keybindCVar->addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        std::string newBind = cvar.getStringValue();
        
        // Unbind old key if it exists and isn't "None"
        if (!oldValue.empty() && oldValue != "None") {
            cvarManager->executeCommand("unbind " + oldValue, false);
            LOG("Unbound old Custom Player Anthems keybind: " + oldValue);
        }
        
        // Set new keybind if it's not "None"
        if (newBind != "None") {
            cvarManager->setBind(newBind, "togglemenu " + GetMenuName());
            LOG("Set Custom Player Anthems keybind: " + newBind + " -> togglemenu " + GetMenuName());
            statusMessage = "Custom Player Anthems keybind set to " + newBind;
        } else {
            statusMessage = "Custom Player Anthems keybind cleared";
        }
        
        currentKeybind = newBind;
    });
    
    // Register console commands with PERMISSION_ALL to work everywhere
    cvarManager->registerNotifier("helloworld_toggle", [this](std::vector<std::string> args) {
        cvarManager->executeCommand("togglemenu " + GetMenuName());
        LOG("Custom Player Anthems window toggle command executed");
    }, "Toggle Custom Player Anthems window", PERMISSION_ALL);
    
    cvarManager->registerNotifier("helloworld_show", [this](std::vector<std::string> args) {
        if (!this->isWindowOpen) {
            cvarManager->executeCommand("togglemenu " + GetMenuName());
        }
        LOG("Custom Player Anthems window show command executed");
    }, "Show Custom Player Anthems window", PERMISSION_ALL);
    
    cvarManager->registerNotifier("helloworld_hide", [this](std::vector<std::string> args) {
        if (this->isWindowOpen) {
            cvarManager->executeCommand("togglemenu " + GetMenuName());
        }
        LOG("Custom Player Anthems window hide command executed");
    }, "Hide Custom Player Anthems window", PERMISSION_ALL);
    
    // Hook goal scored event (PRD requirement)
    gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventGoalScored", [this](std::string eventName) {
        OnGoalScored(eventName);
    });
    
    gameWrapper->HookEvent("Function TAGame.Car_TA.OnHitBall", [this](std::string eventName) {
        OnBallHit(eventName);
    });
    
    LOG("Custom Player Anthems: Event hooks and commands registered");
    statusMessage = "Custom Player Anthems ready! Use 'helloworld_toggle' to open window or set WAV file.";
}

void CustomPlayerAnthems::onUnload()
{
    LOG("Custom Player Anthems unloaded");
}

void CustomPlayerAnthems::OnBallHit(std::string eventName)
{
    if (!customAnthemsEnabled) return;
    
    // Example: Log when ball is hit
    LOG("Ball hit detected!");
    statusMessage = "Ball hit at " + std::to_string(std::time(nullptr));
}

void CustomPlayerAnthems::OnGoalScored(std::string eventName)
{
    if (!customAnthemsEnabled) return;
    
    LOG("Goal scored! Checking if local player scored...");
    
    // Check if local player scored the goal (PRD requirement)
    if (IsLocalPlayerGoal()) {
        LOG("Local player scored! Playing custom anthem...");
        PlayCustomAnthem();
        statusMessage = "Custom anthem played for your goal!";
    } else {
        LOG("Goal scored by other player, no custom anthem");
        statusMessage = "Goal scored by other player";
    }
    
    goalCounter++; // Increment counter when goal is scored
}

// Custom Player Anthems Audio Implementation (PRD functionality)
void CustomPlayerAnthems::PlayCustomAnthem()
{
    if (wavFilePath.empty()) {
        LOG("No custom anthem file selected");
        statusMessage = "No custom anthem file selected";
        return;
    }
    
    // TODO: Implement actual WAV file playback
    // For now, log the attempt and show status
    LOG("Playing custom anthem: " + wavFilePath);
    statusMessage = "Playing custom anthem: " + selectedFileName;
    
    // TODO: Add fade-out support based on fadeOutEnabled setting
    if (fadeOutEnabled) {
        LOG("Fade-out enabled for custom anthem");
    }
}

void CustomPlayerAnthems::LoadWAVFile(const std::string& filePath)
{
    wavFilePath = filePath;
    // Extract filename from full path for display
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        selectedFileName = filePath.substr(lastSlash + 1);
    } else {
        selectedFileName = filePath;
    }
    
    LOG("Loaded WAV file: " + filePath);
    statusMessage = "Loaded custom anthem: " + selectedFileName;
}

bool CustomPlayerAnthems::IsLocalPlayerGoal()
{
    // TODO: Implement proper local player goal detection
    // This requires checking the game state to determine if the local player scored
    // For now, return true for testing (will be refined later)
    
    if (!gameWrapper->IsInGame()) {
        return false;
    }
    
    // Basic implementation - will need refinement for accurate detection
    auto server = gameWrapper->GetOnlineGame();
    if (!server.IsNull()) {
        // In online game - need to check if local player scored
        // This is a simplified version - actual implementation needs more game state checking
        return true; // Placeholder for testing
    }
    
    // In freeplay/training, assume local player always scores
    return true;
}

void CustomPlayerAnthems::OpenFileDialog()
{
    // TODO: Implement file dialog for WAV file selection
    // This would typically use Windows file dialog or similar cross-platform solution
    LOG("File dialog would open here for WAV file selection");
    statusMessage = "File dialog functionality to be implemented";
}

// PluginSettingsWindow Implementation
void CustomPlayerAnthems::RenderSettings()
{
    // Custom Player Anthems Header (PRD Implementation)
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Custom Player Anthems");
    ImGui::Separator();
    
    ImGui::Text("Play custom WAV files when YOU score goals!");
    ImGui::Text("Works in Online Multiplayer, Freeplay, Custom Training, and Private Matches.");
    
    ImGui::Spacing();
    
    // PRD Settings Implementation
    ImGui::Text("Custom Player Anthems Settings");
    ImGui::Separator();
    
    // PRD Requirement 1: [✓] Enable Custom Anthems
    if (ImGui::Checkbox("Enable Custom Anthems", &customAnthemsEnabled)) {
        cvarManager->getCvar("helloworld_enabled").setValue(customAnthemsEnabled);
        statusMessage = customAnthemsEnabled ? "Custom anthems enabled" : "Custom anthems disabled";
        LOG("Custom anthems " + std::string(customAnthemsEnabled ? "enabled" : "disabled"));
    }
    
    ImGui::Spacing();
    
    // PRD Requirement 2: [Browse Button] WAV File Selector
    ImGui::Text("Selected WAV File:");
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", selectedFileName.c_str());
    
    if (ImGui::Button("Browse for WAV File")) {
        OpenFileDialog();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Clear Selection")) {
        wavFilePath = "";
        selectedFileName = "No file selected";
        statusMessage = "WAV file selection cleared";
        LOG("WAV file selection cleared");
    }
    
    ImGui::Spacing();
    
    // PRD Requirement 3: [✓] Fade Out
    if (ImGui::Checkbox("Fade Out", &fadeOutEnabled)) {
        statusMessage = fadeOutEnabled ? "Fade out enabled" : "Fade out disabled";
        LOG("Fade out " + std::string(fadeOutEnabled ? "enabled" : "disabled"));
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "(anthem will fade out at the end)");
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Goal Counter (keep for demo/testing)
    ImGui::Text("Goal Counter: %d", goalCounter);
    
    if (ImGui::Button("Reset Counter"))
    {
        goalCounter = 0;
        LOG("Goal counter reset");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Test Custom Anthem")) {
        if (customAnthemsEnabled) {
            PlayCustomAnthem();
        } else {
            statusMessage = "Enable custom anthems first!";
        }
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // F-key binding settings (Deja-Vu pattern)
    ImGui::Text("Quick Access F-Key Binding");
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
    if (ImGui::Combo("Custom Anthems Keybind", &currentKeybindIndex, keybindOptions, IM_ARRAYSIZE(keybindOptions))) {
        std::string newKeybind = keybindOptions[currentKeybindIndex];
        if (keybindCVar) {
            keybindCVar->setValue(newKeybind);
        }
    }
    
    ImGui::Text("Current Keybind: %s", currentKeybind.c_str());
    if (currentKeybind != "None") {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Press %s to toggle Custom Anthems window!", currentKeybind.c_str());
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Standalone window controls
    ImGui::Text("Standalone Window Controls");
    ImGui::Text("Window Status: %s", this->isWindowOpen ? "OPEN" : "CLOSED");
    
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
    ImGui::Text("Custom Anthems: %s", customAnthemsEnabled ? "Enabled" : "Disabled");
    ImGui::Text("Fade Out: %s", fadeOutEnabled ? "Enabled" : "Disabled");
    
    // Instructions
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "How to Use Custom Player Anthems:");
    ImGui::BulletText("1. Enable Custom Anthems checkbox above");
    ImGui::BulletText("2. Click 'Browse for WAV File' to select your custom anthem");
    ImGui::BulletText("3. Optionally enable Fade Out for smoother endings");
    ImGui::BulletText("4. Your anthem will play when YOU score goals!");
    ImGui::BulletText("Works in: Online Multiplayer, Freeplay, Custom Training, Private Matches");
    ImGui::BulletText("Set F-key binding for quick access to this window!");
    ImGui::BulletText("Console commands: 'togglemenu helloworld', 'helloworld_toggle'");
}

std::string CustomPlayerAnthems::GetPluginName()
{
    return "Custom Player Anthems";
}

void CustomPlayerAnthems::SetImGuiContext(uintptr_t ctx)
{
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// PluginWindow Implementation
void CustomPlayerAnthems::Render()
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
    
    // Custom Player Anthems content
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Custom Player Anthems");
    ImGui::Separator();
    
    // Display information
    ImGui::Text("Play custom WAV files when YOU score goals!");
    ImGui::Text("This window is accessible everywhere in Rocket League.");
    
    ImGui::Spacing();
    
    // Custom Player Anthems controls
    ImGui::Text("Selected WAV File: %s", selectedFileName.c_str());
    ImGui::Text("Goal Counter: %d", goalCounter);
    
    if (ImGui::Button("Browse for WAV File")) {
        OpenFileDialog();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Test Custom Anthem")) {
        if (customAnthemsEnabled) {
            PlayCustomAnthem();
        } else {
            statusMessage = "Enable custom anthems first!";
        }
    }
    
    if (ImGui::Button("Reset Counter"))
    {
        goalCounter = 0;
        LOG("Goal counter reset");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Status information
    ImGui::Text("Custom Anthems: %s", customAnthemsEnabled ? "Enabled" : "Disabled");
    ImGui::Text("Fade Out: %s", fadeOutEnabled ? "Enabled" : "Disabled");
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
    ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "How to Use Custom Player Anthems:");
    ImGui::BulletText("Set F-key binding in BakkesMod Settings > Plugins > Custom Player Anthems");
    ImGui::BulletText("Or use console: 'togglemenu helloworld'");
    ImGui::BulletText("Browse and select a WAV file for your custom anthem");
    ImGui::BulletText("Your anthem plays when YOU score goals!");
    
    // Update input blocking
    this->shouldBlockInput = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
    
    ImGui::End();
}

std::string CustomPlayerAnthems::GetMenuName()
{
    return "customplayeranthems";
}

std::string CustomPlayerAnthems::GetMenuTitle()
{
    return this->menuTitle;
}

bool CustomPlayerAnthems::ShouldBlockInput()
{
    return this->shouldBlockInput;
}

bool CustomPlayerAnthems::IsActiveOverlay()
{
    return this->isWindowOpen;
}

void CustomPlayerAnthems::OnOpen()
{
    this->isWindowOpen = true;
    LOG("Custom Player Anthems window opened");
}

void CustomPlayerAnthems::OnClose()
{
    this->isWindowOpen = false;
    LOG("Custom Player Anthems window closed");
}

// Template verification build
// Hello World Plugin - Created for demonstration
// GitHub Workflow Integration Test - Sun Jun 29 10:43:56 PDT 2025
