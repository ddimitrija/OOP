#define RAYGUI_IMPLEMENTATION
#include "raygui.h"          // https://github.com/raysan5/raygui  (raygui.h)
#include "tinyfiledialogs.h" // https://sourceforge.net/projects/tinyfiledialogs/

#include <raylib.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>

// ─── Constants ────────────────────────────────────────────────────────────────
static const int   TOOLBAR_H    = 40;
static const int   STATUSBAR_H  = 24;
static const int   BTN_W        = 80;
static const int   BTN_H        = 28;
static const int   BTN_PAD      = 6;
static const int   MAX_TEXT     = 1 << 20; // 1 MB of text

// ─── Globals ──────────────────────────────────────────────────────────────────
static char  g_text[MAX_TEXT]   = "";
static char  g_filepath[2048]   = "";
static bool  g_dirty            = false;
static bool  g_darkTheme        = true;
static bool  g_wordWrap         = true;
static bool  g_editMode         = false;

// ─── Theme colours ────────────────────────────────────────────────────────────
struct Theme {
    Color bg, toolbar, statusbar, border, text, btnFace, btnText, accent;
};

static const Theme DARK = {
    {18,  20,  24,  255},  // bg
    {26,  28,  34,  255},  // toolbar
    {14,  16,  20,  255},  // statusbar
    {50,  55,  70,  255},  // border
    {210, 215, 225, 255},  // text
    {38,  42,  52,  255},  // btnFace
    {200, 205, 220, 255},  // btnText
    {90,  140, 255, 255},  // accent
};

static const Theme LIGHT = {
    {245, 245, 242, 255},  // bg
    {230, 230, 228, 255},  // toolbar
    {215, 215, 212, 255},  // statusbar
    {190, 190, 186, 255},  // border
    {30,  30,  30,  255},  // text
    {220, 220, 218, 255},  // btnFace
    {30,  30,  30,  255},  // btnText
    {60,  110, 220, 255},  // accent
};

// ─── Helpers ──────────────────────────────────────────────────────────────────

static void ApplyRayguiTheme(const Theme &t) {
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,   ColorToInt(t.bg));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,  ColorToInt(t.text));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(t.accent));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(t.accent));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL,  ColorToInt(t.border));
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(t.accent));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL,    ColorToInt(t.btnFace));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED,   ColorToInt(t.toolbar));
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED,   ColorToInt(t.accent));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL,    ColorToInt(t.text));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL,  ColorToInt(t.border));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(t.accent));
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL,    ColorToInt(t.bg));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
}

static int CountLines(const char *s) {
    int n = 1;
    for (; *s; ++s) if (*s == '\n') ++n;
    return n;
}

static void LoadFile(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { tinyfd_messageBox("Error", "Cannot open file.", "ok", "error", 1); return; }
    size_t len = fread(g_text, 1, MAX_TEXT - 1, f);
    fclose(f);
    g_text[len] = '\0';
    strncpy(g_filepath, path, sizeof(g_filepath) - 1);
    g_dirty = false;
}

static bool SaveFile(const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) { tinyfd_messageBox("Error", "Cannot save file.", "ok", "error", 1); return false; }
    fwrite(g_text, 1, strlen(g_text), f);
    fclose(f);
    g_dirty = false;
    return true;
}

static void DoNew() {
    if (g_dirty) {
        int r = tinyfd_messageBox("Unsaved Changes",
            "Discard unsaved changes?", "yesno", "question", 0);
        if (!r) return;
    }
    g_text[0]   = '\0';
    g_filepath[0] = '\0';
    g_dirty     = false;
}

static void DoOpen() {
    if (g_dirty) {
        int r = tinyfd_messageBox("Unsaved Changes",
            "Discard unsaved changes?", "yesno", "question", 0);
        if (!r) return;
    }
    const char *filters[] = {"*.txt", "*.md", "*.csv", "*.log", "*"};
    const char *path = tinyfd_openFileDialog("Open File", "", 5, filters, "Text files", 0);
    if (path) LoadFile(path);
}

static void DoSave() {
    if (g_filepath[0] == '\0') {
        const char *filters[] = {"*.txt", "*"};
        const char *path = tinyfd_saveFileDialog("Save File", "untitled.txt", 2, filters, "Text files");
        if (!path) return;
        strncpy(g_filepath, path, sizeof(g_filepath) - 1);
    }
    SaveFile(g_filepath);
}

