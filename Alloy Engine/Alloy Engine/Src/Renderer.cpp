#include "Renderer.h"
#include "Window.h"
#include "Macros.h"


Renderer::Renderer(Window& _window)
{
	m_render_data = std::make_unique<RenderData>();
	m_target_window = &_window;

	// Swap chain (double buffering)
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = CreateSwapChainDesc(_window);
	CreateDeviceContext(swap_chain_desc);
	CreateRenderTarget();
	CreateDepthBuffer();
	CreateDepthStencilState();
	CreateDepthStencilView();
	CreateRasterState();
	CreateAlphaBlendStates();
	//TurnOnAlphaBlending();
}


Renderer::~Renderer()
{
	SAFE_RELEASE(m_swap_chain);
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_device_context);
	SAFE_RELEASE(m_render_target_view);
	SAFE_RELEASE(m_depth_stencil_buffer);
	SAFE_RELEASE(m_depth_stencil_state);
	SAFE_RELEASE(m_depth_stencil_view);
	SAFE_RELEASE(m_raster_state);
	SAFE_RELEASE(m_alpha_disable_blending_state);
	SAFE_RELEASE(m_alpha_enable_blending_state);
}


void Renderer::BeginFrame() const
{
	// Bind render target
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

	// Set viewport
	auto viewport = CD3D11_VIEWPORT(0.f, 0.f, m_back_buffer_desc.Width,
		m_back_buffer_desc.Width);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_device_context->RSSetViewports(1, &viewport);

	// Set clear colour
	float clear_colour[] = { 0.2, 0.2, 0.180, 1 };
	m_device_context->ClearRenderTargetView(m_render_target_view, clear_colour);

	m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void Renderer::EndFrame() const
{
	// Swap buffer
	m_swap_chain->Present(1, 0);//Shows image (1 is vsync)
}


DXGI_SWAP_CHAIN_DESC Renderer::CreateSwapChainDesc(Window& _window) const
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };

	swap_chain_desc.BufferDesc.Width = _window.GetWindowWidth();
	swap_chain_desc.BufferDesc.Height = _window.GetWindowHeight();
	swap_chain_desc.BufferCount = 1;// Double buffer
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// Colour format (RGB 32 bit)
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = _window.GetHandle();// Set target
	swap_chain_desc.SampleDesc.Count = 8;// Antialiasing
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	return swap_chain_desc;
}


void Renderer::CreateDeviceContext(DXGI_SWAP_CHAIN_DESC& _swap_chain_desc)
{
	// Create the swap chain device and context
	auto result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &_swap_chain_desc,
		&m_swap_chain, &m_device, nullptr, &m_device_context);

	// Check errors
	if (result != S_OK)
	{
		MessageBox(nullptr, "[Renderer](CreateDeviceContext) Error creating DX11", "Error", MB_OK);
		exit(0);
	}
}


void Renderer::CreateRenderTarget()
{
	ID3D11Texture2D* back_buffer;
	m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));// Get buffer
	m_device->CreateRenderTargetView(back_buffer, nullptr, &m_render_target_view);// Set render target
	
	back_buffer->GetDesc(&m_back_buffer_desc);
	back_buffer->Release();// Decrement instance count
}


void Renderer::CreateDepthBuffer()
{
	HRESULT result{ 0 };

	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_target_window->GetWindowWidth();
	depthBufferDesc.Height = m_target_window->GetWindowHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 8;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depth_stencil_buffer);
	if (result != S_OK)
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create Depth buffer", "Error", MB_OK);
		exit(0);
	}
}


void Renderer::CreateDepthStencilState()
{
	HRESULT result{};
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depth_stencil_state);
	if (FAILED(result))
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create Depth stencil", "Error", MB_OK);
		exit(0);
	}

	// Set the depth stencil state.
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
}


void Renderer::CreateDepthStencilView()
{
	HRESULT result{ 0 };
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Flags = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depthStencilViewDesc, &m_depth_stencil_view);
	if (FAILED(result))
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create Depth stencil view", "Error", MB_OK);
		exit(0);
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
}


void Renderer::CreateRasterState()
{
	HRESULT result{};
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_raster_state);
	if (FAILED(result))
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create raster state", "Error", MB_OK);
		exit(0);
	}

	// Now set the rasterizer state.
	m_device_context->RSSetState(m_raster_state);

}


void Renderer::CreateAlphaBlendStates()
{
	HRESULT result {};
	D3D11_BLEND_DESC blendStateDescription;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = true;// Create an alpha enabled blend state description.

	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	
	result = m_device->CreateBlendState(&blendStateDescription, &m_alpha_enable_blending_state);// Create the blend state
	if (FAILED(result))
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create blend state", "Error", MB_OK);
		exit(0);
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = false;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alpha_disable_blending_state);
}


void Renderer::TurnOnAlphaBlending() const
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_device_context->OMSetBlendState(m_alpha_enable_blending_state, blendFactor, 0xffffffff);
}


void Renderer::TurnOffAlphaBlending() const
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_device_context->OMSetBlendState(m_alpha_disable_blending_state, blendFactor, 0xffffffff);
}


#pragma region Getters
ID3D11Device* Renderer::GetDevice() const
{
	return m_device;
}


ID3D11DeviceContext* Renderer::GetDeviceContext() const
{
	return m_device_context;
}


RenderData* Renderer::GetRenderData() const
{
	return m_render_data.get();
}


Window* Renderer::GetTargetWindow() const
{
	return m_target_window;
}


//returns width, height in a pair
std::pair<float, float> Renderer::GetViewportDimensions() const
{
	return std::pair<float, float>(m_back_buffer_desc.Width, m_back_buffer_desc.Height);
}


float Renderer::GetViewportAspectRatio() const
{
	return m_back_buffer_desc.Width / m_back_buffer_desc.Height;
}
#pragma endregion 