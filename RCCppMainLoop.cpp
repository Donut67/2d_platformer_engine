#include <cctype>
#include "RCCppMainLoop.h"
#include "raylib.h"

// /*
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_dark.h"
//*/

#include "platformerEngineBasics.h"
#include "PlayerController.h"
#include "Rigidbody.h"
// New Behaviours

// /*
#define GUI_DRAGGABLE_WINDOW_BOX_IMPLEMENTATION
#include "gui_draggable_window_box.h"
#include "observer_pattern.h"
// */

#include "RuntimeLinkLibrary.h"
RUNTIME_COMPILER_LINKLIBRARY("lib\\raylib.lib");

#include "IObject.h"
#include "SystemTable.h"
#include "ISimpleSerializer.h"
#include "IRuntimeObjectSystem.h"
#include "IObjectFactorySystem.h"
#include "ObjectInterfacePerModule.h"

// /*
#include "RuntimeSourceDependency.h"
//RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("platformerEngineUI", ".cpp");
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("platformerEngineBasics", ".cpp");
// */

namespace fs = std::filesystem;

// RCC++ uses interface Id's to distinguish between different classes
// here we have only one, so we don't need a header for this enum and put it in the same
// source code file as the rest of the code
enum InterfaceIDEnumRCCppEngine
{
    IID_IRCCPP_MAIN_LOOP = IID_ENDInterfaceID, // IID_ENDInterfaceID from IObject.h InterfaceIDEnum

    IID_ENDInterfaceIDEnumRCCppEngine
};

struct RCCppMainLoop : RCCppMainLoopI, TInterface<IID_IRCCPP_MAIN_LOOP, IObject>
{
    // controls initialization
    //----------------------------------------------------------------------------------
    const char* ActionBar000Matrix[3] = {
        "File;#08#New Scene;#05#Open Scene;#02#Save Scene;#159#Exit",
        "Edit;#56#Undo;#57#Redo;#17#Cut;#16#Copy;#18#Paste",
        "Window;##Hierarchy;##Inspector;##Tileset Editor;##Animation Editor"
    };

    LabeledGraph<string, string> graph;

    int ActionBar000EditMode = -1;
    int ActionBar000Result = -1;
    int ActionBar001EditMode = -1;
    int ActionBar001Result = -1;
    int ActionBar002EditMode = -1;
    int ActionBar002Result = -1;

    bool ActionDropdown000EditMode = false;
    bool Spinner002EditMode = false;
    bool Spinner003EditMode = false;
    bool openSceneFilePath = false;
    bool openTileSetFilePath = false;
    bool windowShouldClose = false;

    int tileSetcolumns = 0;
    int tileSetrows = 0;

    Scene* scene = NULL;
    EngineAnimationGraph animation_graph;
    Nary<pair<bool, shared_ptr<GameObject>>> hierarchyTree;
    shared_ptr<Nary<GameObject>> copied_node, tmp_node;

    shared_ptr<GameObject> _game_object;
    Rastreator *rastreator;

    GuiDraggableWindowBoxState fileexplorerstate = InitGuiDraggableWindowBox(Rectangle{ 424, 400, 408, 184 }, "File Explorer");

    string fileextension = ".png";
    string fileexplorerpath = "\\resources";
    string fileexplorerreturnpath;

    shared_ptr<TileSetData> _tilesetdata;
    shared_ptr<EngineAnimation> _animationdata;
    int _animationdata_selecting          = -1;
    int _animationdata_editing            = -1;
    int _animationgraph_editting_label_v  = 0;
    int _animationgraph_editting_label_c  = 0;
    int _animationgraph_editting_label_e  = 0;
    bool _animationgraph_editting_label_x = 0;
    bool _animationdata_sel               = false;

    GuiDraggableWindowBoxState hierarchystate       = InitGuiDraggableWindowBox(Rectangle{ 0, 20, 190, 600 }, "Hierarchy");
    GuiDraggableWindowBoxState inspectorstate       = InitGuiDraggableWindowBox(Rectangle{ GetScreenWidth() - 330.0f, 20, 330, 300 }, "Inspector");
    GuiDraggableWindowBoxState tileseteditorstate   = InitGuiDraggableWindowBox(Rectangle{ 400.0f, 200, 400, 424 }, "TileSet Editor");
    GuiDraggableWindowBoxState animationeditorstate = InitGuiDraggableWindowBox(Rectangle{ 430.0f, 200, 1060, 454 }, "Animation Editor");
    GuiDraggableWindowBoxState animationgraphstate  = InitGuiDraggableWindowBox(Rectangle{ 50.0f, 200, 1200, 424 }, "Animation Graph Editor");

