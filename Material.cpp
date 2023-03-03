#include "Material.h"
#include "DX12Helper.h"

Material::Material(Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState, DirectX::XMFLOAT3 colorTint, DirectX::XMFLOAT2 uvOffset, DirectX::XMFLOAT2 uvScale)
{
	this->pipelineState = pipelineState;
	this->colorTint = colorTint;
	this->uvOffset = uvOffset;
	this->uvScale = uvScale;

	finalGPUHandleForSRVs = {};
	ZeroMemory(textureSRVsBySlot, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * 128);

	finalized = false;
	highestSlot = -1;
}

float Material::GetRoughness() { return roughness; }

DirectX::XMFLOAT3 Material::GetColorTint() { return colorTint; }

DirectX::XMFLOAT2 Material::GetUVOffset() { return uvOffset; }

DirectX::XMFLOAT2 Material::GetUVScale() { return uvScale; }

Microsoft::WRL::ComPtr<ID3D12PipelineState> Material::GetPipelineState() { return pipelineState; }

D3D12_GPU_DESCRIPTOR_HANDLE Material::GetFinalGPUHandleForSRVs() { return finalGPUHandleForSRVs; }

void Material::SetPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState) { this->pipelineState = pipelineState; }

void Material::SetColorTint(DirectX::XMFLOAT3 colorTint) { this->colorTint = colorTint; }

void Material::SetUVOffset(DirectX::XMFLOAT2 offset) { uvOffset = offset; }

void Material::SetUVScale(DirectX::XMFLOAT2 scale) { uvScale = scale; }

void Material::AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, int slot)
{
	if (finalized || slot >= 128 || slot < 0) return;

	textureSRVsBySlot[slot] = srvDescriptorHandle;

	highestSlot = max(highestSlot, slot);
}

void Material::FinalizeTextures()
{
	if (finalized) return;

	DX12Helper& dx12Helper = DX12Helper::GetInstance();

	for (int i = 0; i <= highestSlot; i++)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = 
			dx12Helper.CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);

		if (i == 0)
		{
			finalGPUHandleForSRVs = gpuHandle;
		}
	}

	finalized = true;
}
