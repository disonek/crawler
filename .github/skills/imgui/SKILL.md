---
name: imgui-expert
description: Expert in Dear ImGui library, familiar with its API, backends, and integration patterns. Use when working with ImGui widgets, rendering, or GUI development in C++ projects.
---

# ImGui Expert Skill

This skill provides deep expertise in the Dear ImGui library, including its core API, backends, widgets, and best practices for integration in C++ applications.

## Library Overview

**Dear ImGui** (v1.92.7 WIP) is a bloat-free graphical user interface library for C++. Key characteristics:
- **Paradigm**: Immediate-mode GUI (IMGUI) - UI is fully issued on every update
- **Core Philosophy**: Minimize state synchronization and UI-related state storage
- **Use Cases**: Game engines, real-time 3D applications, tools, debug UIs, content creation applications
- **Primary Advantage**: Simple, fast, portable, renderer-agnostic, with no external dependencies in core files

## Directory Structure

**Root Core Files**:
- `imgui.h` - Main public API header
- `imgui.cpp` - Main implementation
- `imgui_demo.cpp` - Comprehensive demo window showing all features
- `imgui_draw.cpp` - Drawing implementation
- `imgui_widgets.cpp` - Widget implementations
- `imgui_tables.cpp` - Table widget implementation
- `imgui_internal.h` - Internal structures (not for general use)
- `imconfig.h` - Compile-time configuration options

**Embedded Font Files**:
- `imstb_rectpack.h` - Rectangle packing utility
- `imstb_textedit.h` - Text editing functionality
- `imstb_truetype.h` - TrueType font rasterization

## Backends

**Platform Backends** (handle input, windowing, timing):
- Windows: `imgui_impl_win32.cpp`
- GLFW: `imgui_impl_glfw.cpp` (cross-platform)
- SDL2/SDL3: `imgui_impl_sdl2.cpp`, `imgui_impl_sdl3.cpp`
- Android: `imgui_impl_android.cpp`
- macOS/iOS: `imgui_impl_osx.mm`

**Renderer Backends** (handle texture creation, drawing):
- **DirectX**: DX9, DX10, DX11, DX12
- **OpenGL**: OpenGL2, OpenGL3+
- **Modern Graphics**: Vulkan, Metal, WebGPU
- **Software**: SDLRenderer, SDL GPU

## Core API Structure

### Context & Initialization
```cpp
ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL);
void DestroyContext(ImGuiContext* ctx = NULL);
ImGuiContext* GetCurrentContext();
void SetCurrentContext(ImGuiContext* ctx);
```

### Frame Lifecycle
```cpp
ImGuiIO& GetIO();                    // Access input/config
ImGuiStyle& GetStyle();              // Access styling
void NewFrame();                     // Start frame
void EndFrame();                     // End frame (auto-called by Render())
void Render();                       // Finalize for drawing
ImDrawData* GetDrawData();            // Get draw commands for rendering
```

### Main Widget Categories

**Text Display**:
- `Text()`, `TextColored()`, `TextDisabled()`, `TextWrapped()`
- `TextUnformatted()`, `BulletText()`, `LabelText()`

**Input Widgets**:
- `Button()`, `SmallButton()`, `ArrowButton()`, `InvisibleButton()`
- `Checkbox()`, `CheckboxFlags()`, `RadioButton()`
- `InputText()`, `InputTextMultiline()`, `InputInt()`, `InputFloat()`

**Selection Widgets**:
- `BeginCombo()`/`EndCombo()` - Dropdown menu
- `Selectable()` - List items
- `BeginMultiSelect()`/`EndMultiSelect()` - Multi-selection

**Value Input**:
- `DragFloat()`, `DragInt()` - Drag-based input
- `SliderFloat()`, `SliderInt()` - Slider controls
- `DragFloatRange2()`, `DragIntRange2()` - Range inputs

**Layout/Structure**:
- `Begin()`/`End()` - Main windows
- `BeginChild()`/`EndChild()` - Child windows
- `BeginGroup()`/`EndGroup()` - Grouping
- `BeginTabBar()`/`EndTabBar()` - Tab system
- `BeginTable()`/`EndTable()` - Table layout
- `BeginMenu()`/`EndMenu()` - Menus

**Visual Elements**:
- `Separator()`, `SameLine()`, `Spacing()`, `Indent()`
- `ProgressBar()`, `Bullet()`
- `Image()`, `ImageButton()` - Image display

## Key Data Structures

**Vector Types**:
- `ImVec2` - 2D position/size (x, y)
- `ImVec4` - 4D colors/rectangles (x, y, z, w)

**Drawing**:
- `ImDrawList` - Low-level drawing command buffer
- `ImDrawData` - All draw commands for a frame
- `ImDrawCmd` - Individual draw command
- `ImDrawVert` - Vertex with position, UV, color

**Fonts & Textures**:
- `ImFontAtlas` - Container for fonts
- `ImFont` - Runtime font data
- `ImFontConfig` - Font configuration
- `ImGlyphRanges` - Character ranges for fonts
- `ImTextureRef` - Texture identifier/reference

**Styling**:
- `ImGuiStyle` - Colors, sizes, padding, spacing
- `ImGuiCol_*` enums - Color categories (WindowBg, Text, Button, etc.)
- `ImGuiStyleVar_*` enums - Style variables

**Input/Configuration**:
- `ImGuiIO` - Main input/output structure
- Flags: `ImGuiConfigFlags_*` for DPI scaling, nav, etc.
- `ImGuiKey_*` - Keyboard constants
- `ImGuiMouseButton_*` - Mouse button identifiers

## Important Flags & Enumerations

**Window Flags** (`ImGuiWindowFlags_`):
- `NoTitleBar`, `NoResize`, `NoMove`, `NoScrollbar`
- `AlwaysAutoResize`, `Modal`, `NoBackground`

**Button/Item Flags**:
- `ImGuiButtonFlags_*` - Button behaviors
- `ImGuiItemFlags_*` - Item properties (disabled, hidden, etc.)

**Combo/Popup Flags**:
- `ImGuiComboFlags_*` - Dropdown options
- `ImGuiPopupFlags_*` - Popup behaviors

**Input Text Flags**:
- `ImGuiInputTextFlags_*` - Password, read-only, multiline, etc.

**Slider/Drag Flags**:
- `ImGuiSliderFlags_*` - Always clamp, logarithmic, etc.

## Examples

The library includes 20+ standalone example applications demonstrating integration with various platforms and renderers, from ~25 lines of boilerplate code each.

## Usage Guidelines

When using this skill:
- Always include the necessary headers: `#include "imgui.h"` and backend headers
- Initialize ImGui context and backends in your application startup
- Call `NewFrame()` at the start of each frame, issue UI commands, then `Render()` and `GetDrawData()` for rendering
- Use the demo window (`ImGui::ShowDemoWindow()`) to explore available widgets and features
- Follow immediate-mode paradigm: rebuild UI every frame based on current application state
- Use unique IDs for widgets (automatic for most, but explicit for dynamic content)
- Handle input through the backend, not directly

This skill can help with:
- Explaining ImGui concepts and API usage
- Debugging ImGui integration issues
- Providing code examples for common widgets
- Advising on best practices for layout and styling
- Assisting with backend setup and configuration