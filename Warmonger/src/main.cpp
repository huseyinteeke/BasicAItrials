/*
* BLG 252E: Assignment 3 - Raylib GUI Version
* main.cpp
*
* Created on: May 26th, 2026
*     Author: Antigravity AI Pair Programmer
*/

#include "Faction.hpp"
#include "Orcs.hpp"
#include "Dwarves.hpp"
#include "Elves.hpp"
#include "Merchant.hpp"
#include "raylib.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;

// --- DYNAMIC AUDIO SYNTHESIS UTILITIES ---
// Synthesize sound waves directly to RAM so we do not need external wave files.
Sound CreateCoinSound()
{
    int sampleRate = 44100;
    int samplesCount = sampleRate * 0.25f; // 0.25s
    float *data = (float *)malloc(samplesCount * sizeof(float));
    for (int i = 0; i < samplesCount; i++)
    {
        float t = (float)i / sampleRate;
        // Arpeggio chime: 987Hz (B5) up to 1975Hz (B6)
        float freq = 987.77f + 987.77f * (t / 0.25f);
        float amplitude = expf(-t * 12.0f);
        data[i] = sinf(2.0f * PI * freq * t) * amplitude * 0.4f;
    }
    Wave wave;
    wave.frameCount = samplesCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 32;
    wave.channels = 1;
    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

Sound CreateHitSound()
{
    int sampleRate = 44100;
    int samplesCount = sampleRate * 0.2f; // 0.2s
    float *data = (float *)malloc(samplesCount * sizeof(float));
    for (int i = 0; i < samplesCount; i++)
    {
        float t = (float)i / sampleRate;
        float noise = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        float sweepFreq = 180.0f - 130.0f * (t / 0.2f);
        float waveTone = sinf(2.0f * PI * sweepFreq * t);
        float amplitude = expf(-t * 18.0f);
        data[i] = (noise * 0.4f + waveTone * 0.6f) * amplitude * 0.4f;
    }
    Wave wave;
    wave.frameCount = samplesCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 32;
    wave.channels = 1;
    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

Sound CreateClickSound()
{
    int sampleRate = 44100;
    int samplesCount = sampleRate * 0.05f; // 0.05s
    float *data = (float *)malloc(samplesCount * sizeof(float));
    for (int i = 0; i < samplesCount; i++)
    {
        float t = (float)i / sampleRate;
        float amplitude = expf(-t * 70.0f);
        data[i] = sinf(2.0f * PI * 1200.0f * t) * amplitude * 0.25f;
    }
    Wave wave;
    wave.frameCount = samplesCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 32;
    wave.channels = 1;
    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

Sound CreateVictorySound()
{
    int sampleRate = 44100;
    int samplesCount = sampleRate * 0.8f; // 0.8s
    float *data = (float *)malloc(samplesCount * sizeof(float));
    for (int i = 0; i < samplesCount; i++)
    {
        float t = (float)i / sampleRate;
        // Majestic triad: C4 (261.63Hz), E4 (329.63Hz), G4 (392.00Hz)
        float tone = sinf(2.0f * PI * 261.63f * t) + sinf(2.0f * PI * 329.63f * t) + sinf(2.0f * PI * 392.00f * t);
        if (t > 0.2f) {
            tone += sinf(2.0f * PI * 523.25f * t); // High C5
        }
        float amplitude = expf(-t * 4.0f);
        data[i] = (tone / 4.0f) * amplitude * 0.35f;
    }
    Wave wave;
    wave.frameCount = samplesCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 32;
    wave.channels = 1;
    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

Sound CreateFartSound()
{
    int sampleRate = 44100;
    float duration = 0.5f; // 0.5s duration
    int samplesCount = sampleRate * duration;
    float *data = (float *)malloc(samplesCount * sizeof(float));
    for (int i = 0; i < samplesCount; i++)
    {
        float t = (float)i / sampleRate;
        // Frequency sliding down: starts at 140Hz, drops to 55Hz
        float baseFreq = 140.0f - 85.0f * (t / duration);
        // Vibrato flappy LFO at 22Hz
        float lfo = sinf(2.0f * PI * 22.0f * t);
        float modulatedFreq = baseFreq + lfo * 30.0f;
        float angle = 2.0f * PI * modulatedFreq * t;
        float tone = (sinf(angle) + 0.5f * sinf(2.0f * angle) + 0.25f * sinf(3.0f * angle));
        // Add white noise for air pressure
        float noise = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        float noiseVol = 0.18f * (1.0f - t / duration) * (0.5f + 0.5f * lfo);
        float combined = tone * 0.6f + noise * noiseVol;
        float env = 1.0f;
        if (t < 0.05f) {
            env = t / 0.05f;
        } else {
            env = expf(-(t - 0.05f) * 6.5f);
        }
        data[i] = combined * env * 0.5f;
    }
    Wave wave;
    wave.frameCount = samplesCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 32;
    wave.channels = 1;
    wave.data = data;
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}

// --- VISUAL NOTIFICATIONS ---
struct FloatingNotification
{
    Vector2 position;
    string text;
    Color color;
    float velocityY;
    float alpha;
    float lifetime;
};

// Colors representing the theme
const Color COLOR_BG = { 13, 13, 21, 255 };          // Dark space background
const Color COLOR_CARD_BG = { 26, 26, 38, 240 };     // Sleek glass card background
const Color COLOR_GOLD = { 251, 192, 45, 255 };       // Warm shiny gold
const Color COLOR_ORCS = { 211, 47, 47, 255 };        // Crimson Red
const Color COLOR_DWARVES = { 33, 150, 243, 255 };    // Deep Blue
const Color COLOR_ELVES = { 76, 175, 80, 255 };       // Leaf Green
const Color COLOR_TEXT_P = { 255, 255, 255, 255 };    // Primary white text
const Color COLOR_TEXT_S = { 158, 158, 178, 255 };    // Secondary muted gray text
const Color COLOR_PANEL_BORDER = { 58, 58, 78, 255 }; // Clean metallic border
const Color COLOR_BUTTON_N = { 46, 46, 68, 255 };     // Button Normal
const Color COLOR_BUTTON_H = { 76, 76, 108, 255 };    // Button Hover
const Color COLOR_BUTTON_A = { 106, 106, 148, 255 };  // Button Active
const Color COLOR_GREEN_GLOW = { 129, 199, 132, 255 }; // Light green
const Color COLOR_RED_GLOW = { 229, 115, 115, 255 };   // Light red

// Helper function to render a custom rounded border card
void DrawGlassCard(Rectangle rect, Color borderColor)
{
    DrawRectangleRounded(rect, 0.08f, 4, COLOR_CARD_BG);
    DrawRectangleRoundedLines(rect, 0.08f, 4, borderColor);
}

// Helper to draw a modern glowing button and return if clicked
bool DrawModernButton(Rectangle rect, string text, Color accentColor, Sound clickSound)
{
    Vector2 mousePos = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePos, rect);
    bool clicked = false;

    Color buttonColor = COLOR_BUTTON_N;
    if (hovered)
    {
        buttonColor = COLOR_BUTTON_H;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            buttonColor = COLOR_BUTTON_A;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(clickSound);
            clicked = true;
        }
    }

    DrawRectangleRounded(rect, 0.2f, 4, buttonColor);
    DrawRectangleRoundedLines(rect, 0.2f, 4, accentColor);

    int fontSize = 18;
    int textWidth = MeasureText(text.c_str(), fontSize);
    DrawText(text.c_str(), rect.x + (rect.width - textWidth) / 2.0f, rect.y + (rect.height - fontSize) / 2.0f, fontSize, COLOR_TEXT_P);

    return clicked;
}

int main()
{
    // Window configuration
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    // Set config flags for Multi-Sampling (Anti-Aliasing)
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "THE WARMONGER: A NEW DIMENSION");
    SetTargetFPS(60);

    InitAudioDevice();

    // Synthesize sounds
    Sound coinSound = CreateCoinSound();
    Sound hitSound = CreateHitSound();
    Sound clickSound = CreateClickSound();
    Sound victorySound = CreateVictorySound();
    Sound fartSound = CreateFartSound();

    // Setup variables for customization
    int startUnits[3] = { 40, 50, 30 };
    int startAttack[3] = { 20, 40, 20 };
    int startHP[3] = { 200, 300, 120 };
    int startRegen[3] = { 10, 5, 15 };
    int gameState = 0; // 0: SETUP SCREEN, 1: ACTIVE GAMEPLAY
    
    // Game pointers (re-created on setup launch)
    Orcs *orcs = nullptr;
    Dwarves *dwarves = nullptr;
    Elves *elves = nullptr;
    Merchant *merchant = nullptr;

    int day = 1;
    int numOfFactionsAlive = 3;
    bool gameOver = false;

    // Interface interactive controllers
    int sellWeaponsAmount[3] = { 0, 0, 0 };
    int sellArmorsAmount[3] = { 0, 0, 0 };

    // Floating text notifications
    vector<FloatingNotification> notifications;

    // Log messages
    vector<string> battleLogs;

    // Shake & damage flash timers for cards
    float shakeIntensity[3] = { 0.0f, 0.0f, 0.0f };
    float flashIntensity[3] = { 0.0f, 0.0f, 0.0f };

    // Initialize/Reset function
    auto ResetGame = [&]() {
        if (orcs) { delete orcs; orcs = nullptr; }
        if (dwarves) { delete dwarves; dwarves = nullptr; }
        if (elves) { delete elves; elves = nullptr; }
        if (merchant) { delete merchant; merchant = nullptr; }

        day = 1;
        numOfFactionsAlive = 3;
        gameOver = false;
        gameState = 0; // Return to setup

        for (int i = 0; i < 3; i++)
        {
            sellWeaponsAmount[i] = 0;
            sellArmorsAmount[i] = 0;
            shakeIntensity[i] = 0.0f;
            flashIntensity[i] = 0.0f;
        }

        notifications.clear();
        battleLogs.clear();
        battleLogs.push_back("⚔️ Welcome to the Battle of the Three Armies!");
        battleLogs.push_back("💰 Act as the Merchant to sell weapons and armor to finance your vault.");
    };

    ResetGame();

    // Deploy campaign launcher
    auto LaunchCampaign = [&]() {
        if (orcs) delete orcs;
        if (dwarves) delete dwarves;
        if (elves) delete elves;
        if (merchant) delete merchant;

        orcs = new Orcs("Orcs", startUnits[0], startAttack[0], startHP[0], startRegen[0]);
        dwarves = new Dwarves("Dwarves", startUnits[1], startAttack[1], startHP[1], startRegen[1]);
        elves = new Elves("Elves", startUnits[2], startAttack[2], startHP[2], startRegen[2]);

        orcs->AssignEnemies(elves, dwarves);
        dwarves->AssignEnemies(orcs, elves);
        elves->AssignEnemies(orcs, dwarves);

        merchant = new Merchant(10, 10);
        merchant->AssignFactions(orcs, dwarves, elves);

        day = 1;
        numOfFactionsAlive = 3;
        gameOver = false;
        gameState = 1; // Deploy gameplay

        battleLogs.push_back("🌱 Day 1 begins! Factions deployed with custom setups.");
    };

    // Floating text helper
    auto SpawnNotification = [&](Vector2 pos, string text, Color col) {
        FloatingNotification fn;
        fn.position = pos;
        fn.text = text;
        fn.color = col;
        fn.velocityY = -1.5f;
        fn.alpha = 1.0f;
        fn.lifetime = 1.2f;
        notifications.push_back(fn);
    };

    // Main window loop
    while (!WindowShouldClose())
    {
        // --- 1. UPDATE STAGE ---
        float dt = GetFrameTime();

        // Update floating notifications
        for (auto it = notifications.begin(); it != notifications.end();)
        {
            it->position.y += it->velocityY;
            it->lifetime -= dt;
            it->alpha = it->lifetime / 1.2f;
            if (it->lifetime <= 0)
            {
                it = notifications.erase(it);
            }
            else
            {
                it++;
            }
        }

        // Decay shake and damage flash
        for (int i = 0; i < 3; i++)
        {
            if (shakeIntensity[i] > 0.0f)
            {
                shakeIntensity[i] -= dt * 2.0f;
                if (shakeIntensity[i] < 0.0f) shakeIntensity[i] = 0.0f;
            }
            if (flashIntensity[i] > 0.0f)
            {
                flashIntensity[i] -= dt * 2.0f;
                if (flashIntensity[i] < 0.0f) flashIntensity[i] = 0.0f;
            }
        }

        // --- 2. DRAW STAGE ---
        if (gameState == 0)
        {
            // ================= SETUP SCREEN DRAWING =================
            BeginDrawing();
            ClearBackground(COLOR_BG);

            // A. Setup Header Panel
            DrawRectangle(0, 0, screenWidth, 80, COLOR_CARD_BG);
            DrawLine(0, 80, screenWidth, 80, COLOR_PANEL_BORDER);
            DrawText("⚔️ THE WARMONGER: SETUP CONFIGURATOR", 30, 26, 22, COLOR_GOLD);
            DrawText("Configure initial faction parameters before launching the war.", screenWidth - MeasureText("Configure initial faction parameters before launching the war.", 14) - 30, 31, 14, COLOR_TEXT_S);

            // B. Three Columns for customizable Factions
            string factionsName[3] = { "Orcs", "Dwarves", "Elves" };
            Color factionsColor[3] = { COLOR_ORCS, COLOR_DWARVES, COLOR_ELVES };

            for (int i = 0; i < 3; i++)
            {
                float posX = 70.0f + (i * 370.0f);
                float posY = 120.0f;
                float cardW = 320.0f;
                float cardH = 500.0f;

                DrawGlassCard({ posX, posY, cardW, cardH }, factionsColor[i]);

                // Faction Banner
                DrawRectangleRounded({ posX + 10, posY + 10, cardW - 20, 50 }, 0.1f, 4, factionsColor[i]);
                string setupTitle = factionsName[i] + " SETUP";
                DrawText(setupTitle.c_str(), posX + (cardW - MeasureText(setupTitle.c_str(), 18)) / 2.0f, posY + 25, 18, COLOR_TEXT_P);

                // Config Rows
                int rowY = posY + 90;

                auto DrawConfigRow = [&](string label, int &val, int minVal, int maxVal, int step, int offset) {
                    DrawText(label.c_str(), posX + 25, rowY + offset, 15, COLOR_TEXT_S);
                    
                    string valStr = to_string(val);
                    DrawText(valStr.c_str(), posX + 185, rowY + offset, 16, COLOR_TEXT_P);

                    // Decrement Button
                    if (DrawModernButton({ posX + 135, (float)(rowY + offset - 4), 35, 26 }, "-", COLOR_PANEL_BORDER, clickSound))
                    {
                        if (val > minVal) val -= step;
                        if (val < minVal) val = minVal;
                    }

                    // Increment Button
                    if (DrawModernButton({ posX + 240, (float)(rowY + offset - 4), 35, 26 }, "+", COLOR_PANEL_BORDER, clickSound))
                    {
                        if (val < maxVal) val += step;
                    }
                };

                DrawConfigRow("Units Count:", startUnits[i], 5, 100, 5, 0);
                DrawConfigRow("Attack Power:", startAttack[i], 5, 100, 5, 80);
                DrawConfigRow("Health/Unit:", startHP[i], 10, 1000, 20, 160);
                DrawConfigRow("Regen Rate:", startRegen[i], 1, 50, 2, 240);

                // Render descriptive hints at the bottom of card
                int hintY = rowY + 315;
                DrawLine(posX + 25, hintY, posX + cardW - 25, hintY, COLOR_PANEL_BORDER);
                
                string totalHpHint = "Total HP: " + to_string(startUnits[i] * startHP[i]) + " Health Points";
                string totalDmgHint = "Max Attack: " + to_string(startUnits[i] * startAttack[i]) + " Dmg/Turn";
                DrawText(totalHpHint.c_str(), posX + 25, hintY + 15, 13, COLOR_TEXT_S);
                DrawText(totalDmgHint.c_str(), posX + 25, hintY + 40, 13, COLOR_TEXT_S);
            }

            // C. Launch Button at the bottom
            if (DrawModernButton({ 400, 660, 400, 65 }, "⚔️ DEPLOY & LAUNCH WAR ⚔️", COLOR_GOLD, victorySound))
            {
                LaunchCampaign();
            }

            EndDrawing();
        }
        else
        {
            // ================= ACTIVE GAMEPLAY SYSTEM =================
            BeginDrawing();
            ClearBackground(COLOR_BG);

            // A. Header Panel
            DrawRectangle(0, 0, screenWidth, 80, COLOR_CARD_BG);
            DrawLine(0, 80, screenWidth, 80, COLOR_PANEL_BORDER);

            DrawText("⚔️ THE WARMONGER: A NEW DIMENSION", 30, 26, 22, COLOR_GOLD);
            
            // Day count indicator
            string dayText = "DAY " + to_string(day);
            DrawRectangleRounded({ 560, 20, 100, 40 }, 0.2f, 4, COLOR_PANEL_BORDER);
            DrawText(dayText.c_str(), 560 + (100 - MeasureText(dayText.c_str(), 18)) / 2, 31, 18, COLOR_GOLD);

            // Gold vault indicators
            string revenueText = "VAULT: " + to_string(merchant->GetRevenue()) + " GOLD";
            DrawText(revenueText.c_str(), screenWidth - MeasureText(revenueText.c_str(), 20) - 30, 28, 20, COLOR_GOLD);

            string factionsName[3] = { "Orcs", "Dwarves", "Elves" };
            Color factionsColor[3] = { COLOR_ORCS, COLOR_DWARVES, COLOR_ELVES };
            Faction* fPtrs[3] = { orcs, dwarves, elves };

            if (!gameOver)
            {
                // B. MERCHANT SHOP PANEL (LEFT SIDE)
                DrawGlassCard({ 30, 110, 420, 660 }, COLOR_GOLD);
                DrawText("🛡️ MERCHANT DEPOT", 50, 130, 20, COLOR_GOLD);
                DrawLine(50, 160, 430, 160, COLOR_PANEL_BORDER);

                // Daily limits info
                string weaponsStock = "Weapons Left: " + to_string(merchant->GetWeaponPoints()) + " pts";
                string armorsStock = "Armors Left: " + to_string(merchant->GetArmorPoints()) + " pts";
                DrawText(weaponsStock.c_str(), 60, 175, 16, COLOR_TEXT_P);
                DrawText(armorsStock.c_str(), 250, 175, 16, COLOR_TEXT_P);

                // Draw visual mini-bar indicators for stock
                DrawRectangle(60, 200, 150, 8, COLOR_PANEL_BORDER);
                DrawRectangle(60, 200, 150 * ((float)merchant->GetWeaponPoints() / 10.0f), 8, COLOR_ORCS);
                DrawRectangle(250, 200, 150, 8, COLOR_PANEL_BORDER);
                DrawRectangle(250, 200, 150 * ((float)merchant->GetArmorPoints() / 10.0f), 8, COLOR_DWARVES);

                // Sale Interface for Factions
                for (int i = 0; i < 3; i++)
                {
                    int cardY = 230 + (i * 145);
                    DrawRectangleRounded({ 50, (float)cardY, 380, 130 }, 0.08f, 4, COLOR_BG);
                    DrawRectangleRoundedLines({ 50, (float)cardY, 380, 130 }, 0.08f, 4, COLOR_PANEL_BORDER);

                    // Faction Title inside shop
                    DrawText(factionsName[i].c_str(), 65, cardY + 12, 16, factionsColor[i]);

                    if (!fPtrs[i]->IsAlive())
                    {
                        DrawText("💀 DEFEATED - TRADING CLOSED", 160, cardY + 55, 14, COLOR_ORCS);
                        continue;
                    }

                    // Weapons sale row
                    DrawText("Weapons:", 65, cardY + 45, 14, COLOR_TEXT_S);
                    string wAmtStr = to_string(sellWeaponsAmount[i]);
                    DrawText(wAmtStr.c_str(), 200, cardY + 45, 15, COLOR_TEXT_P);

                    // Increments/Decrements for weapons
                    if (DrawModernButton({ 150, (float)(cardY + 40), 30, 25 }, "-", COLOR_PANEL_BORDER, clickSound))
                    {
                        if (sellWeaponsAmount[i] > 0) sellWeaponsAmount[i]--;
                    }
                    if (DrawModernButton({ 230, (float)(cardY + 40), 30, 25 }, "+", COLOR_PANEL_BORDER, clickSound))
                    {
                        if (sellWeaponsAmount[i] < merchant->GetWeaponPoints()) sellWeaponsAmount[i]++;
                    }

                    // Weapon Sell Button
                    if (DrawModernButton({ 285, (float)(cardY + 38), 130, 28 }, "Sell Weapon", COLOR_ORCS, clickSound))
                    {
                        if (sellWeaponsAmount[i] == 0)
                        {
                            PlaySound(fartSound);
                            SpawnNotification({ 220, (float)(cardY + 45) }, "Select amount!", COLOR_ORCS);
                        }
                        else
                        {
                            int oldRev = merchant->GetRevenue();
                            bool success = merchant->SellWeapons(factionsName[i], sellWeaponsAmount[i]);
                            if (success)
                            {
                                int diff = merchant->GetRevenue() - oldRev;
                                PlaySound(coinSound);
                                SpawnNotification({ 350, (float)(cardY + 15) }, "+" + to_string(diff) + "g", COLOR_GOLD);
                                SpawnNotification({ 80, (float)(cardY + 15) }, "Weapons Sold!", COLOR_GREEN_GLOW);
                                battleLogs.push_back("💰 Sold " + to_string(sellWeaponsAmount[i]) + " weapons to " + factionsName[i] + " for " + to_string(diff) + " gold!");
                                sellWeaponsAmount[i] = 0;
                            }
                            else
                            {
                                PlaySound(fartSound);
                                SpawnNotification({ 350, (float)(cardY + 45) }, "Failed!", COLOR_ORCS);
                            }
                        }
                    }

                    // Armors sale row
                    DrawText("Armors:", 65, cardY + 85, 14, COLOR_TEXT_S);
                    string aAmtStr = to_string(sellArmorsAmount[i]);
                    DrawText(aAmtStr.c_str(), 200, cardY + 85, 15, COLOR_TEXT_P);

                    // Increments/Decrements for armors
                    if (DrawModernButton({ 150, (float)(cardY + 80), 30, 25 }, "-", COLOR_PANEL_BORDER, clickSound))
                    {
                        if (sellArmorsAmount[i] > 0) sellArmorsAmount[i]--;
                    }
                    if (DrawModernButton({ 230, (float)(cardY + 80), 30, 25 }, "+", COLOR_PANEL_BORDER, clickSound))
                    {
                        if (sellArmorsAmount[i] < merchant->GetArmorPoints()) sellArmorsAmount[i]++;
                    }

                    // Armor Sell Button
                    if (DrawModernButton({ 285, (float)(cardY + 78), 130, 28 }, "Sell Armor", COLOR_DWARVES, clickSound))
                    {
                        if (sellArmorsAmount[i] == 0)
                        {
                            PlaySound(fartSound);
                            SpawnNotification({ 220, (float)(cardY + 85) }, "Select amount!", COLOR_ORCS);
                        }
                        else
                        {
                            int oldRev = merchant->GetRevenue();
                            bool success = merchant->SellArmors(factionsName[i], sellArmorsAmount[i]);
                            if (success)
                            {
                                int diff = merchant->GetRevenue() - oldRev;
                                PlaySound(coinSound);
                                SpawnNotification({ 350, (float)(cardY + 15) }, "+" + to_string(diff) + "g", COLOR_GOLD);
                                SpawnNotification({ 80, (float)(cardY + 15) }, "Armors Sold!", COLOR_GREEN_GLOW);
                                battleLogs.push_back("💰 Sold " + to_string(sellArmorsAmount[i]) + " armors to " + factionsName[i] + " for " + to_string(diff) + " gold!");
                                sellArmorsAmount[i] = 0;
                            }
                            else
                            {
                                PlaySound(fartSound);
                                SpawnNotification({ 350, (float)(cardY + 85) }, "Failed!", COLOR_ORCS);
                            }
                        }
                    }
                }


                // C. BATTLEFIELD & FACTION CARDS (RIGHT SIDE)
                for (int i = 0; i < 3; i++)
                {
                    Faction* f = fPtrs[i];
                    float posX = 475.0f + (i * 235.0f);
                    float posY = 110.0f;
                    float cardW = 220.0f;
                    float cardH = 390.0f;

                    // Add shake offsets if card is hit
                    float finalX = posX;
                    float finalY = posY;
                    if (shakeIntensity[i] > 0.0f)
                    {
                        finalX += ((float)rand() / RAND_MAX * 16.0f - 8.0f) * shakeIntensity[i];
                        finalY += ((float)rand() / RAND_MAX * 16.0f - 8.0f) * shakeIntensity[i];
                    }

                    // Render Glass Faction Card
                    DrawGlassCard({ finalX, finalY, cardW, cardH }, factionsColor[i]);

                    // Top Faction Banner
                    DrawRectangleRounded({ finalX + 10, finalY + 10, cardW - 20, 45 }, 0.1f, 4, factionsColor[i]);
                    DrawText(factionsName[i].c_str(), finalX + (cardW - MeasureText(factionsName[i].c_str(), 18)) / 2.0f, finalY + 22, 18, COLOR_TEXT_P);

                    // Alive/Defeated badge
                    if (f->IsAlive())
                    {
                        DrawRectangleRounded({ finalX + 60, finalY + 68, 100, 22 }, 0.2f, 4, { 58, 150, 78, 100 });
                        DrawText("ALIVE", finalX + 60 + (100 - MeasureText("ALIVE", 12)) / 2, finalY + 73, 12, COLOR_GREEN_GLOW);
                    }
                    else
                    {
                        DrawRectangleRounded({ finalX + 60, finalY + 68, 100, 22 }, 0.2f, 4, { 180, 50, 50, 100 });
                        DrawText("DEFEATED", finalX + 60 + (100 - MeasureText("DEFEATED", 12)) / 2, finalY + 73, 12, COLOR_RED_GLOW);
                    }

                    // Card war cries & quotes
                    int quoteY = finalY + 102;
                    if (factionsName[i] == "Orcs")
                    {
                        DrawText("\"Stop running, you'll only\"", finalX + 18, quoteY, 13, COLOR_TEXT_S);
                        DrawText("die tired.\"", finalX + 75, quoteY + 18, 13, COLOR_TEXT_S);
                    }
                    else if (factionsName[i] == "Dwarves")
                    {
                        DrawText("\"Taste the power of\"", finalX + 38, quoteY, 13, COLOR_TEXT_S);
                        DrawText("our axes!\"", finalX + 78, quoteY + 18, 13, COLOR_TEXT_S);
                    }
                    else
                    {
                        DrawText("\"You cannot reach\"", finalX + 45, quoteY, 13, COLOR_TEXT_S);
                        DrawText("our elegance.\"", finalX + 65, quoteY + 18, 13, COLOR_TEXT_S);
                    }

                    // Unit Health & Count Info
                    string unitsStr = to_string(f->getNumberOfUnits()) + " UNITS";
                    DrawText(unitsStr.c_str(), finalX + 30, finalY + 155, 16, COLOR_TEXT_P);

                    // Modern Health bar
                    float maxUnits = startUnits[i];
                    float healthPercent = f->IsAlive() ? ((float)f->getNumberOfUnits() / maxUnits) : 0.0f;
                    if (healthPercent < 0) healthPercent = 0;
                    
                    DrawRectangle(finalX + 30, finalY + 180, 160, 12, COLOR_PANEL_BORDER);
                    DrawRectangle(finalX + 30, finalY + 180, 160 * healthPercent, 12, factionsColor[i]);

                    // Stats values
                    int statsY = finalY + 205;
                    auto DrawStatRow = [&](string label, string val, int offset) {
                        DrawText(label.c_str(), finalX + 25, statsY + offset, 14, COLOR_TEXT_S);
                        DrawText(val.c_str(), finalX + 150, statsY + offset, 14, COLOR_TEXT_P);
                    };

                    DrawStatRow("Attack Pt:", to_string(f->getAttackPoint()), 0);
                    DrawStatRow("Unit HP:", to_string(f->getHealthPoint()), 25);
                    DrawStatRow("Regen Rate:", to_string(f->getUnitRegNum()), 50);
                    DrawStatRow("Total HP:", to_string(f->getTotalHealth()), 75);

                    // Apply damage red overlay hit flashes
                    if (flashIntensity[i] > 0.0f)
                    {
                        Color flashCol = { 255, 0, 0, (unsigned char)(120.0f * flashIntensity[i]) };
                        DrawRectangleRounded({ finalX, finalY, cardW, cardH }, 0.08f, 4, flashCol);
                    }
                }


                // D. BATTLE LOG FEED (BOTTOM RIGHT)
                DrawGlassCard({ 475, 520, 480, 250 }, COLOR_PANEL_BORDER);
                DrawText("📜 BATTLE RECORD", 495, 535, 16, COLOR_GOLD);
                DrawLine(495, 560, 935, 560, COLOR_PANEL_BORDER);

                // Render scrolling console messages
                int startIdx = (int)battleLogs.size() - 8;
                if (startIdx < 0) startIdx = 0;
                
                for (int k = startIdx; k < (int)battleLogs.size(); k++)
                {
                    int logY = 572 + ((k - startIdx) * 22);
                    Color logCol = COLOR_TEXT_S;
                    
                    // Color codes for the logs
                    if (battleLogs[k].find("💀") != string::npos || battleLogs[k].find("Defeated") != string::npos)
                    {
                        logCol = COLOR_RED_GLOW;
                    }
                    else if (battleLogs[k].find("💰") != string::npos || battleLogs[k].find("won") != string::npos)
                    {
                        logCol = COLOR_GOLD;
                    }
                    else if (battleLogs[k].find("🌱") != string::npos || battleLogs[k].find("Sold") != string::npos)
                    {
                        logCol = COLOR_GREEN_GLOW;
                    }
                    else if (battleLogs[k].find("⚔️") != string::npos)
                    {
                        logCol = COLOR_TEXT_P;
                    }

                    DrawText(battleLogs[k].c_str(), 495, logY, 13, logCol);
                }


                // E. ACTION CONTROL PANEL (BOTTOM FAR RIGHT)
                DrawGlassCard({ 970, 520, 200, 250 }, COLOR_GOLD);
                
                // Giant glowing turn simulator button
                if (DrawModernButton({ 985, 540, 170, 60 }, "SIMULATE BATTLE", COLOR_GOLD, clickSound))
                {
                    // Record prior unit status to check for damage/regeneration highlights
                    int oldUnits[3] = { orcs->getNumberOfUnits(), dwarves->getNumberOfUnits(), elves->getNumberOfUnits() };

                    // Process turn mechanics
                    merchant->EndTurn();

                    // Factions attack enemy elements
                    if (orcs->IsAlive()) orcs->PerformAttack();
                    if (dwarves->IsAlive()) dwarves->PerformAttack();
                    if (elves->IsAlive()) elves->PerformAttack();

                    // Factions update status/regen units
                    if (orcs->IsAlive()) orcs->EndTurn();
                    if (dwarves->IsAlive()) dwarves->EndTurn();
                    if (elves->IsAlive()) elves->EndTurn();

                    // Recalculate survival counts
                    numOfFactionsAlive = 0;
                    if (orcs->IsAlive()) numOfFactionsAlive++;
                    if (dwarves->IsAlive()) numOfFactionsAlive++;
                    if (elves->IsAlive()) numOfFactionsAlive++;

                    // Clear temporary trade limits
                    for (int i = 0; i < 3; i++)
                    {
                        sellWeaponsAmount[i] = 0;
                        sellArmorsAmount[i] = 0;
                    }

                    battleLogs.push_back("⚔️ --- DAY " + to_string(day) + " BATTLE ROUND ---");

                    // Compare survivors
                    int newUnits[3] = { orcs->getNumberOfUnits(), dwarves->getNumberOfUnits(), elves->getNumberOfUnits() };
                    bool hitRegistered = false;

                    for (int i = 0; i < 3; i++)
                    {
                        int diff = newUnits[i] - oldUnits[i];
                        if (diff < 0)
                        {
                            // Faction suffered unit losses
                            shakeIntensity[i] = 1.0f;
                            flashIntensity[i] = 1.0f;
                            hitRegistered = true;
                            SpawnNotification({ 475.0f + (i * 235.0f) + 110.0f, 150.0f }, to_string(diff) + " units", COLOR_ORCS);
                            battleLogs.push_back("💥 " + factionsName[i] + " suffered -" + to_string(-diff) + " casualties!");
                        }
                        else if (diff > 0)
                        {
                            // Faction regenerated units
                            SpawnNotification({ 475.0f + (i * 235.0f) + 110.0f, 150.0f }, "+" + to_string(diff) + " units", COLOR_ELVES);
                            battleLogs.push_back("🌱 " + factionsName[i] + " regenerated +" + to_string(diff) + " units.");
                        }

                        if (oldUnits[i] > 0 && newUnits[i] <= 0)
                        {
                            battleLogs.push_back("💀 " + factionsName[i] + " has been completely wiped out from the face of the earth!");
                        }
                    }

                    if (hitRegistered)
                    {
                        PlaySound(hitSound);
                    }

                    // Increment Calendar Turn
                    day++;

                    // Verify Win/Loss outcomes
                    if (numOfFactionsAlive <= 1)
                    {
                        gameOver = true;
                        PlaySound(victorySound);
                    }
                }

                // Interactive game reset / quit actions
                if (DrawModernButton({ 985, 610, 170, 40 }, "RESET GAME", COLOR_PANEL_BORDER, clickSound))
                {
                    ResetGame();
                }

                if (DrawModernButton({ 985, 660, 170, 40 }, "📢 BATTLE HORN", COLOR_GOLD, fartSound))
                {
                    SpawnNotification({ 1070, 640 }, "*PHHHRRRTT*", COLOR_GOLD);
                    battleLogs.push_back("📢 Sounded the mighty battle horn! *PHHHRRRTT*");
                }

                if (DrawModernButton({ 985, 710, 170, 40 }, "EXIT SYSTEM", COLOR_ORCS, clickSound))
                {
                    break;
                }
            }
            else
            {
                // F. GAME OVER & VICTORY COVER OVERLAY
                DrawGlassCard({ 150, 120, 900, 560 }, COLOR_GOLD);
                
                DrawText("🏆 CONFLICT RESOLVED 🏆", 400, 160, 32, COLOR_GOLD);
                DrawLine(300, 220, 900, 220, COLOR_PANEL_BORDER);

                // Determine victor
                string winnerName = "None";
                Color winnerColor = COLOR_GOLD;
                string winnerQuote = "";

                if (orcs->IsAlive())
                {
                    winnerName = "ORCS";
                    winnerColor = COLOR_ORCS;
                    winnerQuote = "\"Stop running, you'll only die tired!\"";
                }
                else if (dwarves->IsAlive())
                {
                    winnerName = "DWARVES";
                    winnerColor = COLOR_DWARVES;
                    winnerQuote = "\"Taste the power of our heavy axes!\"";
                }
                else if (elves->IsAlive())
                {
                    winnerName = "ELVES";
                    winnerColor = COLOR_ELVES;
                    winnerQuote = "\"You cannot reach our elegant standard.\"";
                }

                if (winnerName != "None")
                {
                    string winTitle = winnerName + " HAVE WON THE WAR!";
                    DrawText(winTitle.c_str(), 600 - MeasureText(winTitle.c_str(), 26) / 2, 260, 26, winnerColor);
                    DrawText(winnerQuote.c_str(), 600 - MeasureText(winnerQuote.c_str(), 18) / 2, 310, 18, COLOR_TEXT_S);
                }
                else
                {
                    DrawText("MUTUAL DESTRUCTION - NO FACTION SURVIVED", 350, 260, 22, COLOR_ORCS);
                }

                // Stats summary card
                DrawRectangleRounded({ 250, 360, 700, 160 }, 0.1f, 4, COLOR_BG);
                DrawRectangleRoundedLines({ 250, 360, 700, 160 }, 0.1f, 4, COLOR_PANEL_BORDER);

                DrawText("CAMPAIGN METRICS", 280, 385, 18, COLOR_GOLD);
                
                string daysPassed = "Total Campaign Duration: " + to_string(day - 1) + " Days";
                string goldVault = "Accumulated Merchant Revenue: " + to_string(merchant->GetRevenue()) + " Gold";
                DrawText(daysPassed.c_str(), 280, 425, 16, COLOR_TEXT_P);
                DrawText(goldVault.c_str(), 280, 465, 16, COLOR_TEXT_P);

                // Play again button
                if (DrawModernButton({ 400, 560, 180, 50 }, "PLAY AGAIN", COLOR_GREEN_GLOW, clickSound))
                {
                    ResetGame();
                }

                // Exit button
                if (DrawModernButton({ 620, 560, 180, 50 }, "EXIT GAME", COLOR_ORCS, clickSound))
                {
                    break;
                }
            }

            // Draw active notifications
            for (const auto &fn : notifications)
            {
                Color drawCol = fn.color;
                drawCol.a = (unsigned char)(fn.alpha * 255.0f);
                DrawText(fn.text.c_str(), fn.position.x, fn.position.y, 18, drawCol);
            }

            EndDrawing();
        }
    }

    // --- 3. CLEANUP AND TERMINATION ---
    if (orcs) delete orcs;
    if (dwarves) delete dwarves;
    if (elves) delete elves;
    if (merchant) delete merchant;

    UnloadSound(coinSound);
    UnloadSound(hitSound);
    UnloadSound(clickSound);
    UnloadSound(victorySound);
    UnloadSound(fartSound);
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}