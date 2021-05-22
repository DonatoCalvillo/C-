
#ifndef _water
#define _water

#include <string>
#include <d3d11.h>
#include <d3dx11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>
#include <d3dx10math.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Camara.h"


class Water
{
private:
	typedef float float2[2];
	typedef float float3[3];

	struct Vert {
		float3 pos;
		float3 nrm;
		float2 uv;
	};

	struct Parametros
	{
		D3DXMATRIX m_World;
		D3DXMATRIX m_View;
		D3DXMATRIX m_Projection;

		D3DXVECTOR4 v_camPos;

		D3DXVECTOR4 v_lightPos;

		D3DXVECTOR3 c_light;
		float f_shininess;

		float f_height_value;
		float f_map_displace;
		float f_map_displacement_factor;
	}params;

	float width, depth;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;

	ID3D11InputLayout* inputLayout;

	ID3D11ShaderResourceView* t_color;
	ID3D11ShaderResourceView* t_normal;
	ID3D11ShaderResourceView* t_metalness;
	ID3D11ShaderResourceView* t_height;
	ID3D11ShaderResourceView* t_displacement;


	ID3D11SamplerState* s_sampler;

	ID3D11Buffer* cb_parameters;

	ID3D11Device* d_device;
	ID3D11DeviceContext* d_device_context;

	ID3D11Buffer* b_vertex;
	ID3D11Buffer* b_index;

	Vert* pVert;
	unsigned int* pInd;

	unsigned int			    u_v_count = 0;
	unsigned int			    u_i_count = 0;

public:

	struct vector3 {
		float x, y, z;
	};

	Water(std::wstring s_color,
		std::wstring s_normal,
		std::wstring s_metalness,
		std::wstring s_height,
		std::wstring s_displacement,
		float width, float depth,
		int xDivisions, int zDivisions,
		int uTiles, int vTiles, ID3D11Device* d_device, ID3D11DeviceContext* d_device_context)
	{
		this->width = width;
		this->depth = depth;

		this->d_device = d_device;
		this->d_device_context = d_device_context;

		pVert = LoadVertices(width, depth, xDivisions, zDivisions, uTiles, vTiles, u_v_count);

		pInd = LoadIndices(xDivisions, zDivisions);

		CargaParametros(s_color, s_normal, s_metalness, s_height, s_displacement);
	}

	void Draw(D3DXMATRIX M, Camara* cam, D3DXVECTOR4 lightPos)
	{
		static float rotation = 0.0f;
		rotation += 0.0001;

		D3DXMATRIX wTr, vTr, pTr, yRot;
		D3DXMatrixTranspose(&wTr, &M);

		D3DXVECTOR4 finalPos;
		D3DXMatrixRotationY(&yRot, rotation);
		D3DXVec4Transform(&finalPos, &lightPos, &yRot);


		unsigned int stride = sizeof(Vert);
		unsigned int offset = 0;

		d_device_context->IASetInputLayout(inputLayout);

		d_device_context->IASetVertexBuffers(0, 1, &b_vertex, &stride, &offset);
		d_device_context->IASetIndexBuffer(b_index, DXGI_FORMAT_R32_UINT, 0);
		d_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d_device_context->VSSetShader(VS, 0, 0);
		d_device_context->PSSetShader(PS, 0, 0);

		d_device_context->VSSetShaderResources(3, 1, &t_height);
		d_device_context->PSSetShaderResources(0, 1, &t_color);
		d_device_context->PSSetShaderResources(1, 1, &t_normal);
		d_device_context->PSSetShaderResources(2, 1, &t_metalness);
		d_device_context->PSSetShaderResources(3, 1, &t_height);
		d_device_context->PSSetShaderResources(4, 1, &t_displacement);
		d_device_context->VSSetSamplers(0, 1, &s_sampler);
		d_device_context->PSSetSamplers(0, 1, &s_sampler);

		params.m_World = M;
		params.m_View = cam->vista;
		params.m_Projection = cam->proyeccion;
		params.v_camPos = D3DXVECTOR4(cam->posCam, 1);
		params.v_lightPos = lightPos;
		params.c_light = D3DXVECTOR3(1, 1, 1);
		params.f_shininess = 0.1f;
		params.f_height_value = 6.0f;
		params.f_map_displace = rotation;
		params.f_map_displacement_factor = 0.02;

		d_device_context->UpdateSubresource(cb_parameters, 0, 0, &params, 0, 0);

		d_device_context->VSSetConstantBuffers(0, 1, &cb_parameters);

		d_device_context->DrawIndexed(u_i_count, 0, 0);
	}

private:

	Vert* LoadVertices(float width, float depth,
		int xDivisions, int zDivisions,
		int uTiles, int vTiles,
		unsigned int& cantidad)
	{
		float xStep = (float)width / (float)(xDivisions - 1);
		float zStep = (float)depth / (float)(zDivisions - 1);

		float uStep = (float)uTiles / (float)(xDivisions - 1);
		float vStep = (float)vTiles / (float)(zDivisions - 1);
		cantidad = xDivisions * zDivisions;

		Vert* v_arr = new Vert[cantidad];

		for (int i = 0; i < xDivisions; ++i)
		{
			for (int j = 0; j < zDivisions; ++j)
			{
				Vert v;
				v.pos[0] = xStep * j - width / 2.0f;
				v.pos[1] = 10;
				v.pos[2] = zStep * i - depth / 2.0f;

				v.uv[0] = uStep * j;
				v.uv[1] = uStep * i;

				v.nrm[0] = 0;
				v.nrm[1] = 1;
				v.nrm[2] = 0;

				v_arr[i * xDivisions + j] = v;
			}
		}
		return v_arr;
	}

