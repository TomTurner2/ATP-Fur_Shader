#include "Renderer.h"
#include "Window.h"


Renderer::Renderer(Window& _window)
{
	// Swap chain (double buffering)
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = CreateSwapChainDesc(_window);
	CreateDeviceContext(swap_chain_desc);
	CreateRenderTarget();
	m_render_data = std::make_unique<RenderData>();
	m_target_window = &_window;
}


void Renderer::BeginFrame() const
{
	// Bind render target
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);

	// Set viewport
	auto viewport = CD3D11_VIEWPORT(0.f, 0.f, m_back_buffer_desc.Width,
		m_back_buffer_desc.Width);
	m_device_context->RSSetViewports(1, &viewport);

	// Set clear colour
	float clear_colour[] = { 0.2, 0.2, 0.180, 1 };
	m_device_context->ClearRenderTargetView(m_render_target_view, clear_colour);
}


void Renderer::EndFrame() const
{
	// Swap buffer
	m_swap_chain->Present(1, 0);//Shows image (1 is vsync)
}


DXGI_SWAP_CHAIN_DESC Renderer::CreateSwapChainDesc(Window& _window) const
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };

	swap_chain_desc.BufferCount = 1;// Double buffer
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// Colour format (RGB 32 bit)
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = _window.GetHandle();// Set target
	swap_chain_desc.SampleDesc.Count = 8;// Antialiasing
	swap_chain_desc.Windowed = true;

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


Window * Renderer::GetTargetWindow() const
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