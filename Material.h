#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory>

class Material
{
public:
	Material(
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState,
		DirectX::XMFLOAT3 colorTint,
		DirectX::XMFLOAT2 uvOffset,
		DirectX::XMFLOAT2 uvScale,
		float roughness
	);

	// Getter Functions
	float GetRoughness();
	DirectX::XMFLOAT3 GetColorTint();
	DirectX::XMFLOAT2 GetUVOffset();
	DirectX::XMFLOAT2 GetUVScale();
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();

	// Setter Functions
	void SetPipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);
	void SetColorTint(DirectX::XMFLOAT3 colorTint);
	void SetUVOffset(DirectX::XMFLOAT2 offset);
	void SetUVScale(DirectX::XMFLOAT2 scale);

	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, int slot);
	void FinalizeTextures();

private:
	float roughness;
	DirectX::XMFLOAT3 colorTint;
	DirectX::XMFLOAT2 uvOffset;
	DirectX::XMFLOAT2 uvScale;

	bool finalized;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[128]; //Note: if I can't get it to work, switch to 4
	int highestSlot;

	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;
};