static void DoSaveAs() {
    const char *filters[] = {"*.txt", "*"};
    const char *path = tinyfd_saveFileDialog("Save As", "untitled.txt", 2, filters, "Text files");
    if (!path) return;
    strncpy(g_filepath, path, sizeof(g_filepath) - 1);
    SaveFile(g_filepath);
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(900, 620, "RayNotes");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // we handle close ourselves

    GuiLoadStyleDefault();
    ApplyRayguiTheme(DARK);

    // Track previous text length to detect edits from the TextBox
    size_t prevLen = 0;

    while (!WindowShouldClose()) {
        // ── Window title ────────────────────────────────────────────────────
        {
            const char *name = g_filepath[0] ? GetFileName(g_filepath) : "Untitled";
            char title[256];
            snprintf(title, sizeof(title), "%s%s — RayNotes", g_dirty ? "*" : "", name);
            SetWindowTitle(title);
        }

        int W = GetScreenWidth();
        int H = GetScreenHeight();
        const Theme &T = g_darkTheme ? DARK : LIGHT;

        // ── Keyboard shortcuts ───────────────────────────────────────────────
        bool ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
        bool shift = IsKeyDown(KEY_LEFT_SHIFT)  || IsKeyDown(KEY_RIGHT_SHIFT);
        if (ctrl) {
            if (IsKeyPressed(KEY_N)) DoNew();
            if (IsKeyPressed(KEY_O)) DoOpen();
            if (IsKeyPressed(KEY_S)) { if (shift) DoSaveAs(); else DoSave(); }
        }

        // ── Dirty detection (text changed via keyboard) ──────────────────────
        size_t curLen = strlen(g_text);
        if (curLen != prevLen) { g_dirty = true; prevLen = curLen; }

        // ── Layout rects ────────────────────────────────────────────────────
        Rectangle rToolbar  = {0, 0, (float)W, (float)TOOLBAR_H};
        Rectangle rStatus   = {0, (float)(H - STATUSBAR_H), (float)W, (float)STATUSBAR_H};
        Rectangle rEditor   = {4, (float)(TOOLBAR_H + 4),
                                (float)(W - 8),
                                (float)(H - TOOLBAR_H - STATUSBAR_H - 8)};

        BeginDrawing();
        ClearBackground(T.bg);

        // ── Toolbar ─────────────────────────────────────────────────────────
        DrawRectangleRec(rToolbar, T.toolbar);
        DrawLineEx({0, (float)TOOLBAR_H}, {(float)W, (float)TOOLBAR_H}, 1, T.border);

        float bx = 8;
        float by = (TOOLBAR_H - BTN_H) / 2.0f;

        auto Btn = [&](const char *label, float x) -> bool {
            Rectangle r = {x, by, (float)BTN_W, (float)BTN_H};
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(T.btnText));
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(T.btnFace));
            return GuiButton(r, label);
        };

        if (Btn("New",     bx))          DoNew();       bx += BTN_W + BTN_PAD;
        if (Btn("Open",    bx))          DoOpen();      bx += BTN_W + BTN_PAD;
        if (Btn("Save",    bx))          DoSave();      bx += BTN_W + BTN_PAD;
        if (Btn("Save As", bx))          DoSaveAs();    bx += BTN_W + BTN_PAD;

        // Separator
        DrawLineEx({bx + 4, by}, {bx + 4, by + BTN_H}, 1, T.border);
        bx += 14;

        // Word Wrap toggle
        {
            Rectangle r = {bx, by, (float)BTN_W, (float)BTN_H};
            Color face = g_wordWrap ? T.accent : T.btnFace;
            GuiSetStyle(TOGGLE, BASE_COLOR_NORMAL,  ColorToInt(face));
            GuiSetStyle(TOGGLE, TEXT_COLOR_NORMAL,  g_wordWrap
                                                    ? ColorToInt(WHITE)
                                                    : ColorToInt(T.btnText));
            GuiSetStyle(TOGGLE, BASE_COLOR_PRESSED, ColorToInt(T.accent));
            GuiSetStyle(TOGGLE, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
            if (GuiToggle(r, "Wrap", &g_wordWrap)) {} // handled by raygui
            bx += BTN_W + BTN_PAD;
        }

        // Theme toggle (right-aligned)
        {
            float tx = W - BTN_W - 8;
            Rectangle r = {tx, by, (float)BTN_W, (float)BTN_H};
            const char *label = g_darkTheme ? "Light" : "Dark";
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(T.btnFace));
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(T.btnText));
            if (GuiButton(r, label)) {
                g_darkTheme = !g_darkTheme;
                ApplyRayguiTheme(g_darkTheme ? DARK : LIGHT);
            }
        }

        // ── Editor ──────────────────────────────────────────────────────────
        // Draw a subtle border around the textarea
        DrawRectangleLinesEx({rEditor.x - 1, rEditor.y - 1,
                               rEditor.width + 2, rEditor.height + 2}, 1, T.border);

        // GuiTextBox is single-line; for multi-line we use GuiTextBoxMulti
        // (available in raygui 4.x as GuiTextBoxMulti)
        GuiSetStyle(TEXTBOX, BORDER_WIDTH, 0);
        if (GuiTextBoxMulti(rEditor, g_text, MAX_TEXT, g_editMode)) {
            g_editMode = !g_editMode;
        }

        // ── Status bar ──────────────────────────────────────────────────────
        DrawRectangleRec(rStatus, T.statusbar);
        DrawLineEx({0, rStatus.y}, {(float)W, rStatus.y}, 1, T.border);

        int lines = CountLines(g_text);
        int chars = (int)strlen(g_text);
        char status[256];
        snprintf(status, sizeof(status),
                 "  Lines: %d   Chars: %d   %s",
                 lines, chars,
                 g_filepath[0] ? g_filepath : "(unsaved)");

        DrawText(status, 6, H - STATUSBAR_H + 5, 13, T.text);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}