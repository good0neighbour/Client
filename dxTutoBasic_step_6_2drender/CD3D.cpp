#include "CD3D.h"



HRESULT CD3D::Create(HWND thWnd)
{
    //������ �ڵ� ����
    mhWnd = thWnd;


    //directx�ʱ�ȭ 

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(this->mhWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,                           //�׷��� ī�� ��ġ(GPU+VRAM)���� ������ ó��
        D3D_DRIVER_TYPE_WARP,                                   //��� ������ ����Ʈ���� ������ ó��, HARDWARE ���� ������ ������ ó���� ���� �ʴ� ��� �ּ��� ������� ����Ʈ���������� ó�� <-- dx 9.1~ 10.1������ ����
        D3D_DRIVER_TYPE_REFERENCE,                          //CPU���� ������ ����Ʈ���������� ������ ó��, �ſ� ����, ���߿�
        //( �տ� �ܰ��� ����̹����� ������ ��� �����ص� �̰����� �����ϴ�. � ���� �۵� )
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //��ó����: '�׷��� ó�� ��ġ�� � ���̷�Ʈ ���� ������ �����ϴ����� ���� �����̴�. 
    //GPU�� �����ϴ� ��ɼ�Ʈ �� ����
    // �⺻������ ������ ��ó������ ������ ��ó������ �����Ѵ�.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        mDriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);

        if (hr == E_INVALIDARG)
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;


    //������� �����ϸ�, �ش� �׷��� ó�� ��ġ�� DirectX�� ��� ���ر��� �����ϴ��� �˾Ƴ� ���̴�. 
    //����̹� Ÿ�� ����, ��ó���� ����


    //DirectX�� Direct3D(d3d)�� device, device context �� '������'�� ����Ѵ�.


    //DXGI DirectX Graphics Infrastructure ���̷�Ʈ���� �׷��� �ý����� ��� ���� 

    /*
        DXGI�� �׷��� ���� ó�������� �غ��  (Direct3D�ʹ� )������ �������̴�.

        // ������۱����� ����ü�ε��� ������?�� �׷��� ���� ó�� ������ ����� ������ ����ϰ� �ִ�
        //���� ���� ���������� Direct3D�� ���յǾ� �ִ� ���������
        //      Direct3D���� �и��Ǿ� ���� ������ ����������� ���������
        //      ( <-- �ֳ��ϸ�, ���� �ش� ������ ����ȭ�Ǿ� �ִ� �Ϲ����� �����̱� �����̴� )

    */


    /*
        COM Compoent Object Model
            ���̳ʸ� ������ ȣȯ ������ ��ǰ


        // __uuidof ������ COM��ü�� �ĺ��ڸ� ��� �������̴�.
        //  ( ��, ���̳ʸ� �����̹Ƿ� Ŭ������ �̸� ���� �𸥴�. �׷��� �͵� ��� �ĺ��� �μ� ������ ���� ���̵�(�ĺ���)�� �ο��صδ� ���̴�.
        //      �� ���̵� ��´� ��� ���̴� )

        DirectX�� COM����� ������� ������� ���̴�.
    */

    /*

        mpd3dDevice ==>

        dxgiDevice ==> adapter ==> dxgiFactory             -->     mpSwapChain
        ==>dxgiFactory2 ==>                                          -->      mpSwapChain1


        ���� dxgiFactory2�� ������ٸ�              dx11.1�������� ����
        �׷��� �ʴٸ�                                         dx11�������� ����
    */


    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);          //adater DXGI�������� ��, �׷��� ó�� ��ġ�� �߻�ȭ�س��� ��ü(  ����ͱ��� ���� )
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();         //COM��ü�� ������ �� ����ϴ� �Լ� 
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    //dxgiFactory �� �����. 


    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
    if (dxgiFactory2)
    {
        // DirectX 11.1 or later
        hr = mpd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mpd3dDevice1));
        if (SUCCEEDED(hr))
        {
            (void)mpImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mpImmediateContext1));
        }
        //����ü�� �������� ����
        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(mpd3dDevice, this->mhWnd, &sd, nullptr, nullptr, &mpSwapChain1);
        if (SUCCEEDED(hr))
        {
            hr = mpSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mpSwapChain));
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        //����ü�� �������� ����
        DXGI_SWAP_CHAIN_DESC sd = {};           //description
        sd.BufferCount = 1;                     //back buffer������ 1��
        sd.BufferDesc.Width = width;                //������ �ʺ�
        sd.BufferDesc.Height = height;              //������ ���� 
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       //����Ÿ�� ��¿� ����
        sd.OutputWindow = this->mhWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation(this->mhWnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;


    //������� ����, SwapChain ���� �Ϸ�



    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    //������� ����, ���� Ÿ�� �� ���� �Ϸ�

    //OM Output Merger 
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);


    //�ؽ�ó ����
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;        //���� �ػ��� �ؽ�ó �̹����� ����� ���� mipmap�̶�� �Ѵ�
    descDepth.ArraySize = 1;        //
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//���̹���24, ���ٽ�8 -->32, unsigned normalize [0,1]
    descDepth.SampleDesc.Count = 1;    //���ø��� �ؽ�ó�� �ٷ궧 ����. ���ø�����.���ø��Լ�?�� 1��
    descDepth.SampleDesc.Quality = 0;    //���ø�����.����Ƽ
    descDepth.Usage = D3D11_USAGE_DEFAULT;  //�⺻ �뵵�� �����̴� 
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;     //����/���ٽǹ��ۿ��̴�.
    descDepth.CPUAccessFlags = 0;    //cpu������ �����Ѵ�
    descDepth.MiscFlags = 0;    //��Ÿ ���
    //�ؽ�ó ����
    //DirectX11���� ����/���ٽǹ��۴� �ؽ�ó ���ҽ��� �� ������ ����Ѵ�
    mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);

    //depth/stencil view ����
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;     //������ ������ descDepth�� ������
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;    //2���� ������ �����ͷ� ����
    descDSV.Texture2D.MipSlice = 0;            //�Ӹ� ������ �⺻

    //Depth/StencilView ����
    mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);


    //Depth/StencilView�� OM�� ����
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);




    //Ȱ��, ��Ȱ�� ���¿� ���� ����
    D3D11_DEPTH_STENCIL_DESC depthEnableDesc;
    D3D11_DEPTH_STENCIL_DESC depthDisableDesc;

    ZeroMemory(&depthEnableDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));//�޸𸮿� 0���� ����
    depthEnableDesc.DepthEnable = true;
    mpd3dDevice->CreateDepthStencilState(&depthEnableDesc, &mpDepthEnableState);

    ZeroMemory(&depthDisableDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));//�޸𸮿� 0���� ����
    depthDisableDesc.DepthEnable = false;
    mpd3dDevice->CreateDepthStencilState(&depthDisableDesc, &mpDepthDisableState);









    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mpImmediateContext->RSSetViewports(1, &vp);
    //RS~ RenderState

    return S_OK;
}
void CD3D::Destroy()
{
    if (mpImmediateContext) mpImmediateContext->ClearState();

    if (mpRenderTargetView) mpRenderTargetView->Release();
    if (mpSwapChain1) mpSwapChain1->Release();
    if (mpSwapChain) mpSwapChain->Release();
    if (mpImmediateContext1) mpImmediateContext1->Release();
    if (mpImmediateContext) mpImmediateContext->Release();
    if (mpd3dDevice1) mpd3dDevice1->Release();
    if (mpd3dDevice) mpd3dDevice->Release();
}

void CD3D::Clear(float tR, float tG, float tB)
{
    //XM~ DirectX Math
    XMVECTORF32 tColor;
    tColor.f[0] = tR;
    tColor.f[1] = tG;
    tColor.f[2] = tB;


    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //���̹��۴� ó���� 1�� �����ϰڴ�
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3D::Clear(XMVECTORF32 tColor)
{
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    //���̹��۴� ó���� 1�� �����ϰڴ�
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3D::Present()
{
    mpSwapChain->Present(0, 0);
}



void CD3D::DoTurnOnDepthBuffer()
{
    mpImmediateContext->OMSetDepthStencilState(mpDepthEnableState, 1);
}
void CD3D::DoTurnOffDepthBuffer()
{
    mpImmediateContext->OMSetDepthStencilState(mpDepthDisableState, 1);

}