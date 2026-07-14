#include<Dxlib.h>

#include"SceneManager.h"
#include"Input.h"






int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow){
    ChangeWindowMode(TRUE);
    SetDoubleStartValidFlag(TRUE);
    if (DxLib_Init() == -1)
    {
        MessageBox(NULL, "DxLib Init Error", "Error", MB_OK);
        return -1;
    }

   // MessageBox(NULL, "After Init", "TEST", MB_OK);

    Scene scene;

    while (ProcessMessage() == 0)
    {
        UpdateInput();
        UpdateKey();

        scene.Update();

        ClearDrawScreen();
        scene.Draw();
        ScreenFlip();
    }

    DxLib_End();

    return 0;
}