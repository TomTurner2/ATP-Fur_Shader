#pragma once
#include <d3d11.h>
#include "RenderData.h"
#include <memory>

class Window;

class Renderer
{
public:
	explicit Renderer(Window& _window);
	~Renderer();

	void BeginFrame() const;
	void EndFrame() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	RenderData* GetRenderData() const;
	Window* GetTargetWindow() const;
	std::pair<float, float> GetViewportDimensions() const;
	float GetViewportAspectRatio() const;

	void TurnOnAlphaBlending() const;
	void TurnOffAlphaBlending() const;

private:
	IDXGISwapChain* m_swap_chain { nullptr };
	ID3D11Device* m_device { nullptr };
	ID3D11DeviceContext* m_device_context { nullptr };
	ID3D11RenderTargetView* m_render_target_view { nullptr };// Render texture for screen
	D3D11_TEXTURE2D_DESC m_back_buffer_desc;
	UINT m_quality_level = 8;

	std::unique_ptr<RenderData> m_render_data { nullptr };
	Window* m_target_window { nullptr };


	ID3D11Texture2D* m_depthStencilBuffer { nullptr };
	ID3D11DepthStencilState* m_depthStencilState { nullptr };
	ID3D11DepthStencilView* m_depthStencilView { nullptr };
	ID3D11RasterizerState* m_rasterState { nullptr };
	ID3D11BlendState* m_alphaEnableBlendingState { nullptr };
	ID3D11BlendState* m_alphaDisableBlendingState;


	DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc(Window& _window) const;
	void CreateDeviceContext(DXGI_SWAP_CHAIN_DESC& _swap_chain);
	void CreateRenderTarget();
	void CreateDepthBuffer();
	void CreateDepthStencilState();
	void CreateDepthStencilView();
	void CreateRasterState();
	void CreateAlphaBlendStates();
};