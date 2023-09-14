/*******************************************************************************************
*
*   DraggableWindowBox v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_DRAGGABLE_WINDOW_BOX_IMPLEMENTATION
*       #include "gui_draggable_window_box.h"
*
*       INIT: GuiDraggableWindowBoxState state = InitGuiDraggableWindowBox();
*       DRAW: GuiDraggableWindowBox(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()
#include <vector>
#include "Graph.h"

#ifndef GUI_DRAGGABLE_WINDOW_BOX_H
#define GUI_DRAGGABLE_WINDOW_BOX_H

#include "RuntimeSourceDependency.h"
RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes

namespace fs = std::filesystem;

typedef enum {
    FILE_EXPLORER_NULL = 0,
    OPEN_SCENE,
    OPEN_TILESET,
    OPEN_ANIMATION,
    OPEN_ANIMATION_GRAPH,
    NEW_TILESET,
    SELECT_TILESET,
    SELECT_ANIMATION,
    SELECT_ANIMATION_VERTEX,
    SAVE_SCENE
} GuiFileExplorerListener;

typedef enum {
    OPEN_FILE = 0,
    CREATE_FILE
} GuiFileExplorerState;

typedef enum {
    RENDER_GRAPH_STATE_NULL = 0,
    RENDER_GRAPH_EXPECTING_FIRST,
    RENDER_GRAPH_EXPECTING_END,
    RENDER_GRAPH_EXPECTING_NAME,
    RENDER_GRAPH_EDITTING_VERTEX,
    RENDER_GRAPH_EDITTING_EDGE,
    RENDER_GRAPH_EDITTING_DESTINATION
} RenderGraphEditingState;

typedef enum {
    SCENE_EDITOR_STATE_PLAYING = 0,
    SCENE_EDITOR_STATE_PAUSED,
    SCENE_EDITOR_STATE_STOPPED,
} SceneEditorState;

typedef struct {
    const char* title;
    bool active, dragging, resizehover, resizeing;

    Rectangle bounds;
    Vector2 anchor, prev;

} GuiDraggableWindowBoxState;

typedef struct {
    GuiFileExplorerListener listener;
    GuiFileExplorerState state;

    string extension, base_path, return_path;
    char text[100];

    bool close, editting;
} GuiFileExplorer;

typedef struct {
    shared_ptr<TileMap> _tilemap;
    bool active;
    int current;

} GuiTileMapEdittingState;

typedef struct {
    const char* title;
    bool edittingx, edittingy;
    char x[32], y[32];

    Vector2 values;

} GuiInputVector2State;

typedef struct {
    const char* title;
    bool editting;
    char value[128];

} GuiInputStringState;

typedef struct {
    EngineAnimationGraph graph;

    Rectangle layout;
    int new_count;

    string begin, end, name;
    RenderGraphEditingState editting;
    bool setup;
} GuiGraphRenderState;

// GuiDraggableWindowBoxState* current_window = NULL;

// #ifdef __cplusplus
// extern "C" {            // Prevents name mangling of functions
// #endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiDraggableWindowBoxState  InitGuiDraggableWindowBox(Rectangle bounds, const char* title);
GuiFileExplorer             InitGuiFileExplorer(GuiFileExplorerListener listener, GuiFileExplorerState state, string extension, string base_path);
GuiInputVector2State        InitGuiInputVector2(const char* title, Vector2 values);
GuiInputStringState         InitGuiInputString(const char* title, string value);
GuiGraphRenderState         InitGuiGraphRenderState(LabeledGraph<string, string> graph, Rectangle rec);
GuiTileMapEdittingState     InitGuiTileMapEdittingState(shared_ptr<TileMap> map);

bool GuiDraggableWindowBox(GuiDraggableWindowBoxState* state);
void GuiGraphRender(GuiGraphRenderState* state);
int  GuiDrawScreenGrid(Vector2 initial, GuiTileMapEdittingState* map);
int  GuiActionDropDown(Rectangle bounds, const char* text, bool* editMode, int* selected);
int  GuiActionPane(Rectangle bounds, const char** text, int count, int* editMode);
void GuiInputVector2(Rectangle bounds, GuiInputVector2State* state);
int  GuiInputString(Rectangle bounds, GuiInputStringState* state);


shared_ptr<GameObject> GuiTree(Rectangle bounds, Rectangle itemBounds, Nary<pair<bool, shared_ptr<GameObject>>> tree);
void                   GuiTree_i(Rectangle bounds, Rectangle *itemBounds, Nary<pair<bool, shared_ptr<GameObject>>> a, float offset, shared_ptr<GameObject> *id);

Rectangle moveRectangle(Rectangle r, Vector2 vec);

shared_ptr<GameObject> find_object_by_id(const Nary<GameObject>& a, ObjectID e);
shared_ptr<GameObject> find_object_by_id_i(const Nary<GameObject>& a, bool& found, ObjectID e);

Nary<GameObject> find_node_by_id(const Nary<GameObject>& a, ObjectID e);
Nary<GameObject> find_node_by_id_i(const Nary<GameObject>& a, bool& found, ObjectID e);

// #ifdef __cplusplus
// }
// #endif

#endif // GUI_DRAGGABLE_WINDOW_BOX_H

/***********************************************************************************
*
*   GUI_DRAGGABLE_WINDOW_BOX IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_DRAGGABLE_WINDOW_BOX_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiDraggableWindowBoxState InitGuiDraggableWindowBox(Rectangle bounds, const char* title) {
    GuiDraggableWindowBoxState state = { 0 };

    state.active = true;
    state.dragging = false;
    state.resizehover = false;
    state.resizeing = false;

    state.prev = { 0, 0 };

    state.bounds = Rectangle{ 0, 0, bounds.width, bounds.height };
    state.title = title;

    state.anchor = { bounds.x, bounds.y };

    // Custom variables initialization

    return state;
}

GuiFileExplorer InitGuiFileExplorer(GuiFileExplorerListener listener, GuiFileExplorerState state, string extension, string base_path) {
    GuiFileExplorer result;

    result.listener  = listener;
    result.state     = state;
    result.extension = extension;
    result.base_path = base_path;
    strcpy(result.text, "");

    if (state == CREATE_FILE) result.editting = false;

    return result;
}

GuiInputVector2State InitGuiInputVector2(const char* title, Vector2 values) {
    GuiInputVector2State state = { 0 };

    state.title = title;

    state.edittingx = false;
    state.edittingy = false;

    strcpy(state.x, ftostr(values.x, 3).c_str());
    strcpy(state.y, ftostr(values.y, 3).c_str());

    state.values = values;

    // Custom variables initialization

    return state;
}
GuiInputStringState InitGuiInputString(const char* title, string value) {
    GuiInputStringState state = { 0 };

    state.title = title;

    state.editting = false;

    strcpy(state.value, value.c_str());

    // Custom variables initialization

    return state;
}

GuiGraphRenderState InitGuiGraphRenderState(LabeledGraph<string, string> graph, Rectangle rec) {
    GuiGraphRenderState state;

    state.graph.graph = graph;
    state.layout = rec;
    state.new_count = 0;

    state.begin = state.end = state.name = "";
    state.editting = RENDER_GRAPH_STATE_NULL;

    state.setup = false;

    return state;
}

GuiTileMapEdittingState InitGuiTileMapEdittingState(shared_ptr<TileMap> map) {
    GuiTileMapEdittingState state;

    state._tilemap = map;
    state.active = false;
    state.current = 1;

    return state;
}

bool GuiDraggableWindowBox(GuiDraggableWindowBoxState* state) {
    if (state->active) {
        //--------------------------------------------------------------------
        // Update control
        //--------------------------------------------------------------------
        #if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
        #define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT        24
        #endif

        Rectangle statusBar = moveRectangle(state->bounds, state->anchor);
        statusBar.height = RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), statusBar)) {
            state->dragging = true;
            state->prev = GetMousePosition();
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) state->dragging = false;

        if (state->dragging) {
            Vector2 move{ state->prev.x - GetMousePosition().x, state->prev.y - GetMousePosition().y };
            state->anchor = Vector2{ state->anchor.x - move.x, state->anchor.y - move.y };
            state->prev = GetMousePosition();
        }

        state->resizehover = CheckCollisionPointRec(GetMousePosition(), Rectangle{ statusBar.x + state->bounds.width - 5, statusBar.y + state->bounds.height - 5 , 5 ,5 });
        if (state->resizehover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->resizeing = true;
            state->prev = GetMousePosition();
        }
        else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) state->resizeing = false;

        if (state->resizeing) {
            Vector2 move{ state->prev.x - GetMousePosition().x, state->prev.y - GetMousePosition().y };
            state->bounds.width = state->bounds.width - move.x < 190 ? 190 : state->bounds.width - move.x;
            state->bounds.height = state->bounds.height - move.y < 48 ? 48 : state->bounds.height - move.y;
            state->prev = GetMousePosition();
        }

        //--------------------------------------------------------------------
        // Draw control
        //--------------------------------------------------------------------
        state->active = !GuiWindowBox(moveRectangle(state->bounds, state->anchor), state->title);
        if (state->resizehover || state->resizeing) GuiDrawIcon(187, statusBar.x + state->bounds.width - 32, statusBar.y + state->bounds.height - 32, 2, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
    }

    return state->active;
}

void GuiDrawFileExplorer(GuiFileExplorer* state, Rectangle bounds) {
    state->close = false;
    Rectangle h = bounds;
    h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
    h.height = 20;
    h.width -= 2;
    int i = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
    int j = GuiGetStyle(BUTTON, BORDER_WIDTH);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 0);

    std::string base_path = fs::current_path().string() + state->base_path;
    for (const auto& entry : fs::recursive_directory_iterator(base_path)) {
        if (entry.path().extension() == state->extension && h.y + h.height < bounds.y + bounds.height - h.height) {
            std::string text = entry.path().string();
            if (GuiButton(h, text.substr(text.find(state->base_path), text.size()).c_str())) {
                state->return_path = text.substr(text.find(state->base_path) + 1, text.size());
                if (state->state == OPEN_FILE) state->close = true;
                else strcpy(state->text, state->return_path.c_str());
            }
            h.y += h.height + 1;
        }
    }
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, i);
    GuiSetStyle(BUTTON, BORDER_WIDTH, j);

    if (state->state == CREATE_FILE){
        if (GuiTextBox({ bounds.x, bounds.y + bounds.height - 20.0f, bounds.width - 20.0f, 20.0f }, state->text, 100, state->editting)) {
            state->editting = !state->editting;
            if (!state->editting) state->return_path = state->text;
        }
        Rectangle b{ bounds.x + bounds.width - 20.0f, bounds.y + bounds.height - 20.0f, 20.0f, 20.0f };
        GuiState prev = guiState;
        guiState = state->return_path == ""? STATE_DISABLED : prev;
        if (GuiButton(b, "#119#")) state->close = true;
        guiState = prev;
    }
}

void GuiGraphRender(GuiGraphRenderState* state) {
    vector<string> vertices = state->graph.graph.GetVertex();
    float spacing = 20.0f;
    float width = (state->layout.width - 20.0f) / vertices.size() - spacing;
    Rectangle r{ state->layout.x + 20.0f, state->layout.y + 40.0f, width, 20 };

    int actual_color = 0;
    Color colors[12] = { 
        Color{255, 255, 0, 255}, // Yellow
        Color{0, 255, 0, 255},   // Yellow-Green
        Color{0, 121, 0, 255},   // Green
        Color{0, 147, 147, 255}, // Blue-Green
        Color{0, 0, 255, 255},   // Blue
        Color{115, 8, 165, 255}, // Blue-Violet
        Color{186, 0, 255, 255}, // Violet
        Color{204, 0, 175, 255}, // Red-Violet
        Color{255, 0, 0, 255},   // Red
        Color{255, 70, 0, 255},  // Red-Orange
        Color{255, 127, 0, 255}, // Orange
        Color{255, 179, 0, 255}  // Yellow-Orange
    };

    for (auto i : vertices) {
        if (GuiButton(r, NULL)) {
            if (state->editting == RENDER_GRAPH_EXPECTING_FIRST) {
                state->editting = RENDER_GRAPH_EXPECTING_END;
                state->begin = i; 
            }
            else if(state->editting == RENDER_GRAPH_EXPECTING_END) {
                state->editting = RENDER_GRAPH_EXPECTING_NAME;
                state->setup = true;
                state->end = i; 
            }
            else if (state->editting == RENDER_GRAPH_EDITTING_DESTINATION) {
                state->graph.graph.ChangeDestination(state->begin, i, state->name);
                state->editting = RENDER_GRAPH_STATE_NULL;
                state->begin = state->end = state->name = "";
            }
            else {
                state->editting = RENDER_GRAPH_EDITTING_VERTEX;
                state->setup = true;
                state->begin = i;
            }
        }

        GuiDrawRectangle(r, 1, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)), colors[actual_color]);
        Rectangle new_aux1 = {r.x + 5, r.y, r.width - 7, r.height};
        GuiDrawText(i.c_str(), new_aux1, TEXT_ALIGN_LEFT, actual_color <= 1 ? BLACK : WHITE);

        Rectangle aux = moveRectangle(r, {0, 20});
        for (auto j : state->graph.graph.GetEdges(i)) {
            string next = state->graph.graph.GetNextVertex(i, j);

            Rectangle new_aux2 = { aux.x + 10, aux.y, aux.width - 20, aux.height };
            if (GuiButton(new_aux2, NULL)) {
                state->editting = RENDER_GRAPH_EDITTING_EDGE;
                state->setup = true;
                state->name = j;
                state->begin = i;
                state->end = next;
            }

            GuiDrawRectangle(aux, 0, WHITE, GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)));
            GuiDrawText(j.c_str(), new_aux2, TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

            int x = 0;
            bool found = false;

            while (x < vertices.size() && !found) {
                found = vertices[x] == next;
                if (!found) x++;
            }
            
            if (found) {
                if (GuiButton({ aux.x + aux.width - 10, aux.y, 10, aux.height }, NULL)) {
                    state->editting = RENDER_GRAPH_EDITTING_DESTINATION;
                    state->name = j;
                    state->begin = i;
                }
                GuiDrawRectangle({ aux.x + aux.width - 10, aux.y, 10, aux.height }, 0, WHITE, colors[x]);
            }

            aux = moveRectangle(aux, {0, 20});
        }

        r = moveRectangle(r, {r.width + spacing, 0});
        actual_color++;
    }
}

int GuiActionDropDown(Rectangle bounds, const char* text, bool* editMode, int* selected) {
    int result = 0;
    GuiState state = guiState;
    int itemFocused = -1;

    int itemCount = 0;
    const char** items = GuiTextSplit(text, ';', &itemCount, NULL);

    float width = 0, x = bounds.x;

    for (int i = 1; i < itemCount; i++) {
        int aux = GetTextWidth(items[i]) + 24;
        if (aux > width) width = (float)aux;
    }
    if (GuiGetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT) == TEXT_ALIGN_CENTER) x -= (width - (GetTextWidth(items[0]) + GuiGetStyle(ACTIONDROPDOWN, TEXT_PADDING) * 2)) / 2;
    else if (GuiGetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT) x -= (width - (GetTextWidth(items[0]) + GuiGetStyle(ACTIONDROPDOWN, TEXT_PADDING) * 2));

    Rectangle boundsOpenitem = { x, bounds.y + bounds.height, width, bounds.height };
    
    Rectangle boundsOpen = boundsOpenitem;
    boundsOpen.height = (itemCount - 1) * (bounds.height);

    Rectangle itemBounds = { bounds.x, bounds.y, GetTextWidth(items[0]) + GuiGetStyle(ACTIONDROPDOWN, TEXT_PADDING) * 2.0f, bounds.height };

    //--------------------------------------------------------------------
    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && (*editMode || !guiLocked) && (itemCount > 1) && !guiSliderDragging) {
        Vector2 mousePoint = GetMousePosition();

        if (*editMode) {
            state = STATE_PRESSED;

            // Check if mouse has been pressed or released outside limits
            if (!CheckCollisionPointRec(mousePoint, itemBounds) && !CheckCollisionPointRec(mousePoint, boundsOpen)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
            }

            // Check if already selected item has been pressed again
            if (CheckCollisionPointRec(mousePoint, itemBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;

            // Check focused and selected item
            for (int i = 1; i < itemCount; i++) {
                if (CheckCollisionPointRec(mousePoint, boundsOpenitem)) {
                    itemFocused = i;
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        *selected = i;
                        result = 1;         // Item selected
                    }
                    break;
                }

                // Update item rectangle y position for next item
                boundsOpenitem.y += (bounds.height);
            }

            boundsOpenitem.y = bounds.y + bounds.height;
        }
        else {
            if (CheckCollisionPointRec(mousePoint, itemBounds)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    result = 1;
                    state = STATE_PRESSED;
                }
                else state = STATE_FOCUSED;
            }
        }
    }
    //--------------------------------------------------------------------
    // Draw control
    //--------------------------------------------------------------------

    if (*editMode) GuiPanel(boundsOpen, NULL);

    if (state == STATE_PRESSED || state == STATE_FOCUSED) GuiDrawRectangle(itemBounds, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER + state * 3)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE + state * 3)));
    GuiDrawText(items[0], GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + state * 3)));
    //GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT)
    if (*editMode) {
        // Draw visible items
        for (int i = 1; i < itemCount; i++) {
            if (i == itemFocused) {
                GuiDrawRectangle(boundsOpenitem, GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH), GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED)));
                GuiDrawText(items[i], GetTextBounds(STATUSBAR, boundsOpenitem), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_FOCUSED)));
            }
            else GuiDrawText(items[i], GetTextBounds(STATUSBAR, boundsOpenitem), GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL)));

            // Update item rectangle y position for next item
            boundsOpenitem.y += (bounds.height);
        }
    }

    return result;
}

// Action pane control
int GuiActionPane(Rectangle bounds, const char** text, int count, int *editMode){
    int result = -1;
    int pos = 0;

    GuiDrawRectangle(bounds, 0, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE)));

    for (int i = 0; i < count; i++) {
        int selected = -1;
        bool edit = *editMode == i;
        int itemCount = 0;
        const char** items = GuiTextSplit(text[i], ';', &itemCount, NULL);

        Rectangle itemBounds = { bounds.x + pos, bounds.y, GetTextWidth(items[0]) + GuiGetStyle(ACTIONDROPDOWN, TEXT_PADDING) * 2.0f, bounds.height };
        pos += (GetTextWidth(items[0]) + GuiGetStyle(ACTIONDROPDOWN, TEXT_PADDING) * 2.0f + 1.0f);

        if (edit) GuiLock();
        if (GuiActionDropDown(itemBounds, text[i], &edit, &selected)) {
            edit = !edit;
            if (!edit) result = selected - 1;
            else *editMode = i;
        }
        GuiUnlock();
    }

    return result;
}

// Input Vector2 control
void GuiInputVector2(Rectangle bounds, GuiInputVector2State* state) {
    Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
    GuiDrawText(state->title, GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    itemBounds = { itemBounds.x + itemBounds.width, bounds.y, (bounds.width - itemBounds.width) / 2 - 2, bounds.height };
    if (GuiTextBox(itemBounds, state->x, 128, state->edittingx)) {
        state->edittingx = !state->edittingx;
        if (!state->edittingx) state->values.x = stof(state->x);
    }

    itemBounds = { bounds.x + bounds.width - itemBounds.width - 1, bounds.y, itemBounds.width, bounds.height };
    if (GuiTextBox(itemBounds, state->y, 128, state->edittingy)) {
        state->edittingy = !state->edittingy;
        if (!state->edittingy) state->values.y = stof(state->y);
    }
}

int GuiInputString(Rectangle bounds, GuiInputStringState* state) {
    Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
    GuiDrawText(state->title, GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

    itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, (bounds.width - itemBounds.width) - 1, bounds.height };
    if (GuiTextBox(itemBounds, state->value, 128, state->editting)) {
        state->editting = !state->editting;
        return !state->editting;
    }
    return 0;
}

// Tree control
shared_ptr<GameObject> GuiTree(Rectangle bounds, Rectangle itemBounds, Nary<pair<bool, shared_ptr<GameObject>>> tree) {
    shared_ptr<GameObject> result = nullptr;
    float offset = 0.0f;
    itemBounds = moveRectangle(itemBounds, { bounds.x, bounds.y });

    for (int i = 1; i <= tree.nBrothers(); i++) GuiTree_i(bounds, &itemBounds, tree.brother(i), offset, &result);

    return result;
}

void GuiTree_i(Rectangle bounds, Rectangle *itemBounds, Nary<pair<bool, shared_ptr<GameObject>>> a, float offset, shared_ptr<GameObject>*result) {
    if (!a.isEmpty() && itemBounds->y + itemBounds->height <= bounds.y + bounds.height) {
        GuiState iconstate = guiState;
        GuiState textstate = guiState;

        Rectangle iconBounds = { offset + itemBounds->x, itemBounds->y, itemBounds->height * 0.8f, itemBounds->height };
        Rectangle textBounds = { offset + itemBounds->x + itemBounds->height * 0.8f, itemBounds->y, itemBounds->width - itemBounds->height * 0.8f - offset, itemBounds->height };

        //--------------------------------------------------------------------
        // Update control
        //--------------------------------------------------------------------
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, iconBounds)) {
            iconstate = STATE_FOCUSED;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) iconstate = STATE_PRESSED;
        }
        else if (CheckCollisionPointRec(mousePoint, textBounds)) {
            textstate = STATE_FOCUSED;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) textstate = STATE_PRESSED;
        }

        if (iconstate == STATE_PRESSED) a.content()->first = !a.content()->first;
        if (textstate == STATE_PRESSED) *result = a.content()->second;

        //--------------------------------------------------------------------
        // Draw control
        //--------------------------------------------------------------------
        string icon = a.content()->first ? "#116#" : "#115#";
        string text = a.content()->second->name();

        if (a.nChilds() > 0) {
            if (iconstate == STATE_PRESSED || iconstate == STATE_FOCUSED) GuiDrawRectangle(iconBounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER + iconstate * 3)), GetColor(GuiGetStyle(STATUSBAR, BASE + iconstate * 3)));
            GuiDrawText(icon.c_str(), iconBounds, GuiGetStyle(STATUSBAR, TEXT_ALIGNMENT), GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
        }

        if (textstate == STATE_PRESSED || textstate == STATE_FOCUSED) GuiDrawRectangle(textBounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER + textstate * 3)), GetColor(GuiGetStyle(STATUSBAR, BASE + textstate * 3)));
        GuiDrawText(text.c_str(), GetTextBounds(STATUSBAR, textBounds), GuiGetStyle(STATUSBAR, TEXT_ALIGNMENT), GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        // Update bounds and offset
        //--------------------------------------------------------------------
        itemBounds->y += itemBounds->height + 1;
        offset += itemBounds->height * 0.8f;

        if (a.content()->first) for (int i = 1; i <= a.nChilds(); i++) GuiTree_i(bounds, itemBounds, a.child(i), offset, result);
    }
}

int GuiDrawScreenGrid(Vector2 initial, GuiTileMapEdittingState* map) {
    Vector2 cell_size{ map->_tilemap->tileSet()->_tileSize.x * map->_tilemap->scale(), map->_tilemap->tileSet()->_tileSize.y * map->_tilemap->scale() };
    Vector2 origin{ float(int(initial.x) % (int)cell_size.x) - cell_size.x, float(int(initial.y) % (int)cell_size.y) - cell_size.y };
    Rectangle dst{ initial.x, initial.y, (float)GetScreenWidth(), (float)GetScreenHeight() };

    Vector2 result{ GetMousePosition().x - dst.x, GetMousePosition().y - dst.y };
    Vector2 position{ 
        result.x > 0 ? (int)(result.x / cell_size.x) : (int)((result.x - cell_size.x) / cell_size.x),
        result.y > 0 ? (int)(result.y / cell_size.y) : (int)((result.y - cell_size.y) / cell_size.y) 
    };

    result.x = result.x > 0? (int)(result.x / cell_size.x) * cell_size.x : (int)((result.x - cell_size.x) / cell_size.x) * cell_size.x;
    result.y = result.y > 0? (int)(result.y / cell_size.y) * cell_size.y : (int)((result.y - cell_size.y) / cell_size.y) * cell_size.y;
    GuiDrawRectangle({ result.x + dst.x, result.y + dst.y, cell_size.x, cell_size.y }, 0, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), Color{ 255, 255, 255, 100 });
    
    for (int i = (int)origin.x; i <= GetScreenWidth() + cell_size.x * 2; i += cell_size.x) DrawLine(i, (int)origin.y, i, (int)origin.y + GetScreenHeight() + cell_size.x * 2, i == initial.x ? GREEN : Color{ 255, 255, 255, 100 });
    for (int i = (int)origin.y; i <= GetScreenWidth() + cell_size.y * 2; i += cell_size.y) DrawLine((int)origin.x, i, (int)origin.x + GetScreenWidth() + cell_size.y * 2, i, i == initial.y ? RED : Color{ 255, 255, 255, 100 });

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) map->_tilemap->addPoint(position, map->current);
    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) map->_tilemap->addPoint(position, -1);

    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

Rectangle moveRectangle(Rectangle r, Vector2 vec) {
    return { vec.x + r.x, vec.y + r.y, r.width, r.height };
}

shared_ptr<GameObject> find_object_by_id(const Nary<GameObject>& a, ObjectID e) {
    shared_ptr<GameObject> res;
    int i = 1;
    bool found = false;

    while (!found && i <= a.nBrothers()) {
        res = find_object_by_id_i(a.brother(i), found, e);
        i++;
    }

    return res;
}

shared_ptr<GameObject> find_object_by_id_i(const Nary<GameObject>& a, bool& found, ObjectID e) {
    shared_ptr<GameObject> aux;
    if (!a.isEmpty() && !(a.content()->id() == e)) {
        int i = 1;
        while (!found && i <= a.nChilds()) {
            aux = find_object_by_id_i(a.child(i), found, e);
            i++;
        }
    }
    else if (!a.isEmpty() && a.content()->id() == e) {
        found = true;
        aux = a.content();
    }
    return aux;
}

Nary<GameObject> find_node_by_id(const Nary<GameObject>& a, ObjectID e) {
    Nary<GameObject> res;
    int i = 1;
    bool found = false;

    while (!found && i <= a.nBrothers()) {
        res = find_node_by_id_i(a.brother(i), found, e);
        i++;
    }

    return res;
}

Nary<GameObject> find_node_by_id_i(const Nary<GameObject>& a, bool& found, ObjectID e) {
    Nary<GameObject> aux;
    if (!a.isEmpty() && !(a.content()->id() == e)) {
        int i = 1;
        while (!found && i <= a.nChilds()) {
            aux = find_node_by_id_i(a.child(i), found, e);
            i++;
        }
    }
    else if (!a.isEmpty() && a.content()->id() == e) {
        found = true;
        aux = a;
    }
    return aux;
}

#endif // GUI_DRAGGABLE_WINDOW_BOX_IMPLEMENTATION
