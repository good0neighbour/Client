#include "framework.h"

#include "CDxEngine.h"

/*
    dxTutoBegin_step_0

    이번 스텝에서는 다음 두 가지를 수행하겠다

    i) window 창을 띄운다
    ii) DirectX11을 이용하여 렌더링 시스템을 초기화하고 해제한다

    CAPIEngine.h    ---> CDxEngine.h
    CAPIEngine.cpp  ---> CDxEngine.cpp
    framework.h
    main.cpp
*/
class CRyuEngine : public CDxEngine
{ 
public:
    CRyuEngine() {};
    virtual ~CRyuEngine() {};

private:
    CRyuEngine(const CRyuEngine& t) = delete;
    CRyuEngine& operator=(const CRyuEngine& t) = delete;

public:
    virtual void OnCreate() override
    {
        CDxEngine::OnCreate();
    }
    virtual void OnDestroy() override
    {        
        CDxEngine::OnDestroy();
    }
    virtual void OnUpdate(float tDeltaTime) override
    {
        CDxEngine::OnUpdate(tDeltaTime);
        this->Clear(0.1f, 0.1f, 0.3f);
        this->Present();
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    
    CRyuEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    MSG msg = { 0 };
    msg = tEngine.Run();

    return (int)msg.wParam;
}