    GuiGraphRenderState graphrenderstate;
    GuiInputStringState rendergrapnameinputstate = InitGuiInputString("Label Name", "");
    GuiInputStringState animationnameinputstate  = InitGuiInputString("Name", "");
    vector<GuiInputStringState> animationedittingvalues;

    GuiTileMapEdittingState tilemapedittingstate;

    GuiFileExplorerListener fileExplorerListenerState;
    int scenestate = SCENE_EDITOR_STATE_PLAYING;

    // data for compiling window
    static constexpr double SHOW_AFTER_COMPILE_TIME = 3.0f;
    double compileStartTime = -SHOW_AFTER_COMPILE_TIME;
    double compileEndTime   = -SHOW_AFTER_COMPILE_TIME;
    unsigned int compiledModules = 0;

    Vector2 initial{ 0.0f, 0.0f }, prev{ 0.0f, 0.0f }, grid_position{ 0.0f, 0.0f }, cell_size{ 50.0f,50.0f };
    bool moving_scene = false, editting_tilemap = false;

    RCCppMainLoop() {
        g_pSys->pRCCppMainLoopI = this;
        g_pSys->pRuntimeObjectSystem->GetObjectFactorySystem()->SetObjectConstructorHistorySize(10);

        GuiLoadStyleDark();
        GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, 0);
        GuiSetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        GuiSetStyle(ACTIONDROPDOWN, TEXT_PADDING, 16);

        scene = new Scene();
        hierarchyTree = convertToNewTree(*scene->getRoot());
        _game_object = nullptr;

        hierarchystate.active = false;
        inspectorstate.active = false;
        fileexplorerstate.active = false;
        tileseteditorstate.active = false;
        animationeditorstate.active = false;
        animationgraphstate.active = false;

        rastreator = new Rastreator();

