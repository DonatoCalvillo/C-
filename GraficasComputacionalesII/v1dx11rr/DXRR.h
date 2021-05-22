#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "Camara.h"
#include "ModeloRR.h"
#include "Modelo.h"
#include "SkyDome.h"
#include "TerrenoRR.h"
#include "Billboard.h"
#include "Water.h"

#include "XACT3Util.h"


class DXRR {

private:
	int ancho;
	int alto;
public:
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState* alphaBlendState, * commonBlendState;
	D3DXVECTOR4 timer = D3DXVECTOR4(0, 0, 0, 0);

	Camara* camara;
	ModeloRR* moose;
	ModeloRR* elk;
	ModeloRR* deer;
	ModeloRR* cabin;
	ModeloRR* character; 
	ModeloRR* deer2;
	ModeloRR* swan;
	ModeloRR* snowman;
	ModeloRR* fish;
	ModeloRR* fish2;
	ModeloRR* hawk;
	ModeloRR* ibex;
	ModeloRR* penguin;
	ModeloRR* penguin2;
	ModeloRR* seal;
	ModeloRR* rocks;
	ModeloRR* stones;
	ModeloRR* sheep;
	ModeloRR* stork;
	ModeloRR* tahr;
	SkyDome* skydome;
	TerrenoRR* terreno;
	BillboardRR* flower;
	BillboardRR* plant;
	BillboardRR* mountain;
	BillboardRR* mountain2;
	BillboardRR* mountain3;
	BillboardRR* mountain4;
	BillboardRR* bush;
	BillboardRR* bush2;
	Water* water;


	CXACT3Util m_XACT3; //music

	float izqder;
	float arriaba;
	float vel;
	float lerp = 0.5f;
	double distancia = 0.0;
	float xPos;
	float yPos;

	float FAA;


	DXRR(HWND hWnd, int Ancho, int Alto)
	{
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		camara = new Camara(D3DXVECTOR3(10, 80, 10), D3DXVECTOR3(0, 80, 0), D3DXVECTOR3(0, 1, 0), Ancho, Alto);

		skydome = new SkyDome(35, 35, 80.0f, &d3dDevice, &d3dContext, L"assets\\sky\\SkyDome3.png");

		terreno = new TerrenoRR(300, 300, d3dDevice, d3dContext, L"assets\\Terrain\\mountain.jpg", L"assets\\Terrain\\Moss0.png",
			L"assets\\Terrain\\Moss0Nm.png", L"assets\\Terrain\\Moss.png", L"assets\\Terrain\\MossNm.png",
			L"assets\\Terrain\\Snow1.jpg", L"assets\\Terrain\\Snow1Nm.jpg", L"assets\\Terrain\\blend0.jpg");


#pragma region Models
		moose = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Moose/moose.obj", L"assets/Models/Moose/moose.jpg", L"assets/Models/Moose/moose_nm.jpg", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		elk = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Elk/elk.obj", L"assets/Models/Elk/elk.jpg", L"assets/Models/Elk/elk_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		deer = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Deer/deer.obj", L"assets/Models/Deer/deer.jpg", L"assets/Models/Deer/deer_nm.jpg", L"assets/Models/Deer/deer_spc.png", L"assets/Models/Moose/spc.jpg");

		character = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Character/owen.obj", L"assets/Models/Character/owen.png", L"assets/Models/Character/owen_nm.png", L"assets/Models/Character/owen_spc.png", L"assets/Models/Moose/spc.jpg");
		character->setRotation(-5.5f);

		deer2 = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Deer2.0/deer0.obj", L"assets/Models/Deer2.0/deer.jpg", L"assets/Models/Moose/dis.jpg", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		swan = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Swan/swan0.obj", L"assets/Models/Swan/swan.jpg", L"assets/Models/Moose/dis.jpg", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");

		snowman = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Snowman/SnowmanOBJ.obj", L"assets/Models/Snowman/snowman.jpg", L"assets/Models/Snowman/snowman_nm.jpg", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		fish = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Fish/fish.obj", L"assets/Models/Fish/fish.png", L"assets/Models/Fish/fish_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		fish2 = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Fish2/fish.obj", L"assets/Models/Fish2/fish.png", L"assets/Models/Moose/dis.jpg", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		hawk = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Hawk/hawk.obj", L"assets/Models/Hawk/hawk.jpg", L"assets/Models/Hawk/hawk_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		ibex = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Ibex/ibex.obj", L"assets/Models/Ibex/ibex.jpg", L"assets/Models/Ibex/ibex_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		penguin = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Penguin/Pinguino.obj", L"assets/Models/Penguin/Pinguino.jpg", L"assets/Models/Penguin/Pinguino_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		penguin2 = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Penguin2/penguin.obj", L"assets/Models/Penguin2/penguin.png", L"assets/Models/Penguin2/penguin_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		rocks = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Rock/rock.obj", L"assets/Models/Rock/rock1.png", L"assets/Models/Rock/rock1_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		stones = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Rock/stone.obj", L"assets/Models/Rock/rock3.png", L"assets/Models/Rock/rock3_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		seal = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Seal/seal.obj", L"assets/Models/Seal/seal.jpg", L"assets/Models/Seal/seal_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		sheep = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Sheep/sheep.obj", L"assets/Models/Sheep/sheep.jpg", L"assets/Models/Sheep/sheep_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		stork = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Stork/stork.obj", L"assets/Models/Stork/stork.jpg", L"assets/Models/Moose/dis.jpg", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
		tahr = new ModeloRR(d3dDevice, d3dContext, "assets/Models/Tahr/tahr.obj", L"assets/Models/Tahr/tahr.jpg", L"assets/Models/Tahr/tahr_nm.png", L"assets/Models/Moose/spc.jpg", L"assets/Models/Moose/spc.jpg");
#pragma endregion

#pragma region Billboards

