#include "platformerEngineBasics.h"
RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes

#include "SystemTable.h"
// #include "PlayerController.h"
// #include "Rigidbody.h"
#include <set>

namespace fs = std::filesystem;

class Viewer{
public: 
	virtual void draw() = 0;
};

class Observer{ 
public: 
    virtual void update(Rectangle* bounds) = 0;
};

class ViewerGameObject : public Viewer, public Observer {
private:
	shared_ptr<GameObject> _go;

    bool editMode, edittingx, edittingy, edittingwidth, edittingheight;
    char text[100], posx[100], posy[100], sizex[100], sizey[100];

    GuiInputVector2State PositionEditor;
    GuiInputVector2State SizeEditor;

    Rectangle bounds;
public: 
	ViewerGameObject(shared_ptr<GameObject> go) {
        _go = go;
        strcpy(text, _go->name().c_str());

        PositionEditor = InitGuiInputVector2("Position", _go->_offset);
        SizeEditor     = InitGuiInputVector2("Size", _go->_size);
        strcpy(posx, ftostr(_go->_offset.x, 3).c_str());
        strcpy(posy, ftostr(_go->_offset.y, 3).c_str());
        strcpy(sizex, ftostr(_go->_size.x, 3).c_str());
        strcpy(sizey, ftostr(_go->_size.y, 3).c_str());

        editMode = edittingx = edittingy = edittingwidth = edittingheight = false;
        bounds = { 0 };
    }

	void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        int aux = GuiGetStyle(TEXTBOX, BORDER_WIDTH);
        GuiSetStyle(TEXTBOX, BORDER_WIDTH, 0);
        if (GuiTextBox(bounds, text, 128, editMode)) {
            editMode = !editMode;
            if (!editMode) _go->setName(text);
        }
        GuiSetStyle(TEXTBOX, BORDER_WIDTH, aux);
        bounds.y += bounds.height + 1;

        Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Position", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        itemBounds = { itemBounds.x + itemBounds.width, bounds.y, (bounds.width - itemBounds.width) / 2 - 2, bounds.height };
        if (GuiTextBox(itemBounds, posx, 128, edittingx)) {
            edittingx = !edittingx;
            if (!edittingx) _go->_offset.x = stof(posx);
            else strcpy(posx, ftostr(_go->_offset.x, 3).c_str());
        }
        itemBounds = { bounds.x + bounds.width - itemBounds.width - 1, bounds.y, itemBounds.width, bounds.height };
        if (GuiTextBox(itemBounds, posy, 128, edittingy)) {
            edittingy = !edittingy;
            if (!edittingy) _go->_offset.y = stof(posy);
            else strcpy(posy, ftostr(_go->_offset.y, 3).c_str());
        }
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Size", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        itemBounds = { itemBounds.x + itemBounds.width, bounds.y, (bounds.width - itemBounds.width) / 2 - 2, bounds.height };
        if (GuiTextBox(itemBounds, sizex, 128, edittingwidth)) {
            edittingwidth = !edittingwidth;
            if (!edittingwidth) _go->_size.x = stof(sizex);
            else strcpy(sizex, ftostr(_go->_size.x, 3).c_str());
        }
        itemBounds = { bounds.x + bounds.width - itemBounds.width - 1, bounds.y, itemBounds.width, bounds.height };
        if (GuiTextBox(itemBounds, sizey, 128, edittingheight)) {
            edittingheight = !edittingheight;
            if (!edittingheight) _go->_size.y = stof(sizey);
            else strcpy(sizey, ftostr(_go->_size.y, 3).c_str());
        }
        bounds.y += bounds.height + 1;

