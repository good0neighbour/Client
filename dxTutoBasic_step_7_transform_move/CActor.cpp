#include "CActor.h"

#include "CMesh.h"
#include "CMtlTexture.h"

#include "CDxEngine.h"
#include "CD3D.h"


void CActor::Create(CDxEngine* tpEngine)
{
    SetEngine(tpEngine);


    mpMtlTexture = new CMtlTexture();
    mpMtlTexture->Create(mpEngine->mpD3D->GetD3DDevice());
    mpMtlTexture->SetEngine(mpEngine);


    mpMesh = new CMesh();
    mpMesh->Create(mpEngine->mpD3D->GetD3DDevice(), L"resources/slimeby.dds");
    mpMesh->SetEngine(mpEngine);
}
void CActor::Destroy()
{

}

void CActor::Update()
{

}
void CActor::Render()
{

}