		flower = new BillboardRR(L"assets\\Billboards\\flor.png", 105, 110, d3dDevice, d3dContext, 2);
		plant = new BillboardRR(L"assets\\Billboards\\planta.png", 105, 110, d3dDevice, d3dContext, 2);
		mountain = new BillboardRR(L"assets\\Billboards\\mountain.png", 0, 145, d3dDevice, d3dContext, 150);
		mountain2 = new BillboardRR(L"assets\\Billboards\\mountain.png", 0, -145, d3dDevice, d3dContext, 150);
		mountain3 = new BillboardRR(L"assets\\Billboards\\mountain2.png", 145, 0, d3dDevice, d3dContext, 150);
		mountain4 = new BillboardRR(L"assets\\Billboards\\mountain2.png", -145, 0, d3dDevice, d3dContext, 150);
		bush = new BillboardRR(L"assets\\Billboards\\bush0.png", 90, 110, d3dDevice, d3dContext, 2);
		bush2 = new BillboardRR(L"assets\\Billboards\\bush.png", 110, 50, d3dDevice, d3dContext, 2);

#pragma endregion


		water = new Water(L"assets\\Water\\water2.png", L"assets\\Water\\waternormal.jpg", L"assets\\Water\\waterspec.jpg", L"assets\\Water\\waterhei.jpg", L"assets\\Water\\waterdis.jpg", 1000, 1000, 60, 60, 40, 2, d3dDevice, d3dContext);
		

	}

	~DXRR()
	{
		LiberaD3D();
	}

	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for (driver = 0; driver < totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels,
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if (SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if (FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}

		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& backBufferTexture);
		if (FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if (backBufferTexture)
			backBufferTexture->Release();

		if (FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);



		// Initialize XACT3
		result = m_XACT3.Initialize();
		if (!result) return false;
		result = m_XACT3.LoadWaveBank(L"Win\\WaveBank.xwb");
		if (!result) return false;
		result = m_XACT3.LoadSoundBank(L"Win\\SoundBank.xsb");
		if (!result) return false;

		// Play XACT3 cue
		XACTINDEX cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("Majula");
		m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);

		return true;

	}


	void LiberaD3D(void)
	{
		if (depthTexture)
			depthTexture->Release();
		if (depthStencilView)
			depthStencilView->Release();
		if (backBufferTarget)
			backBufferTarget->Release();
		if (swapChain)
			swapChain->Release();
		if (d3dContext)
			d3dContext->Release();
		if (d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}

	void Render(void)
	{
		if (d3dContext == 0)
			return;

		if (d3dContext == 0)
			return;

		timer.x += 0.005;

		if (timer.x >= 6) {
			timer.x = 0;
		}

		//Cambio de FAA
		if (timer.x >= 5) // MEDIO DIA
		{
			FAA = 0.6;
		}
		else if (timer.x >= 4) // AMANECER
		{
			FAA = 0.4;
		}
		else if (timer.x >= 3) //NOCHE
		{
			FAA = 0.2;
		}
		else if (timer.x >= 2)
		{
			FAA = 0.4;
		}
		else if (timer.x >= 1)
		{
			FAA = 0.6;
		}
		else
		{
			FAA = 0.8;
		}

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
		d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		D3DXMATRIX scalingMat;
		D3DXMatrixScaling(&scalingMat, .6f, .6f, .6f);
		D3DXVECTOR4 luz(0, 5, 0, .5);
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5;
		camara->UpdateCam(vel, arriaba, izqder);
		skydome->Update(camara->vista, camara->proyeccion);

		TurnOffDepth();
		skydome->Render(camara->posCam, timer);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion, FAA);


		//ALCE
		moose->Draw(camara->vista, camara->proyeccion, 0.0f, 22.2f, 6.0f, 0.0f, 0.07f, FAA);

		//RENO
		elk->Draw(camara->vista, camara->proyeccion, 10.0f, 27.0f, 3.0f, 0.0f, 2.0f, FAA);

		//VENADO
		deer->Draw(camara->vista, camara->proyeccion, 10.0f, 23.0f, 12.0f, 0.0f, 2.0f, FAA);

		//PERSONAJE
		static vector3 owent = { camara->posCam.x,terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5, camara->posCam.z };
		static vector3 owenr = { 0.f, 0.f, 0.f };
		static vector3 owens = { 0.03f, 0.03f, 0.03f };

		owent = { camara->posCam.x,terreno->Superficie(camara->posCam.x, camara->posCam.z) , camara->posCam.z };
		owenr = { this->izqder , 0.f, 0.f };

		character->Draw(camara->vista, camara->proyeccion, owent.x, owent.y, owent.z, owenr.x, 0.04,FAA);

		//VENDADO 2.0
		deer2->Draw(camara->vista, camara->proyeccion, 50.0f, 17.0f, 12.0f, 0.0f, 0.1f,FAA);

		//CISNE
		swan->Draw(camara->vista, camara->proyeccion, 100.0f, 6.0f, -70.0f, 0.0f, 0.07f,FAA);

		//SNOWMAN
		//snowman->Draw(camara->vista, camara->proyeccion, -100.0f, 5.0f, -70.0f, 0.0f, 0.05f, FAA);
		
		//PESCADO
		fish->Draw(camara->vista, camara->proyeccion, 110.0f, 1.0f, -90.0f, 0.0f, 0.5f, FAA);

		//PESCADO 2.0
		fish2->Draw(camara->vista, camara->proyeccion, 120.0f, 1.0f, -100.0f, 0.0f, 0.5f, FAA);

		//HALCON
		//hawk->Draw(camara->vista, camara->proyeccion, -10.0f, 25.0f, 10.0f, 0.0f, 0.05f, FAA);

		//CABRA
		ibex->Draw(camara->vista, camara->proyeccion, 22.0f, 24.0f, 10.0f, 0.0f, 0.07f, FAA);

		//PINGUINO
		penguin->Draw(camara->vista, camara->proyeccion, 130.0f, 4.0f, -50.0f, 0.0f, 0.05f, FAA);

		//PINGUINO 2.0
		penguin2->Draw(camara->vista, camara->proyeccion, 120.0f, 5.0f, -50.0f, 0.0f, 4.0f, FAA);

		//ROCAS
		rocks->Draw(camara->vista, camara->proyeccion, 120.0f, 0.0f, 100.0f, 0.0f, 0.3f, FAA);

		//PIEDRAS
		stones->Draw(camara->vista, camara->proyeccion, 120.0f, 1.0f, -110.0f, 0.0f, 3.0f, FAA);

		//FOCA
		seal->Draw(camara->vista, camara->proyeccion, 113.0f, 4.0f, -125.0f, 0.0f, 0.1f, FAA);

		//CABRA
		sheep->Draw(camara->vista, camara->proyeccion, -70.0f, 6.0f, 125.0f, 0.0f, 0.1f, FAA);

		//STORK
		stork->Draw(camara->vista, camara->proyeccion, 90.0f, 2.0f, -120.0f, 0.0f, 0.1f, FAA);

		//TAHR
		tahr->Draw(camara->vista, camara->proyeccion, -70.0f, 6.0f, 100.0f, 0.0f, 0.1f, FAA);


		//FLOR
		flower->Draw(camara->vista, camara->proyeccion, camara->posCam, terreno->Superficie(flower->posx, flower->posz), FAA);

		//PLANTA
		plant->Draw(camara->vista, camara->proyeccion, camara->posCam, terreno->Superficie(plant->posx, plant->posz),FAA);

		//MOUNTAIN #1
		mountain->Draw(camara->vista, camara->proyeccion, camara->posCam - camara->posCam, terreno->Superficie(flower->posx, flower->posz) - 2, FAA);

		//MOUNTAIN #2
		mountain2->Draw(camara->vista, camara->proyeccion, camara->posCam - camara->posCam, terreno->Superficie(flower->posx, flower->posz)-2, FAA);

		//MOUNTAIN #3
		mountain3->Draw(camara->vista, camara->proyeccion, camara->posCam - camara->posCam, terreno->Superficie(flower->posx, flower->posz) - 2, FAA);

		//MOUNTAIN #4
		mountain4->Draw(camara->vista, camara->proyeccion, camara->posCam - camara->posCam, terreno->Superficie(flower->posx, flower->posz) - 2, FAA);
		
		//PLANTA
		bush->Draw(camara->vista, camara->proyeccion, camara->posCam, terreno->Superficie(bush->posx, bush->posz)-.5, FAA);
		//PLANTA
		bush2->Draw(camara->vista, camara->proyeccion, camara->posCam, terreno->Superficie(bush2->posx, bush2->posz)-.5, FAA);

		TurnOnAlphaBlending();
		water->Draw(scalingMat, camara, luz);
		TurnOffAlphaBlending();


		swapChain->Present(1, 0);
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable = true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);

		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable = true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}
};
#endif