        /*
        if(GuiInputVector2(bounds, &PositionEditor)) _go->_offset = PositionEditor.values;
        bounds.y += bounds.height + 1;

        if(GuiInputVector2(bounds, &SizeEditor)) _go->_size = SizeEditor.values;
        bounds.y += bounds.height + 1;
        // */
	}

    void update(Rectangle *bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerLabel : public Viewer, public Observer {
private:
    shared_ptr<Label> _label;

    bool editMode, edit_font_size, selecting_color, options;
    char* text;
    int active, _font_size;

    Color color;
    GuiInputStringState editor;
    GuiDraggableWindowBoxState state;

    Rectangle bounds;
public:
    ViewerLabel(shared_ptr<GameObject> go) {
        _label = go->getBehaviour<Label>();
        text = new char[_label->text().length() + 1];
        strcpy(text, _label->text().c_str());

        editor = InitGuiInputString("Text", text);
        state  = InitGuiDraggableWindowBox(Rectangle{ GetScreenWidth() - 500.0f, 300, 205, 200 }, "Color Picker");
        editMode = edit_font_size = state.active = options = false;

        char c = _label->text_allign();
        if (c == 'l') active = 0;
        else if (c == 'c') active = 1;
        else active = 2;

        _font_size = _label->font_size();
        color = _label->color();

        bounds = { 0 };
    }

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("Label", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };
        bounds.y += bounds.height + 1;

        GuiInputString(bounds, &editor);
        if (!editor.editting) _label->setText(editor.value);
        bounds.y += bounds.height + 1;

        Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Text allign", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.height, bounds.height };

        GuiToggleGroup(itemBounds, "L;C;R", &active);
        _label->setAllign(active == 0? 'l' : active == 1? 'c' : 'r');
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Font size", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, itemBounds.width + 15, bounds.height };
        if (GuiSpinner(itemBounds, "", &_font_size, 0, 100, edit_font_size)) edit_font_size = !edit_font_size;
        _label->setFontSize(_font_size);
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Color", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };
        if (GuiButton(itemBounds, NULL)) state.active = true;

        GuiDrawRectangle(itemBounds, 1, GetColor(GuiGetStyle(TEXTBOX, BORDER + (STATE_DISABLED * 3))), _label->color());
        itemBounds = { itemBounds.x, itemBounds.y + itemBounds.height - 5, itemBounds.width - 1, 5 };
        GuiDrawRectangle(itemBounds, 0, GetColor(GuiGetStyle(TEXTBOX, BORDER + (STATE_DISABLED * 3))), WHITE);
        itemBounds.width = itemBounds.width * _label->color().r / 255;
        GuiDrawRectangle(itemBounds, 0, GetColor(GuiGetStyle(TEXTBOX, BORDER + (STATE_DISABLED * 3))), BLACK);

        if (GuiDraggableWindowBox(&state)) {
            Rectangle h = moveRectangle(state.bounds, state.anchor);
            h = moveRectangle(h, { 5, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 5 });
            h.width  -= 35;
            h.height -= 35;
            GuiColorPicker(h, NULL, &color);
            _label->setColor(color);
        }

        bounds.y += bounds.height + 1;
    }

    void update(Rectangle *bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }

};

class ViewerSprite : public Viewer, public Observer {
private:
    shared_ptr<Sprite> _sprite;

    bool selecting_color, options;

    string filename, fileexplorerreturnpath, fileextension, fileexplorerpath;
    shared_ptr<Texture2D> texture;
    Color color;
    GuiDraggableWindowBoxState state;
    GuiFileExplorerListener fileExplorerListenerState;
    GuiDraggableWindowBoxState fileexplorerstate;

    Rectangle bounds;
public:
    ViewerSprite(shared_ptr<GameObject> go) {
        _sprite = go->getBehaviour<Sprite>();

        filename = _sprite->filename();

        state  = InitGuiDraggableWindowBox(Rectangle{ GetScreenWidth() - 500.0f, 300, 205, 200 }, "Color Picker");
        fileexplorerstate = InitGuiDraggableWindowBox(Rectangle{ 424, 400, 408, 184 }, "File Explorer");
        options = fileexplorerstate.active = false;
        fileExplorerListenerState = FILE_EXPLORER_NULL;

        color = _sprite->color();

        state.active = options = false;
    }

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("Sprite", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };
        bounds.y += bounds.height + 1;

        Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Filename", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };

        int align = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiButton(itemBounds, _sprite->filename() == "" ? "No File Selected" : _sprite->filename().c_str())) {
            fileExplorerListenerState = SELECT_ANIMATION;
            fileexplorerstate.active = true;
            fileextension = ".png";
            fileexplorerpath = "/resources";
        }
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, align);
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Color", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };
        if (GuiButton(itemBounds, NULL)) state.active = true;

        GuiDrawRectangle(itemBounds, 1, GetColor(GuiGetStyle(TEXTBOX, BORDER + (STATE_DISABLED * 3))), _sprite->color());
        itemBounds = { itemBounds.x, itemBounds.y + itemBounds.height - 5, itemBounds.width - 1, 5 };
        GuiDrawRectangle(itemBounds, 0, GetColor(GuiGetStyle(TEXTBOX, BORDER + (STATE_DISABLED * 3))), WHITE);
        itemBounds.width = itemBounds.width * _sprite->color().r / 255 - 1;
        GuiDrawRectangle(itemBounds, 0, GetColor(GuiGetStyle(TEXTBOX, BORDER + (STATE_DISABLED * 3))), BLACK);

        if (GuiDraggableWindowBox(&state)) {
            Rectangle h = moveRectangle(state.bounds, state.anchor);
            h = moveRectangle(h, { 5, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + 5 });
            h.width -= 35;
            h.height -= 35;
            GuiColorPicker(h, NULL, &color);
            _sprite->setcolor(color);
        }

        if (fileExplorerListenerState == SELECT_ANIMATION && fileexplorerreturnpath != "") {
            cout << fileexplorerpath << '\n';
            _sprite->settexture(Resources::getInstance()->texture(fileexplorerreturnpath));
            _sprite->setfilename(fileexplorerreturnpath);
            fileexplorerreturnpath = "";
            fileExplorerListenerState = FILE_EXPLORER_NULL;
            fileexplorerstate.active = false;
        }

        if (GuiDraggableWindowBox(&fileexplorerstate)) {
            Rectangle h = moveRectangle(fileexplorerstate.bounds, fileexplorerstate.anchor);
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

        bounds.y += bounds.height + 1;
    }

    void update(Rectangle *bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerAnimatedSprite : public Viewer, public Observer {
private:
    shared_ptr<AnimatedSprite> _animated_sprite;

    string filename, fileexplorerreturnpath, fileextension, fileexplorerpath;
    bool edit_frame_interval, fliph, flipv, options;

    GuiInputVector2State SizeEditor;
    GuiFileExplorerListener fileExplorerListenerState;
    GuiDraggableWindowBoxState fileexplorerstate;
    Rectangle bounds;
public:
    ViewerAnimatedSprite(shared_ptr<GameObject> go) {
        _animated_sprite = go->getBehaviour<AnimatedSprite>();
        filename = _animated_sprite->filename();
        fileexplorerstate = InitGuiDraggableWindowBox(Rectangle{ 424, 400, 408, 184 }, "File Explorer");
        edit_frame_interval = options = fileexplorerstate.active = false;
        fliph = _animated_sprite->fliph();
        flipv = _animated_sprite->flipv();
        fileExplorerListenerState = FILE_EXPLORER_NULL;
    }

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("Animated Sprite", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };
        bounds.y += bounds.height + 1;
        
        Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Filename", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };

        int align = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiButton(itemBounds, _animated_sprite->filename() == "" ? "No File Selected" : _animated_sprite->filename().c_str())) {
            fileExplorerListenerState = SELECT_ANIMATION;
            fileexplorerstate.active = true;
            fileextension = ".aon";
            fileexplorerpath = "/";
        }
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, align);
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 2, bounds.y, 85, bounds.height };
        GuiDrawText("Flip", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, itemBounds.height, bounds.height };
        GuiCheckBox(itemBounds, NULL, &fliph);
        itemBounds = { itemBounds.x + itemBounds.width + 5, bounds.y, itemBounds.height, bounds.height };
        GuiCheckBox(itemBounds, NULL, &flipv);
        _animated_sprite->fliph(fliph);
        _animated_sprite->flipv(flipv);
        bounds.y += bounds.height + 1;

        if (fileExplorerListenerState == SELECT_ANIMATION && fileexplorerreturnpath != "") {
            cout << fileexplorerpath << '\n';
            _animated_sprite->setAnimation(fileexplorerreturnpath);
            fileexplorerreturnpath = "";
            fileExplorerListenerState = FILE_EXPLORER_NULL;
            fileexplorerstate.active = false;
        }

        if (GuiDraggableWindowBox(&fileexplorerstate)) {
            Rectangle h = moveRectangle(fileexplorerstate.bounds, fileexplorerstate.anchor);
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
    }

    void update(Rectangle* bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerAnimator : public Viewer, public Observer {
private:
    shared_ptr<Animator> behavior;

    string fileexplorerreturnpath, fileextension, fileexplorerpath;
    Rectangle bounds;
    GuiFileExplorerListener fileExplorerListenerState;
    GuiDraggableWindowBoxState fileexplorerstate;
    bool options;
public:
    ViewerAnimator(shared_ptr<GameObject> go) {
        behavior = go->getBehaviour<Animator>();
        fileexplorerstate = InitGuiDraggableWindowBox(Rectangle{ 424, 400, 408, 184 }, "File Explorer");
        options = fileexplorerstate.active = false;
        fileExplorerListenerState = FILE_EXPLORER_NULL;
    }

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("Animator", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        bounds.y += bounds.height + 1;
        Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Filename", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };

        int align = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiButton(itemBounds, behavior->filename() == "" ? "No File Selected" : behavior->filename().c_str())) {
            fileExplorerListenerState = OPEN_ANIMATION_GRAPH;
            fileexplorerstate.active = true;
            fileextension = ".aph";
            fileexplorerpath = "/";
        }
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, align);
        bounds.y += bounds.height + 1;

        if (fileExplorerListenerState == OPEN_ANIMATION_GRAPH && fileexplorerreturnpath != "") {
            cout << fileexplorerreturnpath << '\n';
            behavior->setGraph(fileexplorerreturnpath);
            fileexplorerreturnpath = "";
            fileExplorerListenerState = FILE_EXPLORER_NULL;
            fileexplorerstate.active = false;
        }

        if (GuiDraggableWindowBox(&fileexplorerstate)) {
            Rectangle h = moveRectangle(fileexplorerstate.bounds, fileexplorerstate.anchor);
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
    }

    void update(Rectangle* bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerTileMap : public Viewer, public Observer {
private:
    shared_ptr<TileMap> _tilemap;

    char scale[128];
    bool edit_scale, options;

    string filename, fileexplorerreturnpath, fileextension, fileexplorerpath;
    Rectangle bounds;
    GuiFileExplorerListener fileExplorerListenerState;
    GuiDraggableWindowBoxState fileexplorerstate;
public:
    ViewerTileMap(shared_ptr<GameObject> go) {
        _tilemap = go->getBehaviour<TileMap>();

        filename = _tilemap->filename();
        strcpy(scale, ftostr(_tilemap->scale(), 2).c_str());

        fileexplorerstate = InitGuiDraggableWindowBox(Rectangle{ 424, 400, 408, 184 }, "File Explorer");
        edit_scale = options = fileexplorerstate.active = false;
        fileExplorerListenerState = FILE_EXPLORER_NULL;
    }
    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("TileMap", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };
        bounds.y += bounds.height + 1;

        Rectangle itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("TileSet", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };
        GuiDrawText(_tilemap->tileSet() == nullptr ? "No File Selected" : (_tilemap->tileSet()->_path.substr(0, _tilemap->tileSet()->_path.find('.')) + ".tsd").c_str(), GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        int align = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        if (GuiButton(itemBounds, _tilemap->filename() == "" ? "No File Selected" : (_tilemap->tileSet()->_path.substr(0, _tilemap->tileSet()->_path.find('.')) + ".tsd").c_str())) {
            fileExplorerListenerState = SELECT_ANIMATION;
            fileexplorerstate.active = true;
            fileextension = ".tsd";
            fileexplorerpath = "/resources";
        }
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, align);
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("MetaData", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, bounds.width - 86, bounds.height };
        if (GuiButton(itemBounds, "Edit")) g_pSys->pRCCppMainLoopI->edit_tilemap(_tilemap);
        // GuiDrawText(filename == "" ? "No File Selected" : filename.c_str(), GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        bounds.y += bounds.height + 1;

        itemBounds = { bounds.x + 1, bounds.y, 85, bounds.height };
        GuiDrawText("Scale", GetTextBounds(DEFAULT, itemBounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        itemBounds = { itemBounds.x + itemBounds.width - 1, bounds.y, itemBounds.width + 15, bounds.height };
        if (GuiTextBox(itemBounds, scale, 128, edit_scale)) {
            edit_scale = !edit_scale;
            if (!edit_scale) _tilemap->setScale(stof(scale));
        }
        bounds.y += bounds.height + 1;

        if (fileExplorerListenerState == SELECT_ANIMATION && fileexplorerreturnpath != "") {
            cout << fileexplorerreturnpath << '\n';
            _tilemap->setTileSet(fileexplorerreturnpath);
            fileexplorerreturnpath = "";
            fileExplorerListenerState = FILE_EXPLORER_NULL;
            fileexplorerstate.active = false;
        }

        if (GuiDraggableWindowBox(&fileexplorerstate)) {
            Rectangle h = moveRectangle(fileexplorerstate.bounds, fileexplorerstate.anchor);
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
    }
    void update(Rectangle* bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerBaseUI : public Viewer, public Observer {
private:
    shared_ptr<BaseUI> ui;

    Rectangle bounds;
    bool options;
public:
    ViewerBaseUI(shared_ptr<GameObject> go) {
        ui = go->getBehaviour<BaseUI>();
        options = false;
    }

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("BaseUI", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };
        bounds.y += bounds.height + 1;
    }

    void update(Rectangle* bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerPlayerController : public Viewer, public Observer {
private:
    shared_ptr<PlayerController> behavior;

    Rectangle bounds;
    bool options;
public:
    ViewerPlayerController(shared_ptr<GameObject> go) {
    behavior = go->getBehaviour<PlayerController>();
    options = false;
}

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("PlayerController", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };

        bounds.y += bounds.height + 1;
}

    void update(Rectangle * bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class ViewerRigidbody : public Viewer, public Observer {
private:
    shared_ptr<Rigidbody> behavior;

    Rectangle bounds;
    bool options;
public:
    ViewerRigidbody(shared_ptr<GameObject> go) {
    behavior = go->getBehaviour<Rigidbody>();
    options = false;
}

    void draw() {
        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));
        GuiDrawText("Rigidbody", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));

        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };

        bounds.y += bounds.height + 1;
}

    void update(Rectangle * bounds) {
        this->bounds = *bounds;
        draw();
        *bounds = this->bounds;
    }
};

class Subject {
    virtual void add(const shared_ptr<Observer>& o) = 0; 
    virtual void remove(const shared_ptr<Observer>& o) = 0; 
    virtual void notify(Rectangle bounds) = 0;
};

class Rastreator : public Subject{ 
private: 
    vector<shared_ptr<Observer>> _observers; 
    shared_ptr<GameObject> _game_object;
    Rectangle _bounds;
    vector<function<void()>> _actions;
    string _text;
    vector<bool> options;
    
    char input_text[50];
    bool open_behaviors, edit_behavior_name;
    int selected;

    void add(const shared_ptr<Observer>& o) {
        _observers.push_back(o);
    }
    void remove(const shared_ptr<Observer>& o) {
        int i = 0;
        bool found = false;

        while (i < _observers.size() && !found) {
            found = _observers[i] == o;
            if (!found) i++;
        }

        if (found) _observers.erase(_observers.begin() + i);
    }
public: 
    Rastreator() {
        open_behaviors = edit_behavior_name = false;
        _bounds = { 0 };
        selected = -1; 
        setCreators(vector<pair<string, function<void()>>>{
            { "New Sprite", bind(&Rastreator::addBehaviour<Sprite>, this, make_shared<Sprite>()) },
            { "New Animated Sprite", bind(&Rastreator::addBehaviour<AnimatedSprite>, this, make_shared<AnimatedSprite>()) },
            { "New Animator", bind(&Rastreator::addBehaviour<Animator>, this, make_shared<Animator>()) },
            { "New Tile Map", bind(&Rastreator::addBehaviour<TileMap>, this, make_shared<TileMap>()) },
            { "New BaseUI", bind(&Rastreator::addBehaviour<BaseUI>, this, make_shared<BaseUI>()) },
            { "New Label", bind(&Rastreator::addBehaviour<Label>, this, make_shared<Label>()) },
            { "New PlayerController", bind(&Rastreator::addBehaviour<PlayerController>, this, make_shared<PlayerController>()) },
            { "New Rigidbody", bind(&Rastreator::addBehaviour<Rigidbody>, this, make_shared<Rigidbody>()) },
            { "New Behaviour", bind(&Rastreator::ExpextBehaviourName, this) }
        });
    }
    Rastreator(const shared_ptr<GameObject>& r) {
        open_behaviors = false;
        _bounds = { 0 };
        selected = -1;
        setGameObject(r);
    }

    void draw() {
        Rectangle itemBounds = { _bounds.x + (_bounds.width - 125) / 2 + 5, _bounds.y, 125, _bounds.height };
        GuiDrawRectangle(_bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));

        Rectangle input_bounds = _bounds;
        input_bounds.y += input_bounds.height;
        if (edit_behavior_name){
            if (GuiTextBox(input_bounds, input_text, 50, edit_behavior_name)) {
                edit_behavior_name = !edit_behavior_name;
                CreateBehaviour(input_text);
            }
        }
        else GuiDrawRectangle(input_bounds, 0, WHITE, GetColor(GuiGetStyle(DROPDOWNBOX, BASE)));

        GuiSetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        if (open_behaviors) GuiLock();
        selected = -1;
        if (GuiActionDropDown(itemBounds, _text.c_str(), &open_behaviors, &selected)) {
            open_behaviors = !open_behaviors;
            if (selected != -1) _actions[selected - 1]();
        }
        GuiUnlock();
        GuiSetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    }
    void notify(Rectangle bounds) {
        vector<Rectangle> optionbounds;
        for (auto o : _observers) {
            if (auto j = dynamic_pointer_cast<ViewerGameObject>(o)) {}
            else optionbounds.push_back({ bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height });

            o->update(&bounds);
        }
        if(_game_object) draw();

        int i = 0;
        for (auto item : optionbounds) {
            bool p = options[i];
            int selected = -1;

            if (p) GuiLock();

            GuiSetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
            GuiSetStyle(ACTIONDROPDOWN, TEXT_PADDING, 0);
            if (GuiActionDropDown(item, "#140#;Move Up;Move Down;Close", &p, &selected)) {
                if (selected != -1) {
                    if (selected == 1) _game_object->moveBehaviourUp(i);
                    else if (selected == 2) _game_object->moveBehaviourDown(i);
                    else if (selected == 3) _game_object->removeBehaviour(i);
                    setGameObject(_game_object);
                }
                options[i] = !p;
            }
            GuiSetStyle(ACTIONDROPDOWN, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(ACTIONDROPDOWN, TEXT_PADDING, 16);

            GuiUnlock();
            i++;
        }

        _bounds = bounds;
    }
    void setCreators(vector<pair<string, function<void()>>> actions) {
        _text = "Add Behaviour";
        for (auto i : actions) addCreatorBack(i);
    }
    void addCreatorBack(pair<string, function<void()>> action) {
        _text += ";" + action.first;
        _actions.push_back(action.second);
    }
    void addCreator(pair<string, function<void()>> action) {
        vector<string> aux = splice(_text, ';');
        _text = "Add Behaviour";
        for (int i = 0; i < aux.size() - 1; i++) _text += ";" + aux[i];
        _text += ";" + action.first + ";" + aux.back();

        _actions.insert(_actions.end() - 1, action.second);
    }
    void setGameObject(const shared_ptr<GameObject>& r) {
        _game_object = r;
        _observers.clear();
        add(make_shared<ViewerGameObject>(_game_object));

        for (auto item : _game_object->getBehaviours()) {
            if (dynamic_pointer_cast<Label>(item) != nullptr) add(make_shared<ViewerLabel>(_game_object));
            else if (dynamic_pointer_cast<Sprite>(item) != nullptr) add(make_shared<ViewerSprite>(_game_object));
            else if (dynamic_pointer_cast<AnimatedSprite>(item) != nullptr) add(make_shared<ViewerAnimatedSprite>(_game_object));
            else if (dynamic_pointer_cast<Animator>(item) != nullptr) add(make_shared<ViewerAnimator>(_game_object));
            else if (dynamic_pointer_cast<TileMap>(item) != nullptr) add(make_shared<ViewerTileMap>(_game_object));
            else if (dynamic_pointer_cast<BaseUI>(item) != nullptr) add(make_shared<ViewerBaseUI>(_game_object));
            else if (dynamic_pointer_cast<PlayerController>(item) != nullptr) add(make_shared<ViewerPlayerController>(_game_object));
            else if (dynamic_pointer_cast<Rigidbody>(item) != nullptr) add(make_shared<ViewerRigidbody>(_game_object));
            options.push_back(false);
        }
    }
    template<typename N> void addBehaviour(shared_ptr<N> behavior) {
        /*if (auto j = dynamic_pointer_cast<Animator>(behavior)) {
            if (_game_object->getBehaviour<AnimatedSprite>() != nullptr) {
                _game_object->addBehaviour(behavior);
                setGameObject(_game_object);
            }
        }
        else {// */
            _game_object->addBehaviour(behavior);
            setGameObject(_game_object);
        // }
    }
    void ExpextBehaviourName() {
        edit_behavior_name = true;
        strcpy(input_text, "NewBehaviour");
    }
    void CreateBehaviour(string behavior_name){
        string upper, lower;
        ofstream file(behavior_name + ".h");

        upper = str_toupper(behavior_name);
        lower = str_tolower(behavior_name);

        file << "#ifndef __" << upper << "_H__\n#define __" << upper << "_H__\n\n#include \"RuntimeInclude.h\"\nRUNTIME_MODIFIABLE_INCLUDE;\n#include \"raylib.h\"\n#include \"platformerEngineBasics.h\"\n\n";
        file << "using namespace std;\n\nclass " << behavior_name << " : public Behaviour {\nprivate:\n    // Declare variables here\npublic:\n    " << behavior_name << "() {\n";
        file << "        // Don't delete this, the constructor is required\n    }\n\n    // /*\n    void update() {\n        // Code written here will be run every frame\n    }\n    // */\n\n    // /*\n";
        file << "    void draw() {\n        // Code written here will be run every frame after every update is run\n    }\n    // */\n    // Do not delete from here\n";
        file << "    shared_ptr<Behaviour> clone() const { return make_shared<" << behavior_name << ">(*this); }\n";
        file << "    string dataToString() const { return \"" << lower << ":\"; }\n";
        file << "}; \n\n#endif\n";

        file.close();

        {
            ifstream filein("observer_pattern.h");
            ofstream fileout("observer_pattern.tmp");

            string line;
            bool already_in = false;
            while (getline(filein, line)) {
                if (!already_in) {
                    if (line == "// #include \"" + behavior_name + ".h\"") already_in = true;
                    if (line == "#include <set>") fileout << "// #include \"" << behavior_name << ".h\"\n";
                    if (line == "            { \"New Behaviour\", bind(&Rastreator::ExpextBehaviourName, this) }")
                        fileout << "            { \"New " << behavior_name << "\", bind(&Rastreator::addBehaviour<" << behavior_name << ">, this, make_shared<" << behavior_name << ">()) },\n";
                    if (line == "class Subject {") {
                        fileout << "class Viewer" << behavior_name << " : public Viewer, public Observer {\nprivate:\n    shared_ptr<" << behavior_name << "> behavior;\n\n    Rectangle bounds;\n    bool options;\npublic:\n    Viewer" << behavior_name << "(shared_ptr<GameObject> go) {\n";
                        fileout << "    behavior = go->getBehaviour<" << behavior_name << ">();\n    options = false;\n}\n\n    void draw() {\n        GuiDrawRectangle(bounds, GuiGetStyle(STATUSBAR, BORDER_WIDTH), GetColor(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), GetColor(GuiGetStyle(STATUSBAR, BASE_COLOR_NORMAL)));\n";
                        fileout << "        GuiDrawText(\"" << behavior_name << "\", GetTextBounds(STATUSBAR, bounds), TEXT_ALIGN_LEFT, GetColor(GuiGetStyle(STATUSBAR, TEXT_COLOR_NORMAL)));\n\n        Rectangle optionsBounds = { bounds.x + bounds.width - bounds.height, bounds.y, bounds.height, bounds.height };\n";
                        fileout << "\n        bounds.y += bounds.height + 1;\n}\n\n    void update(Rectangle * bounds) {\n        this->bounds = *bounds;\n        draw();\n        *bounds = this->bounds;\n    }\n};\n\n";
                    }
                    if (line == "            options.push_back(false);")
                        fileout << "            else if (dynamic_pointer_cast<" << behavior_name << ">(item) != nullptr) add(make_shared<Viewer" << behavior_name << ">(_game_object));\n";
                }
                line += "\n";
                fileout << line;
            }
        }
        {
            ifstream filein("observer_pattern.tmp");
            ofstream fileout("observer_pattern.h");
            string line;

            while (getline(filein, line)) fileout << line << '\n';
            std::remove("observer_pattern.tmp");
        }
        {
            ifstream filein("RCCppMainLoop.cpp");
            ofstream fileout("RCCppMainLoop.tmp");

            string line;
            bool already_in = false;
            while (getline(filein, line)) {
                if (line == "// New Behaviours") fileout << "#include \"" << behavior_name << ".h\"\n";
                line += "\n";
                fileout << line;
            }
        }
        {
            ifstream filein("RCCppMainLoop.tmp");
            ofstream fileout("RCCppMainLoop.cpp");
            string line;

            while (getline(filein, line)) fileout << line << '\n';
            std::remove("RCCppMainLoop.tmp");
        }
        {
            ifstream filein("platgormerEngineBasics.h");
            ofstream fileout("platgormerEngineBasics.tmp");

            string line;
            bool already_in = false;
            while (getline(filein, line)) {
                if (line == "class GameObject : public Object, public enable_shared_from_this<GameObject>{") fileout << "class " << behavior_name << ";\n";
                line += "\n";
                fileout << line;
            }
        }
        {
            ifstream filein("platgormerEngineBasics.tmp");
            ofstream fileout("platgormerEngineBasics.h");
            string line;

            while (getline(filein, line)) fileout << line << '\n';
            std::remove("platgormerEngineBasics.tmp");
        }
    }
};