	unsigned int* LoadIndices(unsigned int ancho, unsigned int alto)
	{
		u_i_count = (ancho - 1) * (alto - 1) * 6;
		unsigned int* indices = new unsigned int[u_i_count];
		unsigned int counter = 0;
		for (int i = 0; i < alto - 1; ++i)
		{
			for (int j = 0; j < ancho - 1; ++j)
			{
				int lowerLeft = i * ancho + j;
				int lowerRight = i * ancho + (j + 1);
				int topLeft = (i + 1) * ancho + j;
				int topRight = (i + 1) * ancho + (j + 1);

				indices[counter++] = lowerLeft;
				indices[counter++] = topLeft;
				indices[counter++] = lowerRight;

				indices[counter++] = lowerRight;
				indices[counter++] = topLeft;
				indices[counter++] = topRight;
			}
		}
		return indices;
	}

	HRESULT CompileD3DShader(WCHAR* filePath, char* entry, char* shaderModel, ID3DBlob** buffer)
	{
		//forma de compilar el shader
		DWORD shaderFlags = D3DCOMPILE_DEBUG;

		ID3DBlob* errorBuffer = 0;
		HRESULT result;

		result = D3DX11CompileFromFile(filePath, 0, 0, entry, shaderModel, shaderFlags,
			0, 0, buffer, &errorBuffer, 0);
		if (FAILED(result))
		{
			if (errorBuffer != 0)
			{
				OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
				std::ofstream ofstr = std::ofstream("Water Body Log.txt", std::ios::out | std::ios::trunc);

				ofstr << (char*)errorBuffer->GetBufferPointer();

				ofstr.close();

				errorBuffer->Release();
			}
			return result;
		}

		if (errorBuffer != 0)
			errorBuffer->Release();

		return result;
	}

	bool CargaParametros(
		std::wstring s_color,
		std::wstring s_normal,
		std::wstring s_metalness,
		std::wstring s_height,
		std::wstring s_displacement)
	{
		HRESULT d3dResult;

		ID3DBlob* vsBuffer = 0;

		d3dResult = CompileD3DShader(L"Water.fx", "vert", "vs_5_0", &vsBuffer);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		d3dResult = d_device->CreateVertexShader(vsBuffer->GetBufferPointer(),
			vsBuffer->GetBufferSize(), 0, &VS);

		if (FAILED(d3dResult))
		{

			if (vsBuffer)
				vsBuffer->Release();

			return d3dResult;
		}

		D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

		d3dResult = d_device->CreateInputLayout(solidColorLayout, totalLayoutElements,
			vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout);

		vsBuffer->Release();

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		ID3DBlob* psBuffer = 0;

		d3dResult = CompileD3DShader(L"Water.fx", "pixel", "ps_4_0", &psBuffer);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		d3dResult = d_device->CreatePixelShader(psBuffer->GetBufferPointer(),
			psBuffer->GetBufferSize(), 0, &PS);

		psBuffer->Release();

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}



		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(Vert) * u_v_count;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = pVert;

		d3dResult = d_device->CreateBuffer(&vertexDesc, &resourceData, &b_vertex);

		if (FAILED(d3dResult))
		{
			MessageBox(0, L"Error", L"Error al crear vertex buffer", MB_OK);
			return d3dResult;
		}

		D3D11_BUFFER_DESC indexDesc;
		ZeroMemory(&indexDesc, sizeof(indexDesc));
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.ByteWidth = sizeof(unsigned int) * u_i_count;
		indexDesc.CPUAccessFlags = 0;

		resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = pInd;

		d3dResult = d_device->CreateBuffer(&indexDesc, &resourceData, &b_index);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}


		d3dResult = D3DX11CreateShaderResourceViewFromFile(d_device, s_color.c_str(), 0, 0, &t_color, 0);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		d3dResult = D3DX11CreateShaderResourceViewFromFile(d_device, s_normal.c_str(), 0, 0, &t_normal, 0);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		d3dResult = D3DX11CreateShaderResourceViewFromFile(d_device, s_metalness.c_str(), 0, 0, &t_metalness, 0);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		d3dResult = D3DX11CreateShaderResourceViewFromFile(d_device, s_height.c_str(), 0, 0, &t_height, 0);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		d3dResult = D3DX11CreateShaderResourceViewFromFile(d_device, s_displacement.c_str(), 0, 0, &t_displacement, 0);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}



		D3D11_SAMPLER_DESC colorMapDesc;
		ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		d3dResult = d_device->CreateSamplerState(&colorMapDesc, &s_sampler);

		if (FAILED(d3dResult))
		{
			return d3dResult;
		}

		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = 256; //sizeof(ParametersCB);
		constDesc.StructureByteStride = sizeof(Parametros);
		constDesc.Usage = D3D11_USAGE_DEFAULT;

		d3dResult = d_device->CreateBuffer(&constDesc, 0, &cb_parameters);

		if (FAILED(d3dResult))
		{
			return false;
		}

		return true;
	}
};

#endif