        graphrenderstate = InitGuiGraphRenderState(graph, Rectangle{ 400.0f, 200, 400, 424 });
    }

    void Serialize(ISimpleSerializer* pSerializer) override {
        SERIALIZE( compileStartTime );
        SERIALIZE( compileEndTime );

        // SERIALIZE( current_window );
        SERIALIZE( scene );
        SERIALIZE( hierarchyTree );
        // SERIALIZE( rastreator );
        
        SERIALIZE( fileexplorerstate );
        SERIALIZE( hierarchystate );
        SERIALIZE( inspectorstate );
        SERIALIZE( tileseteditorstate );
        SERIALIZE( animationeditorstate );
        SERIALIZE( animationgraphstate );
        SERIALIZE( graphrenderstate );
        
        SERIALIZE( _game_object );
        SERIALIZE( fileExplorerListenerState );
        SERIALIZE( scenestate );
        SERIALIZE( _tilesetdata );
        SERIALIZE( _animationdata );
        SERIALIZE( animationedittingvalues );

        SERIALIZE( initial );
    }

    void MainLoop() override {
        bool doRCCppUndo = false;
        bool doRCCppRedo = false;

        // Update
        //----------------------------------------------------------------------------------
        if (fileExplorerListenerState == OPEN_SCENE && fileexplorerreturnpath != "") {
            scene = new Scene(fileexplorerreturnpath);
            hierarchyTree = convertToNewTree(*scene->getRoot());
            fileexplorerreturnpath = "";
            openSceneFilePath = false;
        }
        else if (fileExplorerListenerState == OPEN_ANIMATION_GRAPH && fileexplorerreturnpath != "") {
            animation_graph = EngineAnimationGraph(fileexplorerreturnpath);
            fileexplorerreturnpath = "";
            graphrenderstate.graph.graph = animation_graph.graph;
            graphrenderstate.editting = RENDER_GRAPH_STATE_NULL;
            graphrenderstate.begin = graphrenderstate.end = graphrenderstate.name = "";
            openSceneFilePath = false;
        }

        if (scene && scenestate == SCENE_EDITOR_STATE_PLAYING) {
            if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()})) {
                moving_scene = true;
                prev = GetMousePosition();
            }
            else if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) moving_scene = false;

            if (moving_scene) {
                Vector2 move{ prev.x - GetMousePosition().x, prev.y - GetMousePosition().y };
                initial = Vector2{ initial.x - move.x, initial.y - move.y };
                prev = GetMousePosition();
            }

            scene->update(initial);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            if(scene) scene->draw();

            if (editting_tilemap) GuiDrawScreenGrid(initial, &tilemapedittingstate);

            // /*
            if (GuiDraggableWindowBox(&hierarchystate)) {
                Rectangle h = moveRectangle(hierarchystate.layoutRecs[0], hierarchystate.anchor);
                h.height -= RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;
                h.width -= 1;
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                shared_ptr<GameObject> obj;
                if (scene) obj = GuiTree(h, Rectangle{ 0, 0, h.width, 20 }, hierarchyTree);
                if (obj != nullptr) {
                    _game_object = obj;
                    tmp_node = make_shared<Nary<GameObject>>(obj);
                    rastreator->setGameObject(_game_object);
                }
                else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), h)) _game_object = nullptr;
            }

            if (GuiDraggableWindowBox(&inspectorstate)) {
                Rectangle h = moveRectangle(inspectorstate.layoutRecs[0], inspectorstate.anchor);
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                h.height = 20;
                h.width -= 2;
                if (_game_object) rastreator->notify(h);
                else GuiDrawText("No GameObject selected", GetTextBounds(STATUSBAR, h), TEXT_ALIGN_CENTER, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
            }

            if (GuiDraggableWindowBox(&tileseteditorstate)) {
                if (_tilesetdata != nullptr) {
                    #if !defined(RAYGUI_GRID_ALPHA)
                    #define RAYGUI_GRID_ALPHA    0.15f
                    #endif

                    Rectangle h = moveRectangle(tileseteditorstate.layoutRecs[0], tileseteditorstate.anchor);
                    h = moveRectangle(h, { 0, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 20 });
                    h.height = h.height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 41;

                    DrawRectangleLinesEx(h, 1, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

                    float scale = h.width / _tilesetdata->_texture->width;
                    if (_tilesetdata->_texture->height * scale >= h.height) scale = h.height / _tilesetdata->_texture->height;

                    DrawTextureEx(*_tilesetdata->_texture, { h.x, h.y }, 0.0f, scale, WHITE);

                    h = Rectangle{ h.x, h.y, _tilesetdata->_texture->width * scale, _tilesetdata->_texture->height * scale };
                    /*
                    if (CheckCollisionPointRec(GetMousePosition(), h)) {
                        Vector2 result{ GetMousePosition().x - h.x, GetMousePosition().y - h.y };
                        result.x = (int)(result.x / (_tilesetdata->_tileSize.x * scale)) * (_tilesetdata->_tileSize.x * scale);
                        result.y = (int)(result.y / (_tilesetdata->_tileSize.y * scale)) * (_tilesetdata->_tileSize.y * scale);
                        GuiDrawRectangle({ result.x + h.x, result.y + h.y, _tilesetdata->_tileSize.x * scale, _tilesetdata->_tileSize.y * scale }, 0, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), Color{ 255, 255, 255, 100 });
                    }
                    // */
                    int linesV = h.width / tileSetcolumns + 1;
                    int linesH = h.height / tileSetrows + 1;

                    for (int i = 1; i < tileSetcolumns; i++) DrawLine(h.x + linesV * i, h.y, h.x + linesV * i, h.y + h.height, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
                    for (int i = 1; i < tileSetrows; i++) DrawLine(h.x, h.y + linesH * i, h.x + h.width, h.y + linesH * i, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

                    DrawRectangleLinesEx(h, 1, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
                }

                Rectangle h = moveRectangle(tileseteditorstate.layoutRecs[0], tileseteditorstate.anchor);
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                h.height = 20;
                h.width -= 2;

                GuiDrawRectangle(h, 0, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE)));
                if (ActionDropdown000EditMode) GuiLock();
                int selected = -1;
                if (GuiActionDropDown(h, "File;#08#New TileSet;#05#Open TileSet;#02#Save TileSet", &ActionDropdown000EditMode, &selected)) {
                    ActionDropdown000EditMode = !ActionDropdown000EditMode;
                    if (selected == 1) {
                        fileExplorerListenerState = NEW_TILESET;
                        fileexplorerstate.active = true;
                        fileextension = ".png";
                        fileexplorerpath = "\\resources";
                    }
                    else if (selected == 2) {
                        fileExplorerListenerState = OPEN_TILESET;
                        fileexplorerstate.active = true;
                        fileextension = ".tsd";
                        fileexplorerpath = "\\resources";
                    }
                    else if (selected == 3 && _tilesetdata != nullptr) _tilesetdata->save();
                }

                if (_tilesetdata != nullptr) {
                    h.width = h.height * 4;
                    h.x += GetTextWidth("File") + GetTextWidth("Columns") + 32;
                    if (GuiSpinner(h, "Columns", &tileSetcolumns, 1, 20, Spinner002EditMode)) Spinner002EditMode = !Spinner002EditMode;
                    _tilesetdata->setColumns(tileSetcolumns);

                    h.x += GetTextWidth("Columns") + GetTextWidth("Rows") + 4 + h.width / 2;
                    if (GuiSpinner(h, "Rows", &tileSetrows, 1, 20, Spinner003EditMode)) Spinner003EditMode = !Spinner003EditMode;
                    _tilesetdata->setRows(tileSetrows);
                }

                GuiUnlock();

                if (fileExplorerListenerState == OPEN_TILESET && fileexplorerreturnpath != "") {
                    _tilesetdata = Resources::getInstance()->tileset(fileexplorerreturnpath, true);
                    fileexplorerreturnpath = "";
                    openTileSetFilePath = false;
                    tileSetcolumns = (int)_tilesetdata->_nTiles.x;
                    tileSetrows    = (int)_tilesetdata->_nTiles.y;
                }

                if (fileExplorerListenerState == NEW_TILESET && fileexplorerreturnpath != "") {
                    _tilesetdata = make_shared<TileSetData>();
                    _tilesetdata->setTexture(fileexplorerreturnpath);
                    tileSetcolumns = (int)_tilesetdata->_nTiles.x;
                    tileSetrows    = (int)_tilesetdata->_nTiles.y;
                    fileexplorerreturnpath = "";
                    openTileSetFilePath = false;
                }

            }

            if (GuiDraggableWindowBox(&animationeditorstate)) {
                Rectangle h = moveRectangle(animationeditorstate.layoutRecs[0], animationeditorstate.anchor);
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                h.height = 20;
                h.width -= 2;

                if (_animationdata != nullptr){
                    if (_animationdata->_sequence.size() > 0) {
                        int actual = 0;
                        _animationdata_sel = false;
                        Rectangle item{ h.x + 25.0f, h.y + 60.0f, (h.width / _animationdata->_sequence.size()) - 50.0f, 20 };
                        for (auto i : _animationdata->_sequence) {
                            if (GuiTextBox(item, animationedittingvalues[actual].value, 128, actual == _animationdata_editing)) {
                                _animationdata_editing = actual == _animationdata_editing ? -1 : actual;
                                if (actual != _animationdata_editing) _animationdata->setTime(actual, stof(animationedittingvalues[actual].value));
                            }

                            Vector2 aux = (*_animationdata->_data)[i.first];
                            Rectangle src = Rectangle{ aux.x * _animationdata->_data->_tileSize.x, aux.y * _animationdata->_data->_tileSize.y, _animationdata->_data->_tileSize.x, _animationdata->_data->_tileSize.y };
                            Rectangle dst = Rectangle{ item.x, item.y + 20.0f, item.width, item.width };

                            _animationdata_sel = CheckCollisionPointRec(GetMousePosition(), dst);
                            if (_animationdata_sel) _animationdata_selecting = actual;

                            if (_animationdata_selecting == actual) {
                                src = { 0, 0, (float)_animationdata->_data->_texture->width, (float)_animationdata->_data->_texture->height };
                                Rectangle dst_aux = dst;
                                float scale = dst.width / src.width;
                                dst_aux.height = src.height * scale;

                                Vector2 result{ GetMousePosition().x - dst.x, GetMousePosition().y - dst.y };
                                result.x = (int)(result.x / (_animationdata->_data->_tileSize.x * scale)) * (_animationdata->_data->_tileSize.x * scale);
                                result.y = (int)(result.y / (_animationdata->_data->_tileSize.y * scale)) * (_animationdata->_data->_tileSize.y * scale);
                                GuiDrawRectangle({ result.x + dst.x, result.y + dst.y, _animationdata->_data->_tileSize.x * scale, _animationdata->_data->_tileSize.y * scale }, 1, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), Color{ 0, 0, 0, 0 });

                                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                    Vector2 p{ result.x / (_animationdata->_data->_tileSize.x * scale), result.y / (_animationdata->_data->_tileSize.y * scale) };
                                    _animationdata->_sequence[actual].first = p.x + (int)p.y * (int)_animationdata->_data->_nTiles.x;
                                }

                                DrawTexturePro(*_animationdata->_data->_texture, src, dst_aux, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
                            }
                            else DrawTexturePro(*_animationdata->_data->_texture, src, dst, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);

                            GuiDrawRectangle(dst, 1, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), Color{ 0, 0, 0, 0 });
                            item.x += item.width + 50;
                            actual++;
                        }
                        if (!_animationdata_sel) _animationdata_selecting = -1;
                    }
                    if (GuiInputString({ h.x + 25.0f, h.y + 30.0f, h.width / 4.0f, 20.0f }, &animationnameinputstate)) _animationdata->_name = animationnameinputstate.value;
                }

                const char* ActionBar000Matrix[2] = {
                    "File;#08#New Animation;#05#Open Animation;#02#Save Animation",
                    "Edit;#05#Select TileSet;#66#New Point"
                };

                if ((ActionBar001Result = GuiActionPane(h, ActionBar000Matrix, 2, &ActionBar001EditMode)) != -1) {
                    if (ActionBar001EditMode == 0) {
                        if (ActionBar001Result == 0) {
                            animationedittingvalues.clear();
                            _animationdata = make_shared<EngineAnimation>();
                        }
                        else if (ActionBar001Result == 1) {
                            fileExplorerListenerState = OPEN_ANIMATION;
                            fileexplorerstate.active = true;
                            fileextension = ".aon";
                            fileexplorerpath = "\\resources";
                        }
                        else if (ActionBar001Result == 2) _animationdata->save();
                    }
                    else if (ActionBar001EditMode == 1) {
                        if (ActionBar001Result == 0) {
                            fileExplorerListenerState = SELECT_TILESET;
                            fileexplorerstate.active = true;
                            fileextension = ".tsd";
                            fileexplorerpath = "\\resources";
                        }
                        else if (ActionBar001Result == 1 && _animationdata != nullptr) {
                            _animationdata->addPoint(-1, 0.5f);
                            animationedittingvalues.push_back(InitGuiInputString("", "0.5"));
                        }
                    }
                    ActionBar001EditMode = -1;
                }

                if (fileExplorerListenerState == SELECT_TILESET && fileexplorerreturnpath != "") {
                    _animationdata = make_shared<EngineAnimation>(Resources::getInstance()->tileset(fileexplorerreturnpath, true));
                    fileexplorerreturnpath = "";
                    openTileSetFilePath = false;
                }
                if (fileExplorerListenerState == OPEN_ANIMATION && fileexplorerreturnpath != "") {
                    _animationdata = Resources::getInstance()->animation(fileexplorerreturnpath, true);
                    strcpy(animationnameinputstate.value, _animationdata->_name.c_str());
                    for (auto i : _animationdata->_sequence) animationedittingvalues.push_back(InitGuiInputString("", ftostr(i.second, 3).c_str()));

                    fileexplorerreturnpath = "";
                    openTileSetFilePath = false;
                }
            }

            if (GuiDraggableWindowBox(&animationgraphstate)) {
                Rectangle h = moveRectangle(animationgraphstate.layoutRecs[0], animationgraphstate.anchor);
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                h.width -= 240;
                h.height -= RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 20;

                // GuiGraphRenderState prev = graphrenderstate;

                graphrenderstate.layout = h;
                GuiGraphRender(&graphrenderstate);

                h = moveRectangle(animationgraphstate.layoutRecs[0], animationgraphstate.anchor);
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                h.height = 20;
                h.width -= 2;

                const char* ActionBar000Matrix[2] = {
                    "File;#08#New Animation;#05#Open Animation;#02#Save Animation",
                    "Edit;#50#New Vertex;#174#New Edge"
                };
                if ((ActionBar002Result = GuiActionPane(h, ActionBar000Matrix, 2, &ActionBar002EditMode)) != -1) {
                    RenderGraphEditingState act = RENDER_GRAPH_STATE_NULL;
                    if (ActionBar002EditMode == 0) {
                        if (ActionBar002Result == 0) {
                            animation_graph = EngineAnimationGraph();
                            graphrenderstate.graph = animation_graph;
                            graphrenderstate.editting = RENDER_GRAPH_STATE_NULL;
                            graphrenderstate.begin = graphrenderstate.end = graphrenderstate.name = "";
                        }
                        else if (ActionBar002Result == 1) {
                            fileExplorerListenerState = OPEN_ANIMATION_GRAPH;
                            fileexplorerstate.active = true;
                            fileextension = ".aph";
                            fileexplorerpath = "\\resources";
                        }
                        else if (ActionBar002Result == 2) {
                            animation_graph = graphrenderstate.graph;
                            animation_graph.save();
                        }
                    }
                    else if (ActionBar002EditMode == 1) {
                        if (ActionBar002Result == 0) {
                            fileExplorerListenerState = SELECT_ANIMATION_VERTEX;
                            fileexplorerstate.active = true;
                            fileextension = ".aon";
                            fileexplorerpath = "\\resources";
                            fileexplorerreturnpath = "";
                        }
                        else if (ActionBar002Result == 1) {
                            graphrenderstate.editting = RENDER_GRAPH_EXPECTING_FIRST;
                            act = RENDER_GRAPH_EXPECTING_FIRST;
                        }
                    }
                    // if (act == RENDER_GRAPH_STATE_NULL) graphrenderstate = prev;
                    ActionBar002EditMode = -1;
                }

                if (fileExplorerListenerState == SELECT_ANIMATION_VERTEX && fileexplorerreturnpath != "") {
                    shared_ptr<EngineAnimation> anim = Resources::getInstance()->animation(fileexplorerreturnpath, true);
                    graphrenderstate.graph.graph.AddVertex(anim->_name.c_str());
                    graphrenderstate.graph._animations[anim->_name] = Resources::getInstance()->animation(anim->_path, true);

                    for (auto i : graphrenderstate.graph._animations) cout << i.first << ' ';
                    cout << '\n';

                    fileexplorerreturnpath = "";
                    openTileSetFilePath = false;
                    fileExplorerListenerState = FILE_EXPLORER_NULL;
                }

                RenderGraphEditingState editting = graphrenderstate.editting;

                h = moveRectangle(animationgraphstate.layoutRecs[0], animationgraphstate.anchor);
                h = moveRectangle(h, { animationgraphstate.layoutRecs[0].width - 241 , RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 20 });
                h.width   = 240;
                h.height -= RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 21;
                GuiDrawRectangle(h, 0, GetColor(GuiGetStyle(DROPDOWNBOX, BORDER)), GetColor(GuiGetStyle(DROPDOWNBOX, BASE)));

                h.height = 20;
                GuiDrawRectangle(h, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_COLOR_NORMAL)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
                GuiDrawText("Inspector", GetTextBounds(STATUSBAR, h), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

                h.y += 21;
                string text = "STATUS: ";
                text += editting == RENDER_GRAPH_EXPECTING_NAME ? "Expecting Name" : editting == RENDER_GRAPH_EDITTING_VERTEX ? "Editting Vertex Name" : editting == RENDER_GRAPH_EDITTING_EDGE ? "Editting Edge Name" : editting == RENDER_GRAPH_EDITTING_DESTINATION ?  "Editting Destination" : "---------";
                GuiDrawText(text.c_str(), GetTextBounds(STATUSBAR, h), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

                h.y += 21;
                if (graphrenderstate.begin != "" && editting != RENDER_GRAPH_EDITTING_VERTEX) {
                    GuiDrawText(("Start: " + graphrenderstate.begin).c_str(), GetTextBounds(STATUSBAR, h), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
                    h.y += 21;
                }
                if (graphrenderstate.end != "" && editting != RENDER_GRAPH_EDITTING_VERTEX) {
                    GuiDrawText(("End:   " + graphrenderstate.end).c_str(), GetTextBounds(STATUSBAR, h), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
                    h.y += 21;
                }
                if (graphrenderstate.name != "" && editting != RENDER_GRAPH_EDITTING_EDGE) {
                    GuiDrawText(("Label: " + graphrenderstate.name).c_str(), GetTextBounds(STATUSBAR, h), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
                    h.y += 21;
                }

                if (editting == RENDER_GRAPH_EXPECTING_NAME || editting == RENDER_GRAPH_EDITTING_EDGE) {
                    string text = "";
                    auto i = graphrenderstate.graph._variables.begin();
                    while (i != graphrenderstate.graph._variables.end()) {
                        if(i == --graphrenderstate.graph._variables.end()) text = text + i->first.c_str();
                        else text = text + i->first.c_str() + ';';
                        i++;
                    }
                    GuiComboBox({ h.x + 10, h.y + 1, h.width - 20, h.height }, text.c_str(), &_animationgraph_editting_label_v);
                    h.y += 21;
                    GuiComboBox({ h.x + 10, h.y + 1, h.width - 20, h.height }, ">;<;>=;<=;==;!=", &_animationgraph_editting_label_c);
                    h.y += 21;
                    if(GuiSpinner({ h.x + 10, h.y + 1, h.width - 20, h.height }, "", &_animationgraph_editting_label_e, -1000, 1000, _animationgraph_editting_label_x)) _animationgraph_editting_label_x = !_animationgraph_editting_label_x;
                    h.y += 21;
                    if (GuiButton({ h.x + 10, h.y + 1, h.width - 20, h.height }, "Accept")) {
                        auto a = graphrenderstate.graph._variables.begin();
                        advance(a, _animationgraph_editting_label_v);
                        string name = a->first + " " + splice(">;<;>=;<=;==;!=", ';')[_animationgraph_editting_label_c] + " " + to_string(_animationgraph_editting_label_e);

                        if (editting == RENDER_GRAPH_EXPECTING_NAME) graphrenderstate.graph.graph.AddEgde(graphrenderstate.begin, graphrenderstate.end, name);
                        else if (editting == RENDER_GRAPH_EDITTING_EDGE) graphrenderstate.graph.graph.ChangeEdge(graphrenderstate.begin, graphrenderstate.end, name);
                    }
                    h.y += 21;

                }

                if (editting == RENDER_GRAPH_EDITTING_VERTEX) {
                    Rectangle aux = moveRectangle(h, { 8, 0 });
                    aux.width -= 20;
                    if (GuiButton(aux, "Remove Vertex")) {
                        graphrenderstate.graph.graph.RemoveVertex(graphrenderstate.begin);
                        graphrenderstate.editting = RENDER_GRAPH_STATE_NULL;
                        graphrenderstate.begin = graphrenderstate.end = graphrenderstate.name = "";
                    }
                    h.y += 21;
                }

                if (editting == RENDER_GRAPH_EDITTING_EDGE) {
                    Rectangle aux = moveRectangle(h, { 8, 0 });
                    aux.width -= 16;
                    if (GuiButton(aux, "Remove Edge")) {
                        graphrenderstate.graph.graph.RemoveEgde(graphrenderstate.begin, graphrenderstate.end);
                        graphrenderstate.editting = RENDER_GRAPH_STATE_NULL;
                        graphrenderstate.begin = graphrenderstate.end = graphrenderstate.name = "";
                    }
                    h.y += 21;
                }

                h = moveRectangle(animationgraphstate.layoutRecs[0], animationgraphstate.anchor);
                h = moveRectangle(h, { animationgraphstate.layoutRecs[0].width - 231 , animationgraphstate.layoutRecs[0].height - 25 });
                h.width = 220;
                h.height = 20;

                if (GuiButton(h, "Add Variable")) graphrenderstate.graph._variables["var" + to_string(graphrenderstate.graph._variables.size() + 1)] = 0;
                h.y -= 21;
                string to_delete = "";
                for (auto i : graphrenderstate.graph._variables) {
                    if (GuiButton({ h.x, h.y, h.height, h.height }, "#113#")) to_delete = i.first;
                    GuiDrawText(i.first.c_str(), moveRectangle(h, {h.height + 5, 0}), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));
                    h.y -= 21;
                }

                if (to_delete != "") graphrenderstate.graph._variables.erase(to_delete);

                if (IsKeyPressed(KEY_ESCAPE)) {
                    graphrenderstate.editting = RENDER_GRAPH_STATE_NULL;
                    graphrenderstate.begin = graphrenderstate.end = graphrenderstate.name = "";
                }
            }

            if (GuiDraggableWindowBox(&fileexplorerstate)) {
                Rectangle h = moveRectangle(fileexplorerstate.layoutRecs[0], fileexplorerstate.anchor);
                h = moveRectangle(h, { 1, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT });
                h.height = 20;
                h.width -= 2;
                int i = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
                int j = GuiGetStyle(BUTTON, BORDER_WIDTH);
                GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(BUTTON, BORDER_WIDTH, 0);

                std::string base_path = fs::current_path().string() + fileexplorerpath;
                for (const auto& entry : fs::recursive_directory_iterator(base_path)) {
                    std::string text = entry.path().string();
                    if (entry.path().extension() == fileextension) {
                        if (GuiButton(h, text.substr(text.find(fileexplorerpath), text.size()).c_str())) {
                            fileexplorerreturnpath = text.substr(text.find(fileexplorerpath) + 1, text.size());
                            fileexplorerstate.active = false;
                        }
                        h.y += h.height + 1;
                    }
                }
                GuiSetStyle(BUTTON, TEXT_ALIGNMENT, i);
                GuiSetStyle(BUTTON, BORDER_WIDTH, j);
            }

            const char* ActionBar000Matrix[3] = {
                "File;#08#New Scene;#05#Open Scene;#02#Save Scene;#159#Exit",
                "Edit;#162#New GameObject;#17#Cut;#16#Copy;#18#Paste",
                "Window;#198#Hierarchy;#198#Inspector;#198#Tileset Editor;#198#Animation Editor;#198#Animation Graph Editor"
            };

            if ((ActionBar000Result = GuiActionPane({ 0.0f, 0.0f, (float)GetScreenWidth(), 20.0f }, ActionBar000Matrix, 3, &ActionBar000EditMode)) != -1) {
                if (ActionBar000EditMode == 0) {
                    if (ActionBar000Result == 0) {
                        scene = new Scene();
                        hierarchyTree = convertToNewTree(*scene->getRoot());
                        _game_object = nullptr;
                    }
                    else if (ActionBar000Result == 1) {
                        fileExplorerListenerState = OPEN_SCENE;
                        fileexplorerstate.active = true;
                        fileextension = ".scn";
                        fileexplorerpath = "\\resources";
                    }
                    else if (ActionBar000Result == 2 && scene != nullptr) scene->save();
                    else if (ActionBar000Result == 3) windowShouldClose = true; 
                }
                else if (ActionBar000EditMode == 1){
                    if (ActionBar000Result == 0) {
                        Nary<GameObject> child = Nary<GameObject>(make_shared<GameObject>(Vector2{ 0, 0 }, Vector2{ 10, 10 }, "New GameObject"));
                        if (_game_object != nullptr) scene->getRoot()->find(_game_object).setChild(child);
                        else scene->getRoot()->setBrother(child);
                        hierarchyTree = convertToNewTree(*scene->getRoot());
                    }
                    else if (ActionBar000Result == 1 && tmp_node != nullptr) { 
                        copied_node = tmp_node; 
                        scene->getRoot()->removeNode(tmp_node->content());
                        hierarchyTree = convertToNewTree(*scene->getRoot());
                    }
                    else if (ActionBar000Result == 2) copied_node = tmp_node;
                    else if (ActionBar000Result == 3 && copied_node != nullptr) {
                        if (_game_object != nullptr) scene->getRoot()->find(_game_object).setChild(makeCopy(*copied_node));
                        else scene->getRoot()->setBrother(makeCopy(*copied_node));
                        hierarchyTree = convertToNewTree(*scene->getRoot());
                    }
                }
                else if(ActionBar000EditMode == 2){
                    if(ActionBar000Result == 0) hierarchystate.active = true;
                    else if (ActionBar000Result == 1) inspectorstate.active = true;
                    else if (ActionBar000Result == 2) tileseteditorstate.active = true;
                    else if (ActionBar000Result == 3) animationeditorstate.active = true;
                    else if (ActionBar000Result == 4) animationgraphstate.active = true;
                    else if (ActionBar000Result == 5) animationgraphstate.active = true;
                }
                ActionBar000EditMode = -1;
            }
            GuiToggleGroup({ (int)GetScreenWidth() / 2.0f - 45.0f, 0.0f, 30.0f, 20.0f }, "#131#;#132#;#133#", &scenestate);
            GuiDrawRectangle({ 0.0f, GetScreenHeight() - 20.0f, (float)GetScreenWidth(), 20.0f }, 0, WHITE, Color{ 44, 44, 44, 255 });

            GuiLabel({ GetScreenWidth() - 430.0f, GetScreenHeight() - 20.0f, 100.0f, 20.0f }, "Mouse Position: ");
            GuiLabel({ GetScreenWidth() - 330.0f, GetScreenHeight() - 20.0f, 50.0f, 20.0f }, ("x:" + ftostr(GetMousePosition().x - initial.x, 0)).c_str());
            GuiLabel({ GetScreenWidth() - 280.0f, GetScreenHeight() - 20.0f, 50.0f, 20.0f }, ("y:" + ftostr(GetMousePosition().y - initial.y, 0)).c_str());

            // Setting compiling info
            double time = GetTime();
            bool bCompiling = g_pSys->pRuntimeObjectSystem->GetIsCompiling();
            double timeSinceLastCompile = time - compileEndTime;

            if (bCompiling || timeSinceLastCompile < SHOW_AFTER_COMPILE_TIME) {
                if (bCompiling) {
                    if (timeSinceLastCompile > SHOW_AFTER_COMPILE_TIME) compileStartTime = time;
                    compileEndTime = time; // ensure always updated
                }

                bool bCompileOk = g_pSys->pRuntimeObjectSystem->GetLastLoadModuleSuccess();

                Color windowBgCol = GREEN;
                if (!bCompiling) {
                    if (bCompileOk) windowBgCol = GREEN;
                    else windowBgCol = RED;
                }
                char text[80];

                if (bCompiling) sprintf(text, "#16#Compiling... time %.2fs", (float)(time - compileStartTime));
                else {
                    if (bCompileOk) sprintf(text, "#112#Compiling... time %.2fs. SUCCEED", (float)(compileEndTime - compileStartTime));
                    else sprintf(text, "#113#Compiling... time %.2fs. FAILED", (float)(compileEndTime - compileStartTime));
                }
                GuiLabel({ GetScreenWidth() - 230.0f, GetScreenHeight() - 20.0f, 230.0f, 20.0f }, text);
            }
        EndDrawing();

        // Do not add any code after this point as Undo/Redo will delete this
        if (doRCCppUndo) g_pSys->pRuntimeObjectSystem->GetObjectFactorySystem()->UndoObjectConstructorChange();
        if (doRCCppRedo) g_pSys->pRuntimeObjectSystem->GetObjectFactorySystem()->RedoObjectConstructorChange();
    }

    bool WindowShouldClose() {
        return windowShouldClose;
    }

    Nary<GameObject> makeCopy(const Nary<GameObject>& a) {
        if (!a.isEmpty()) {
            shared_ptr<GameObject> aux = make_shared<GameObject>(*a.content());
            for (auto i : a.content()->getBehaviours()) aux->addBehaviour(i->clone());
            Nary<GameObject> res(aux);

            for (int i = 1; i < a.nChilds(); i++) res.setChild(makeCopy(a.child(i)));

            return res;
        }
        else return Nary<GameObject>();
    }

    void edit_tilemap(shared_ptr<TileMap> tilemap) {
        editting_tilemap = !editting_tilemap;
        tilemapedittingstate = InitGuiTileMapEdittingState(tilemap);
    }
};

REGISTERSINGLETON(RCCppMainLoop, true); 
