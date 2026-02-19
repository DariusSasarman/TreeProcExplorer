#include "graphics/raylib_files/raylib.h"
#include "process_monitor/list_processes.h"
#include "graphics/visual_representation.h"
#include "translation_between_forms/translator.h"

tree * getProcessTree()
{
    process * processes = getProcesses();
    tree * ret =  translate(processes);
    free(processes);
    return ret;
}

int main() { 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TreeProcExplorer");
    SetTargetFPS(60); // Smoother for camera movement

    // Initialize Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ WINDOW_WIDTH/2, WINDOW_HEIGHT/2 };
    camera.offset = (Vector2){ WINDOW_WIDTH/2, WINDOW_HEIGHT/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    tree *forest = getProcessTree();
    setCoordinates(forest); // Initial layout

    float updateTimer = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        updateTimer += deltaTime;

        //1. Coordinate translation for opening clicks
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Convert screen space (mouse) to world space (camera)
            Vector2 worldClick = GetScreenToWorld2D(GetMousePosition(), camera);
            handleClick(worldClick.x, worldClick.y, forest->virtual_root);
        }

        //2. Camera controls : dragging
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            camera.target.x += delta.x *(-1.0f / camera.zoom);
            camera.target.y += delta.y *(-1.0f / camera.zoom);
        }
        /// Camera controls : zoom
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;
            camera.zoom += wheel * 0.125f;
            if (camera.zoom < 0.1f) camera.zoom = 0.1f;
        }

        // background update
        if (updateTimer >= 2.0f) {
            ///Every two seconds
            tree *newState = getProcessTree();
            ///Check if the process count has changed
            if(isDifferent(newState->virtual_root, forest->virtual_root)) {
                /// if so, replace the old tree
                setCoordinates(newState); // Layout the new tree
                tree *aux = forest;
                forest = newState;
                /// And delete it
                deleteTree(aux);
            } else {
                /// Else, just forget about it
                deleteTree(newState); // No changes, discard
            }
            updateTimer = 0.0f;
        }

        // Raylib drawing
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode2D(camera);

                represent(forest);
    
            EndMode2D();

            // UI Overlay (static text)
            DrawText("Left click to open the process' folder | Right Click to Drag the camera | Scroll to Zoom", 10, 10, 20, DARKGRAY);
            DrawFPS(WINDOW_WIDTH - 80, 10);
        EndDrawing();
    }

    deleteTree(forest);
    CloseWindow();
    return 0;
}
