#include "Renderer.h"
#include "Window.h"
#include "Macros.h"


Renderer::Renderer(Window& _window, bool _start_fullscreen = true)
{
	m_render_data = std::make_unique<RenderData>();
	m_target_window = &_window;

	auto swap_chain_desc = CreateSwapChainDesc(_window);// Swap chain (double buffering).
	CreateDeviceContext(swap_chain_desc);
	CreateRenderTarget();
	CreateDepthBuffer();
	CreateDepthStencilState();
	CreateDepthStencilView();
	CreateRasterState();
	CreateAlphaBlendStates();

	m_swap_chain->SetFullscreenState(_start_fullscreen, nullptr);
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
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);// Bind render target.
	SetViewPort();
	ClearViews();	
}


void Renderer::SetViewPort() const
{
	auto viewport = CD3D11_VIEWPORT(0.f, 0.f, m_back_buffer_desc.Width,
		m_back_buffer_desc.Width);// Set viewport.

	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_device_context->RSSetViewports(1, &viewport);
}


void Renderer::ClearViews() const
{
	float clear_colour[] = { 0.2, 0.2, 0.180, 1 };
	m_device_context->ClearRenderTargetView(m_render_target_view, clear_colour);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void Renderer::EndFrame() const
{	
	m_swap_chain->Present(1, 0);// Swap buffer (show image) (1 is vsync).
}


DXGI_SWAP_CHAIN_DESC Renderer::CreateSwapChainDesc(Window& _window) const
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc {};

	swap_chain_desc.BufferDesc.Width = _window.GetWindowWidth();
	swap_chain_desc.BufferDesc.Height = _window.GetWindowHeight();
	swap_chain_desc.BufferCount = 1;// Double buffer.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// Colour format (RGB 32 bit).
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = _window.GetHandle();// Set target.
	swap_chain_desc.SampleDesc.Count = 8;// Antialiasing.
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
	// Create the swap chain device and context.
	auto result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &_swap_chain_desc,
		&m_swap_chain, &m_device, nullptr, &m_device_context);

	
	if (result != S_OK)// Check errors.
	{
		MessageBox(nullptr, "[Renderer](CreateDeviceContext) Error creating DX11", "Error", MB_OK);
		exit(0);
	}
}


void Renderer::CreateRenderTarget()
{
	ID3D11Texture2D* back_buffer;
	m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));// Get buffer.
	m_device->CreateRenderTargetView(back_buffer, nullptr, &m_render_target_view);// Set render target.
	
	back_buffer->GetDesc(&m_back_buffer_desc);
	back_buffer->Release();// Decrement instance count.
}


void Renderer::CreateDepthBuffer()
{
	HRESULT result {};
	D3D11_TEXTURE2D_DESC buffer_desc {};
	
	// Set up the description of the depth buffer.
	buffer_desc.Width = m_target_window->GetWindowWidth();
	buffer_desc.Height = m_target_window->GetWindowHeight();
	buffer_desc.MipLevels = 1;
	buffer_desc.ArraySize = 1;
	buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	buffer_desc.SampleDesc.Count = 8;
	buffer_desc.SampleDesc.Quality = 0;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	// Create Depth Buffer.
	result = m_device->CreateTexture2D(&buffer_desc, nullptr, &m_depth_stencil_buffer);
	if (result != S_OK)
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create Depth buffer", "Error", MB_OK);
		exit(0);
	}
}


void Renderer::CreateDepthStencilState()
{
	HRESULT result {};
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc {};

	// Set up the description of the stencil state.
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0;
	depth_stencil_desc.StencilWriteMask = 0;

	// Stencil operations if pixel is front-facing.
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);// Create the depth stencil state.
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
	HRESULT result {};
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc {};

	// Set up the depth stencil view description.
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	depth_stencil_view_desc.Flags = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
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
	D3D11_RASTERIZER_DESC rasterizer_desc {};

	//Set up rasteriser description.
	rasterizer_desc.AntialiasedLineEnable = false;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;// back face cuulling
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0.0f;
	rasterizer_desc.DepthClipEnable = true;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FrontCounterClockwise = false;
	rasterizer_desc.MultisampleEnable = true;
	rasterizer_desc.ScissorEnable = false;
	rasterizer_desc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterizer_desc, &m_raster_state);
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
	D3D11_BLEND_DESC blend_desc {};

	// Create alpha enabled blend state description.
	blend_desc.RenderTarget[0].BlendEnable = true;

	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = 15;
	
	// Create the blend state.
	result = m_device->CreateBlendState(&blend_desc, &m_alpha_enable_blending_state);
	if (FAILED(result))
	{
		MessageBox(nullptr, "[Renderer](CreateDepthBuffer) Failed to create blend state", "Error", MB_OK);
		exit(0);
	}

	// MDisabled blend state description.
	blend_desc.RenderTarget[0].BlendEnable = false;

	// Create disabled blend state.
	result = m_device->CreateBlendState(&blend_desc, &m_alpha_disable_blending_state);
}




void Renderer::TurnOnAlphaBlending() const
{
	float blend_factor[4];

	// Setup the blend factor.
	blend_factor[0] = 0.0f;
	blend_factor[1] = 0.0f;
	blend_factor[2] = 0.0f;
	blend_factor[3] = 0.0f;

	// Turn on alpha blending.
	m_device_context->OMSetBlendState(m_alpha_enable_blending_state, blend_factor, 0xffffffff);
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