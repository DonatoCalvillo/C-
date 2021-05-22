

//#define _XM_NO_INTRINSICS_

#include <d3d11.h>
#include <d3dx11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>
#include <d3dx10math.h>
#include <conio.h>
#include <vector>
#include <iostream>

using namespace std;

struct vector3 {
	float x, y, z;
};

class Modelo {
private:
	struct VertexComponent
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 UV;		
		D3DXVECTOR3 normal;		
	};

	struct VertexCollide
	{
		D3DXVECTOR3 pos;
	};	

	struct vector2 {
		float u, v;
	};

	ID3D11VertexShader* VertexShaderVS;
	ID3D11PixelShader* solidColorPS;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11ShaderResourceView* colorMap;
	ID3D11SamplerState* colorMapSampler;

	ID3D11Buffer* viewCB;
	ID3D11Buffer* projCB;
	ID3D11Buffer* worldCB;
	ID3D11Buffer* FAA;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projMatrix;

	vector3 rotacion;
	vector3 traslacion;
	vector3 escalacion;

	int ancho, alto;
	int anchoTexTerr, altoTexTerr;
	float anchof, altof;
	float deltax, deltay;

	UINT* indices;
	int cantind;
	VertexComponent* vertices;
	int cantvert;
	VertexCollide* vertcol;	
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;

public:

	

	Modelo(ID3D11Device* D3DDevice, ID3D11DeviceContext* D3DContext, WCHAR textura[], const char* obj, vector3 rot, vector3 esc, vector3 tras);

	~Modelo();

	bool CompileD3DShader(WCHAR* filePath, char* entry, char* shaderModel, ID3DBlob** buffer);

	bool CargaParametros(WCHAR* diffuseTex, const char* obj);

	void UnloadContent();

	void Draw(D3DXMATRIX vista, D3DXMATRIX proyeccion, vector3 rot, vector3 trans, vector3 esc , float faa);

	void CargaModelos(const char* obj);

	vector3 getRotacion() { return this->rotacion; }

	vector3 getTraslacion() { return this->traslacion; }

	vector3 getescalacion() { return this->escalacion; }

};
